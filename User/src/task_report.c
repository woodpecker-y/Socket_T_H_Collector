#include "task_report.h"
#include "nb.h"
#include "bsp.h"
#include "sysparams.h"
#include "protocol.h"
#include "task_disp.h"
#include "update_cycle.h"
#include "task_disp.h"

//#define HAVE_NB_TEST

//u8   task_report_max_rssi_band(u8 *band_rssi, u8 cnt);
void task_report_display_triggered(void);

/*判断NB模块是否具有上报的条件*/
void task_report_detect(void);

static ReportHandler s_report_handler;

void task_report_init(void)
{
    s_report_handler.sts = E_REPORT_IDLE;
    s_report_handler.connected_state = 0;        //连接状态设置为0
    s_report_handler.max_connect_timeout = 70;

    s_report_handler.timer = 3 * UNIT_SECOND;
}

//void task_report_hexdump(u8 *dat, u16 len)
//{
//    u8 i = 0;
//
//    for (i=0; i<len; ++i)
//    {
//        printf("%02X ", dat[i]);
//    }
//
//    printf("\r\n");
//}

void task_report_proc(void)
{
    u8 rv = 0xFF;
    LogData log;
    LogData now_data;

    task_report_detect();

    switch(s_report_handler.sts)
    {
    case E_REPORT_IDLE: //空闲
        break;
    case E_REPORT_READY://准备
        rv = BSP_NB_CheckIsReady(g_run_params.re_power_on_flag, g_band_list[g_sys_params.band_no], (char*)g_sys_params.apn, (char*)g_sys_params.ip, &g_sys_params.port, (char*)g_sys_params.addr);
        if ( rv == 0 )
        {
            // 如果设备未出厂，则写设备编号和出厂标志到系统参数区
            if (g_sys_params.factory_flag == 0)
            {
                g_sys_params.factory_flag = 1;

                sysparams_write(E_ADDR_OFFSET_FACTORY, &g_sys_params.factory_flag, sizeof(g_sys_params.factory_flag));
                sysparams_write(E_ADDR_OFFSET_DEV_ADDR, g_sys_params.addr, sizeof(g_sys_params.addr));
            }
            s_report_handler.sts = E_REPORT_NET;
            break;
        }

        // 超时处理30s,发送失败时会自动重发命令，发送超时间隔默认1s，可通过BSP_NB_SetRepeatSendCounter配置
        s_report_handler.timeout_timer--;
        if (s_report_handler.timeout_timer <= 0)
        {
            s_report_handler.timeout_timer = 0;
            s_report_handler.sts = E_REPORT_REBOOT;
            s_report_handler.connected_state = 0;
            s_report_handler.connect_failed_cnt++;
        }
        break;
    case E_REPORT_NET: //入网
        rv = BSP_NB_CheckNet(&g_run_params.rssi);
        if(rv == 0)
        {
            // 1.初次上电
            // 1.1联网成功，信号值大于等于15，直接选择
            // 1.2联网成功，已经连过1次，联通仅支持2个频段，因此大于1说明第一次不理想，第二次也不理想，小于15，直接选择频道2
            // 2.模组复位
            if ((g_run_params.re_power_on_flag == 1 && (g_run_params.rssi >= 15 || s_report_handler.poweron_connect_counter > 1))
              || g_run_params.re_power_on_flag == 0)
            {
                // 重置联网倒计时计数器
                s_report_handler.timer = g_sys_params.upload_cycle*UNIT_MIN;
                // 切换到读取RSSI
                s_report_handler.sts = E_REPORT_GETRSSI;

                // 重新上电标志在联网成功后关闭
                g_run_params.re_power_on_flag = 0;

                // 保存频段信息
                //sysparams_write(E_ADDR_OFFSET_BAND_NO, &g_sys_params.band_no, 1);

                s_report_handler.connect_failed_cnt = 0;
            }
            else    // 初次上电，联网成功，但是信号值不够15，且是首次选网，重选
            {
                g_sys_params.band_no++;
#ifdef HAVE_CU
                if (g_sys_params.band_no >= 2)
#endif
                {
#ifdef HAVE_CU
                    g_sys_params.band_no = 0;
#elif defined(HAVE_CT)
                    g_sys_params.band_no = 2;
#endif
                }

                // 保存频段信息
                //sysparams_write(E_ADDR_OFFSET_BAND_NO, &g_sys_params.band_no, 1);

                // 初次上电选网次数
                s_report_handler.poweron_connect_counter++;
                if(s_report_handler.poweron_connect_counter>=255)
                    s_report_handler.poweron_connect_counter = 2;


                // 重启后断电
                s_report_handler.sts = E_REPORT_REBOOT;
                s_report_handler.connected_state = 0;

                // 联网失败计数器
                s_report_handler.connect_failed_cnt++;
            }
            break;
        }

        // 超时处理60s,发送失败时会自动重发命令，发送超时间隔默认1s，可通过BSP_NB_SetRepeatSendCounter配置
        s_report_handler.timeout_timer--;
        // 获取信息，并获取信号值大于18的频段
        if (s_report_handler.timeout_timer <= 0)
        {
            // 处理超时，超时也属于无信号，需要切换频段
            s_report_handler.timeout_timer = 0;

            // 需要切换频点信息，切换频段优先级
            if (g_run_params.re_power_on_flag == 1)
            {
//                g_sys_params.band_no = 0;

                // 之前从未连接成功过，依次连接网络
                g_sys_params.band_no++;
#ifdef HAVE_CU
                if (g_sys_params.band_no >= 2)
#endif
                {
#ifdef HAVE_CU
                    g_sys_params.band_no = 0;
#elif defined(HAVE_CT)
                    g_sys_params.band_no = 2;
#endif
                    //g_sys_params.band_no = task_report_max_rssi_band(g_sys_params.band_rssi, sizeof(g_sys_params.band_rssi)/sizeof(g_sys_params.band_rssi[0]));
                }

                // 保存频段信息
                //sysparams_write(E_ADDR_OFFSET_BAND_NO, &g_sys_params.band_no, 1);
//                    sysparams_write(E_ADDR_OFFSET_BAND_RSSI, g_sys_params.band_rssi, 3);

                // 初次上电选网次数
                s_report_handler.poweron_connect_counter++;
                if(s_report_handler.poweron_connect_counter>=255)
                    s_report_handler.poweron_connect_counter = 2;
            }

            // 重启后断电
            s_report_handler.sts = E_REPORT_REBOOT;
            s_report_handler.connected_state = 0;
            s_report_handler.connect_failed_cnt++;
            break;
        }
        break;
    case E_REPORT_GETRSSI://检测信号
        rv = BSP_NB_GetRSSIAndTime(&g_run_params.rssi, g_sys_params.t);
        if(rv == 0)
        {
            // 更新联网状态
            s_report_handler.connected_state = 1;

            // 本轮上传开始，上传计数器清零
            s_report_handler.upload_cnt = 0;

            // 每联网12次写内存一次RTC
            s_report_handler.rtc_update_counter--;
            if (s_report_handler.rtc_update_counter <= 0)
            {
                s_report_handler.rtc_update_counter = 24;

                //更新系统时间
                rtc_write_bcd(g_sys_params.t, sizeof(g_sys_params.t));
                sysparams_write(E_ADDR_OFFSET_TIME, g_sys_params.t, sizeof(g_sys_params.t));                // 系统时间要定期存一下
                g_run_params.rtc_updata = 1;
            }

            s_report_handler.sts = E_REPORT_RECVDATA;
            s_report_handler.timeout_timer = MAX_WAIT_RESPONSE_TIMEOUT*UNIT_SECOND;     // 读取数据超时时间

            break;
        }

        // 超时处理30s,发送失败时会自动重发命令，发送超时间隔默认1s，可通过BSP_NB_SetRepeatSendCounter配置
        s_report_handler.timeout_timer--;
        if (s_report_handler.timeout_timer <= 0)
        {
            s_report_handler.timeout_timer = 0;
            s_report_handler.sts = E_REPORT_REBOOT;

            // 联网失败
            s_report_handler.connected_state = 0;
        }
        break;
    case E_REPORT_DATA_PACK://组包
#if 0
        printf("\r\nrecords_rd:%lu, records_wr:%lu\r\n", g_sys_params.records_rd, g_sys_params.records_wr);
#endif
        if (g_sys_params.records_rd != g_sys_params.records_wr)
        {
            // 本轮上传计数器加1
            s_report_handler.upload_cnt++;

            // 读日志
            memset(&log, 0, sizeof(log));
            log_read(&log, g_sys_params.records_rd);

#if 0
            printf("[report] temp:%d, humi:%u, rssi:%d, st:%d\r\n", log.temp, log.humi, log.rssi, log.st);
            printf("time:%02X%02X-%02X-%02X %02X:%02X:%02X\r\n",
                       log.sample_time[0],
                       log.sample_time[1],
                       log.sample_time[2],
                       log.sample_time[3],
                       log.sample_time[4],
                       log.sample_time[5],
                       log.sample_time[6]);
#endif

            // 准备报文
            s_report_handler.pkg_size = sizeof(s_report_handler.pkg_data);
            memset(s_report_handler.pkg_data, 0, sizeof(s_report_handler.pkg_data));//清空上传数组
            protocol_report_request(s_report_handler.pkg_data, &s_report_handler.pkg_size, g_sys_params.addr, &log);

//            // 首次发送使用普通发送，等待命令下发，其它时候使用RAI发送快速进入PSM模式
            if (s_report_handler.upload_cnt > 1)
            {
                s_report_handler.sts = E_REPORT_REPORT;
                s_report_handler.send_mode = E_SEND_RAI_NON;//E_SEND_RAI_CON   E_SEND_RAI_NON
            }
            else
            {
                s_report_handler.sts = E_REPORT_REPORT;
                // 即使是读取服务器命令，RAI模式也可以
                s_report_handler.send_mode = E_SEND_RAI_NON;
            }

            // 发送数据超时
            s_report_handler.timeout_timer = 15*UNIT_SECOND;
        }
        else if (g_sys_params.records_rd == g_sys_params.records_wr && s_report_handler.upload_cnt == 0) // 无数据时发送空包快速进入低功耗
        {
            // 本轮上传计数器加1
            s_report_handler.upload_cnt++;

            //首次上电上报一次数据
            now_data.temp = g_run_params.temp;
            now_data.humi = g_run_params.humi/10;
            now_data.rssi = g_run_params.rssi;
            now_data.st   = 0x00;

            // 读取当前时间
            rtc_read_bcd(g_sys_params.t, 7);
            memcpy(now_data.sample_time, g_sys_params.t, 7);
            now_data.sample_time[6] = 0x00;     //美化时间秒归为0

            protocol_report_request(s_report_handler.pkg_data, &s_report_handler.pkg_size, g_sys_params.addr, &now_data);

            s_report_handler.sts = E_REPORT_REPORT;
            s_report_handler.send_mode = E_SEND_RAI_NON;

            // 发送数据超时8s
            s_report_handler.timeout_timer = 15*UNIT_SECOND;
        }
        else
        {
            s_report_handler.sts = E_REPORT_LOWPOWER_CONFIRM;
            // 发送数据超时5s
            s_report_handler.timeout_timer = 10*UNIT_SECOND;
        }

        BSP_NB_RecvInit();
        break;
    case E_REPORT_REPORT: //正常上报
        rv = BSP_NB_IPSendData(s_report_handler.pkg_data, s_report_handler.pkg_size, s_report_handler.send_mode);
        if ( rv == 0 )
        {
            BSP_NB_RecvInit();

            delay_ms(100);
            if (g_sys_params.records_rd != g_sys_params.records_wr)
            {
                // 更新读记录标志
                g_sys_params.records_rd++;
                if (g_sys_params.records_rd >= LOG_MAX_RECORDS_COUNT)
                {
                    g_sys_params.records_rd = 0;
                }

                // 存储读记录标志
                sysparams_write(E_ADDR_OFFSET_RECORDS_READ, (u8*)&g_sys_params.records_rd, sizeof(g_sys_params.records_rd));
            }
            // 切换状态：后面不进入等待模式，直接连续发送
            s_report_handler.sts = E_REPORT_DATA_PACK;

            s_report_handler.retry_cnt = 0;//只要发送成功则重试次数归0
            break;
        }

        // 超时处理3s
        s_report_handler.timeout_timer--;
        if (s_report_handler.timeout_timer <= 0)
        {
            s_report_handler.timeout_timer = 0;
            s_report_handler.sts = E_REPORT_REBOOT;
        }
        break;
    case E_REPORT_RECVDATA://接收
        rv = BSP_NB_ReadData(s_report_handler.pkg_data, &s_report_handler.pkg_size);
        if( rv == 0 )
        {
            #ifdef HAVE_NB_TEST
            printf("s_report_handler.pkg_size = %d\r\n", s_report_handler.pkg_size);
            task_report_hexdump(s_report_handler.pkg_data, s_report_handler.pkg_size);
            #endif
            // 处理下发的命令
            rv = task_report_dealwith_command(s_report_handler.pkg_data, &s_report_handler.pkg_size);
            if ( rv == 0 )
            {
                s_report_handler.sts = E_REPORT_RECVDATA;//等待接收直到没有数据 ，然后，联网首先进行接收 之后再进行 打包->发送
                // 反馈发送数据超时时间
                s_report_handler.timeout_timer = MAX_WAIT_RESPONSE_TIMEOUT*UNIT_SECOND;
                break;
            }
        }

        // 超时处理
        s_report_handler.timeout_timer--;
        if (s_report_handler.timeout_timer <= 0)
        {
            #ifdef HAVE_NB_TEST
            printf("no recv data\r\n");
            #endif
            s_report_handler.sts = E_REPORT_DATA_PACK;
        }
        break;

//    case E_REPORT_FEEDBACK:  //反馈发送
//        s_report_handler.send_mode = E_SEND_RAI_NON;
//        //task_report_hexdump(s_report_handler.pkg_data, s_report_handler.pkg_size);
//        rv = BSP_NB_IPSendData(s_report_handler.pkg_data, s_report_handler.pkg_size, s_report_handler.send_mode);
//        if ( rv == 0 )
//        {
//            //printf("BACK IS OK\r\n");
//            if (s_report_handler.reboot_flag == 0)
//            {
//                // 等待接收数据超时3s
//                s_report_handler.timeout_timer = MAX_WAIT_RESPONSE_TIMEOUT*UNIT_SECOND;
//                s_report_handler.sts = E_REPORT_RECVDATA;
//
//                // 增加反馈发送成功率
//                delay_ms(20);
//
//                break;
////                // 是否有其余的数据待上传
////                s_report_handler.sts = E_REPORT_DATA_PACK;
//            }
//            else
//            {
//                // 发送完毕后设备立即重启，通过使看门狗溢出的方法溢出
//                while (1);
//            }
//        }
//
//        // 超时处理8s
//        s_report_handler.timeout_timer--;
//        if (s_report_handler.timeout_timer <= 0)
//        {
//            // 是否有其余的数据待上传
//            s_report_handler.sts = E_REPORT_DATA_PACK;
//        }
//        break;

    case E_REPORT_REBOOT:        // 软重启
        {
            u16 factor = 0;

            s_report_handler.connected_state = 0;
            g_run_params.rssi = 0;                      // 信号值置0
            task_disp_triggered(E_TASK_DISP_ANT);       // 刷新信号

            if(s_report_handler.retry_cnt >= 3)         // 重复链接3次如果没有链接成功则关闭模块电源等下次周期再次链接--重链接
            {
                s_report_handler.retry_cnt = 0;
                factor = (g_sys_params.addr[13]-0x30)*100 + (g_sys_params.addr[14]-0x30)*10 + (g_sys_params.addr[15]-0x30);
                if (g_sys_params.upload_cycle > 1)
                {
                    s_report_handler.timer = update_timer(g_sys_params.upload_cycle, 120 + factor%(g_sys_params.upload_cycle/2*60))*UNIT_SECOND;
                }
                else if (g_sys_params.upload_cycle == 1)
                {
                    s_report_handler.timer = update_timer(g_sys_params.upload_cycle, 0)*UNIT_SECOND;
                }
                BSP_NB_PowerOff();                   //断电等下次周期的时候重连
                s_report_handler.powered_on = 0;
                printf("--------------power off--------------\r\n");
            }
            else
            {
                s_report_handler.retry_cnt ++;
                s_report_handler.timer = 30*UNIT_SECOND;    //等待重连
                NB_SoftReboot();
            }

            s_report_handler.sts = E_REPORT_FINISH;

            #ifdef HAVE_NB_TEST
            printf("Reboot NB \r\n");
            #endif
            if (s_report_handler.connect_failed_cnt > MAX_CONNECT_FAILED_CNT)
            {
                s_report_handler.connect_failed_cnt = 0;
                g_run_params.re_power_on_flag = 1;      // 重新选择入网频段
            }
        }
        break;
    case E_REPORT_LOWPOWER_CONFIRM:
        rv = BSP_NB_GetPSMState();
        if (rv == 0)
        {
            #ifdef HAVE_NB_TEST
            printf("into lowpower confIRM\r\n");
            #endif
            s_report_handler.sts = E_REPORT_FINISH;
            break;
        }

        // 超时处理5s
        s_report_handler.timeout_timer--;
        if (s_report_handler.timeout_timer <= 0)
        {
            #ifdef HAVE_NB_TEST
            printf("lower confirm failed.\r\n");
            #endif
            // 进入重启模组断电状态
            s_report_handler.sts = E_REPORT_FINISH;
        }
        break;
    case E_REPORT_FINISH:
        #ifndef HAVE_NB_TEST
        com_close(COM1);
        //串口两个引脚置高电平，因为NB模块的串口是高电平，中间有一个电阻不然会形成电压差功耗变高
        GPIO_Init(GPIOA, GPIO_Pin_3, GPIO_Mode_Out_OD_HiZ_Slow);
        GPIO_Init(GPIOA, GPIO_Pin_2, GPIO_Mode_Out_PP_Low_Slow);
        #endif
        s_report_handler.sts = E_REPORT_IDLE;
        break;
    default:
        break;
    }
}

/*! \brief
*       NB模块上报触发函数
*
*/
void task_report_triggered(u8 workmode)
{
    // 上电
    if (s_report_handler.powered_on == 0)
    {
        s_report_handler.powered_on = 1;
        BSP_NB_PowerOn();
    }

    BSP_NB_Init(9600, USART_WordLength_8b, USART_StopBits_1, USART_Parity_No);

    if (s_report_handler.connected_state == 1)
    {
        s_report_handler.sts = E_REPORT_GETRSSI;  //上报
    }
    else
    {
        s_report_handler.sts = E_REPORT_READY;    //准备入网
    }

    s_report_handler.workmode = workmode;

    s_report_handler.timeout_timer = s_report_handler.max_connect_timeout * UNIT_SECOND;

//    if (s_report_handler.powered_on == 1)
//    {
//        //delay_ms(8);
//        //printf("powered on\r\n");
//    }
}



/*! \brief
*       判断NB模块是否具有上报的条件
*/
void task_report_detect(void)
{
    u16 factor = 0;

    // 周期检测
    s_report_handler.timer--;
    if ( s_report_handler.timer <= 0 )
    {
        if (s_report_handler.connected_state == 1)
        {
            // 根据上报周期的一半来设置偏移时间
            factor = (g_sys_params.addr[13]-0x30)*100 + (g_sys_params.addr[14]-0x30)*10 + (g_sys_params.addr[15]-0x30);
            if (g_sys_params.upload_cycle > 1)
            {
                s_report_handler.timer = update_timer(g_sys_params.upload_cycle, 120 + factor%(g_sys_params.upload_cycle/2*60))*UNIT_SECOND;
            }
            else if (g_sys_params.upload_cycle == 1)
            {
                s_report_handler.timer = update_timer(g_sys_params.upload_cycle, 0)*UNIT_SECOND;
            }
            //s_report_handler.timer = g_sys_params.upload_cycle*UNIT_MIN;
        }
        else if(s_report_handler.retry_cnt <= 3)
        {
            // 如果未入网，2Min启动一次入网
            s_report_handler.timer = 2*UNIT_MIN;//链接所需要的时间
        }

        task_report_triggered(WORKMODE_NORMAL);
    }

//    // 根据上报周期上报数据
//    s_report_handler.timer--;
//    if ( s_report_handler.timer <= 0 )
//    {
//        if (s_report_handler.connected_state == 0)
//        {
//            s_report_handler.timer = 2*UNIT_MIN;
//        }
//        else
//        {
//            s_report_handler.timer = g_sys_params.upload_cycle * UNIT_MIN;
//        }
//        task_report_triggered(WORKMODE_NORMAL);
//    }
}

// 读取上报进程状态机的状态
u8 task_report_get_state(void)
{
    return s_report_handler.sts;
}

u8 task_report_get_net_state(void)
{
    return s_report_handler.connected_state;
}

/*! \brief
*       解包nbiot指令下发协议处理
* \param command[IN]        - 接收到的数据
* \param len[OUT]           - 数据长度

*/
int task_report_dealwith_command(u8 *command, u16 *len)
{
    int rv = 0;
    s_report_handler.cmd = command[0];

    switch(s_report_handler.cmd)
    {
    case E_CMD_TYPE_SET_FIXTH:             // 设置补偿值
        rv = protocol_fix_t_h(&g_sys_params.fix_temp, &g_sys_params.fix_humi, (s8*)command, len);
        if (rv == 0)
        {
            //存储 补偿值
            sysparams_write(E_ADDR_OFFSET_FIX_TEMP, (u8*)&g_sys_params.fix_temp, sizeof(g_sys_params.fix_temp));
            sysparams_write(E_ADDR_OFFSET_FIX_HUMI, (u8*)&g_sys_params.fix_humi, sizeof(g_sys_params.fix_humi));
            #if 0
            printf("g_sys_params.fix_temp=%d\r\n", g_sys_params.fix_temp);
            printf("g_sys_params.fix_humi=%d\r\n", g_sys_params.fix_humi);
            #endif

            return 0;
        }
        break;
    case E_CMD_TYPE_SET_REPORTCYCLE:      // 设置上报周期
        rv = protocol_update_upload_cycle(&g_sys_params.upload_cycle, command, len);
        if (rv == 0)
        {
            //存储 上报周期
            sysparams_write(E_ADDR_OFFSET_UPLOAD_CYCLE, (u8*)&g_sys_params.upload_cycle, sizeof(g_sys_params.upload_cycle));
            #if 0
            printf("g_sys_params.upload_cycle=%d\r\n", g_sys_params.upload_cycle);
            #endif

            return 0;
        }
        break;
    default:
        return -2;
        break;
    }

    return -1;
}

void task_report_set_max_connect_timeout(s32 value)
{
    s_report_handler.max_connect_timeout = value;
}

//u8 task_report_max_rssi_band(u8 *band_rssi, u8 cnt)
//{
//    u8 i = 0;
//    u8 max = 0;
//    u8 idx = 0;
//
//    for (i=0; i<cnt; ++i)
//    {
//        if (band_rssi[i] > max)
//        {
//            max = band_rssi[i];
//            idx = i;
//        }
//    }
//
//    return idx;
//}


//        rtc_read(&t);// 供暖期判断
//        sum = (t.tm_mon+1)*100 + t.tm_mday;
//        if (sum>=g_sys_params.hp_st || sum<=g_sys_params.hp_et)
//        {//在供暖期

//         }

//          printf("Time:%02X%02X-%02X-%02X %02X:%02X:%02X\r\n", g_sys_params.t[0], g_sys_params.t[1], g_sys_params.t[2], g_sys_params.t[3], g_sys_params.t[4], g_sys_params.t[5], g_sys_params.t[6] );






