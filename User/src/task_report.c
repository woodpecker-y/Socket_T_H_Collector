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

/*�ж�NBģ���Ƿ�����ϱ�������*/
void task_report_detect(void);

static ReportHandler s_report_handler;

void task_report_init(void)
{
    s_report_handler.sts = E_REPORT_IDLE;
    s_report_handler.connected_state = 0;        //����״̬����Ϊ0
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
    case E_REPORT_IDLE: //����
        break;
    case E_REPORT_READY://׼��
        rv = BSP_NB_CheckIsReady(g_run_params.re_power_on_flag, g_band_list[g_sys_params.band_no], (char*)g_sys_params.apn, (char*)g_sys_params.ip, &g_sys_params.port, (char*)g_sys_params.addr);
        if ( rv == 0 )
        {
            // ����豸δ��������д�豸��źͳ�����־��ϵͳ������
            if (g_sys_params.factory_flag == 0)
            {
                g_sys_params.factory_flag = 1;

                sysparams_write(E_ADDR_OFFSET_FACTORY, &g_sys_params.factory_flag, sizeof(g_sys_params.factory_flag));
                sysparams_write(E_ADDR_OFFSET_DEV_ADDR, g_sys_params.addr, sizeof(g_sys_params.addr));
            }
            s_report_handler.sts = E_REPORT_NET;
            break;
        }

        // ��ʱ����30s,����ʧ��ʱ���Զ��ط�������ͳ�ʱ���Ĭ��1s����ͨ��BSP_NB_SetRepeatSendCounter����
        s_report_handler.timeout_timer--;
        if (s_report_handler.timeout_timer <= 0)
        {
            s_report_handler.timeout_timer = 0;
            s_report_handler.sts = E_REPORT_REBOOT;
            s_report_handler.connected_state = 0;
            s_report_handler.connect_failed_cnt++;
        }
        break;
    case E_REPORT_NET: //����
        rv = BSP_NB_CheckNet(&g_run_params.rssi);
        if(rv == 0)
        {
            // 1.�����ϵ�
            // 1.1�����ɹ����ź�ֵ���ڵ���15��ֱ��ѡ��
            // 1.2�����ɹ����Ѿ�����1�Σ���ͨ��֧��2��Ƶ�Σ���˴���1˵����һ�β����룬�ڶ���Ҳ�����룬С��15��ֱ��ѡ��Ƶ��2
            // 2.ģ�鸴λ
            if ((g_run_params.re_power_on_flag == 1 && (g_run_params.rssi >= 15 || s_report_handler.poweron_connect_counter > 1))
              || g_run_params.re_power_on_flag == 0)
            {
                // ������������ʱ������
                s_report_handler.timer = g_sys_params.upload_cycle*UNIT_MIN;
                // �л�����ȡRSSI
                s_report_handler.sts = E_REPORT_GETRSSI;

                // �����ϵ��־�������ɹ���ر�
                g_run_params.re_power_on_flag = 0;

                // ����Ƶ����Ϣ
                //sysparams_write(E_ADDR_OFFSET_BAND_NO, &g_sys_params.band_no, 1);

                s_report_handler.connect_failed_cnt = 0;
            }
            else    // �����ϵ磬�����ɹ��������ź�ֵ����15�������״�ѡ������ѡ
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

                // ����Ƶ����Ϣ
                //sysparams_write(E_ADDR_OFFSET_BAND_NO, &g_sys_params.band_no, 1);

                // �����ϵ�ѡ������
                s_report_handler.poweron_connect_counter++;
                if(s_report_handler.poweron_connect_counter>=255)
                    s_report_handler.poweron_connect_counter = 2;


                // ������ϵ�
                s_report_handler.sts = E_REPORT_REBOOT;
                s_report_handler.connected_state = 0;

                // ����ʧ�ܼ�����
                s_report_handler.connect_failed_cnt++;
            }
            break;
        }

        // ��ʱ����60s,����ʧ��ʱ���Զ��ط�������ͳ�ʱ���Ĭ��1s����ͨ��BSP_NB_SetRepeatSendCounter����
        s_report_handler.timeout_timer--;
        // ��ȡ��Ϣ������ȡ�ź�ֵ����18��Ƶ��
        if (s_report_handler.timeout_timer <= 0)
        {
            // ����ʱ����ʱҲ�������źţ���Ҫ�л�Ƶ��
            s_report_handler.timeout_timer = 0;

            // ��Ҫ�л�Ƶ����Ϣ���л�Ƶ�����ȼ�
            if (g_run_params.re_power_on_flag == 1)
            {
//                g_sys_params.band_no = 0;

                // ֮ǰ��δ���ӳɹ�����������������
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

                // ����Ƶ����Ϣ
                //sysparams_write(E_ADDR_OFFSET_BAND_NO, &g_sys_params.band_no, 1);
//                    sysparams_write(E_ADDR_OFFSET_BAND_RSSI, g_sys_params.band_rssi, 3);

                // �����ϵ�ѡ������
                s_report_handler.poweron_connect_counter++;
                if(s_report_handler.poweron_connect_counter>=255)
                    s_report_handler.poweron_connect_counter = 2;
            }

            // ������ϵ�
            s_report_handler.sts = E_REPORT_REBOOT;
            s_report_handler.connected_state = 0;
            s_report_handler.connect_failed_cnt++;
            break;
        }
        break;
    case E_REPORT_GETRSSI://����ź�
        rv = BSP_NB_GetRSSIAndTime(&g_run_params.rssi, g_sys_params.t);
        if(rv == 0)
        {
            // ��������״̬
            s_report_handler.connected_state = 1;

            // �����ϴ���ʼ���ϴ�����������
            s_report_handler.upload_cnt = 0;

            // ÿ����12��д�ڴ�һ��RTC
            s_report_handler.rtc_update_counter--;
            if (s_report_handler.rtc_update_counter <= 0)
            {
                s_report_handler.rtc_update_counter = 24;

                //����ϵͳʱ��
                rtc_write_bcd(g_sys_params.t, sizeof(g_sys_params.t));
                sysparams_write(E_ADDR_OFFSET_TIME, g_sys_params.t, sizeof(g_sys_params.t));                // ϵͳʱ��Ҫ���ڴ�һ��
                g_run_params.rtc_updata = 1;
            }

            s_report_handler.sts = E_REPORT_RECVDATA;
            s_report_handler.timeout_timer = MAX_WAIT_RESPONSE_TIMEOUT*UNIT_SECOND;     // ��ȡ���ݳ�ʱʱ��

            break;
        }

        // ��ʱ����30s,����ʧ��ʱ���Զ��ط�������ͳ�ʱ���Ĭ��1s����ͨ��BSP_NB_SetRepeatSendCounter����
        s_report_handler.timeout_timer--;
        if (s_report_handler.timeout_timer <= 0)
        {
            s_report_handler.timeout_timer = 0;
            s_report_handler.sts = E_REPORT_REBOOT;

            // ����ʧ��
            s_report_handler.connected_state = 0;
        }
        break;
    case E_REPORT_DATA_PACK://���
#if 0
        printf("\r\nrecords_rd:%lu, records_wr:%lu\r\n", g_sys_params.records_rd, g_sys_params.records_wr);
#endif
        if (g_sys_params.records_rd != g_sys_params.records_wr)
        {
            // �����ϴ���������1
            s_report_handler.upload_cnt++;

            // ����־
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

            // ׼������
            s_report_handler.pkg_size = sizeof(s_report_handler.pkg_data);
            memset(s_report_handler.pkg_data, 0, sizeof(s_report_handler.pkg_data));//����ϴ�����
            protocol_report_request(s_report_handler.pkg_data, &s_report_handler.pkg_size, g_sys_params.addr, &log);

//            // �״η���ʹ����ͨ���ͣ��ȴ������·�������ʱ��ʹ��RAI���Ϳ��ٽ���PSMģʽ
            if (s_report_handler.upload_cnt > 1)
            {
                s_report_handler.sts = E_REPORT_REPORT;
                s_report_handler.send_mode = E_SEND_RAI_NON;//E_SEND_RAI_CON   E_SEND_RAI_NON
            }
            else
            {
                s_report_handler.sts = E_REPORT_REPORT;
                // ��ʹ�Ƕ�ȡ���������RAIģʽҲ����
                s_report_handler.send_mode = E_SEND_RAI_NON;
            }

            // �������ݳ�ʱ
            s_report_handler.timeout_timer = 15*UNIT_SECOND;
        }
        else if (g_sys_params.records_rd == g_sys_params.records_wr && s_report_handler.upload_cnt == 0) // ������ʱ���Ϳհ����ٽ���͹���
        {
            // �����ϴ���������1
            s_report_handler.upload_cnt++;

            //�״��ϵ��ϱ�һ������
            now_data.temp = g_run_params.temp;
            now_data.humi = g_run_params.humi/10;
            now_data.rssi = g_run_params.rssi;
            now_data.st   = 0x00;

            // ��ȡ��ǰʱ��
            rtc_read_bcd(g_sys_params.t, 7);
            memcpy(now_data.sample_time, g_sys_params.t, 7);
            now_data.sample_time[6] = 0x00;     //����ʱ�����Ϊ0

            protocol_report_request(s_report_handler.pkg_data, &s_report_handler.pkg_size, g_sys_params.addr, &now_data);

            s_report_handler.sts = E_REPORT_REPORT;
            s_report_handler.send_mode = E_SEND_RAI_NON;

            // �������ݳ�ʱ8s
            s_report_handler.timeout_timer = 15*UNIT_SECOND;
        }
        else
        {
            s_report_handler.sts = E_REPORT_LOWPOWER_CONFIRM;
            // �������ݳ�ʱ5s
            s_report_handler.timeout_timer = 10*UNIT_SECOND;
        }

        BSP_NB_RecvInit();
        break;
    case E_REPORT_REPORT: //�����ϱ�
        rv = BSP_NB_IPSendData(s_report_handler.pkg_data, s_report_handler.pkg_size, s_report_handler.send_mode);
        if ( rv == 0 )
        {
            BSP_NB_RecvInit();

            delay_ms(100);
            if (g_sys_params.records_rd != g_sys_params.records_wr)
            {
                // ���¶���¼��־
                g_sys_params.records_rd++;
                if (g_sys_params.records_rd >= LOG_MAX_RECORDS_COUNT)
                {
                    g_sys_params.records_rd = 0;
                }

                // �洢����¼��־
                sysparams_write(E_ADDR_OFFSET_RECORDS_READ, (u8*)&g_sys_params.records_rd, sizeof(g_sys_params.records_rd));
            }
            // �л�״̬�����治����ȴ�ģʽ��ֱ����������
            s_report_handler.sts = E_REPORT_DATA_PACK;

            s_report_handler.retry_cnt = 0;//ֻҪ���ͳɹ������Դ�����0
            break;
        }

        // ��ʱ����3s
        s_report_handler.timeout_timer--;
        if (s_report_handler.timeout_timer <= 0)
        {
            s_report_handler.timeout_timer = 0;
            s_report_handler.sts = E_REPORT_REBOOT;
        }
        break;
    case E_REPORT_RECVDATA://����
        rv = BSP_NB_ReadData(s_report_handler.pkg_data, &s_report_handler.pkg_size);
        if( rv == 0 )
        {
            #ifdef HAVE_NB_TEST
            printf("s_report_handler.pkg_size = %d\r\n", s_report_handler.pkg_size);
            task_report_hexdump(s_report_handler.pkg_data, s_report_handler.pkg_size);
            #endif
            // �����·�������
            rv = task_report_dealwith_command(s_report_handler.pkg_data, &s_report_handler.pkg_size);
            if ( rv == 0 )
            {
                s_report_handler.sts = E_REPORT_RECVDATA;//�ȴ�����ֱ��û������ ��Ȼ���������Ƚ��н��� ֮���ٽ��� ���->����
                // �����������ݳ�ʱʱ��
                s_report_handler.timeout_timer = MAX_WAIT_RESPONSE_TIMEOUT*UNIT_SECOND;
                break;
            }
        }

        // ��ʱ����
        s_report_handler.timeout_timer--;
        if (s_report_handler.timeout_timer <= 0)
        {
            #ifdef HAVE_NB_TEST
            printf("no recv data\r\n");
            #endif
            s_report_handler.sts = E_REPORT_DATA_PACK;
        }
        break;

//    case E_REPORT_FEEDBACK:  //��������
//        s_report_handler.send_mode = E_SEND_RAI_NON;
//        //task_report_hexdump(s_report_handler.pkg_data, s_report_handler.pkg_size);
//        rv = BSP_NB_IPSendData(s_report_handler.pkg_data, s_report_handler.pkg_size, s_report_handler.send_mode);
//        if ( rv == 0 )
//        {
//            //printf("BACK IS OK\r\n");
//            if (s_report_handler.reboot_flag == 0)
//            {
//                // �ȴ��������ݳ�ʱ3s
//                s_report_handler.timeout_timer = MAX_WAIT_RESPONSE_TIMEOUT*UNIT_SECOND;
//                s_report_handler.sts = E_REPORT_RECVDATA;
//
//                // ���ӷ������ͳɹ���
//                delay_ms(20);
//
//                break;
////                // �Ƿ�����������ݴ��ϴ�
////                s_report_handler.sts = E_REPORT_DATA_PACK;
//            }
//            else
//            {
//                // ������Ϻ��豸����������ͨ��ʹ���Ź�����ķ������
//                while (1);
//            }
//        }
//
//        // ��ʱ����8s
//        s_report_handler.timeout_timer--;
//        if (s_report_handler.timeout_timer <= 0)
//        {
//            // �Ƿ�����������ݴ��ϴ�
//            s_report_handler.sts = E_REPORT_DATA_PACK;
//        }
//        break;

    case E_REPORT_REBOOT:        // ������
        {
            u16 factor = 0;

            s_report_handler.connected_state = 0;
            g_run_params.rssi = 0;                      // �ź�ֵ��0
            task_disp_triggered(E_TASK_DISP_ANT);       // ˢ���ź�

            if(s_report_handler.retry_cnt >= 3)         // �ظ�����3�����û�����ӳɹ���ر�ģ���Դ���´������ٴ�����--������
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
                BSP_NB_PowerOff();                   //�ϵ���´����ڵ�ʱ������
                s_report_handler.powered_on = 0;
                printf("--------------power off--------------\r\n");
            }
            else
            {
                s_report_handler.retry_cnt ++;
                s_report_handler.timer = 30*UNIT_SECOND;    //�ȴ�����
                NB_SoftReboot();
            }

            s_report_handler.sts = E_REPORT_FINISH;

            #ifdef HAVE_NB_TEST
            printf("Reboot NB \r\n");
            #endif
            if (s_report_handler.connect_failed_cnt > MAX_CONNECT_FAILED_CNT)
            {
                s_report_handler.connect_failed_cnt = 0;
                g_run_params.re_power_on_flag = 1;      // ����ѡ������Ƶ��
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

        // ��ʱ����5s
        s_report_handler.timeout_timer--;
        if (s_report_handler.timeout_timer <= 0)
        {
            #ifdef HAVE_NB_TEST
            printf("lower confirm failed.\r\n");
            #endif
            // ��������ģ��ϵ�״̬
            s_report_handler.sts = E_REPORT_FINISH;
        }
        break;
    case E_REPORT_FINISH:
        #ifndef HAVE_NB_TEST
        com_close(COM1);
        //�������������øߵ�ƽ����ΪNBģ��Ĵ����Ǹߵ�ƽ���м���һ�����費Ȼ���γɵ�ѹ��ı��
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
*       NBģ���ϱ���������
*
*/
void task_report_triggered(u8 workmode)
{
    // �ϵ�
    if (s_report_handler.powered_on == 0)
    {
        s_report_handler.powered_on = 1;
        BSP_NB_PowerOn();
    }

    BSP_NB_Init(9600, USART_WordLength_8b, USART_StopBits_1, USART_Parity_No);

    if (s_report_handler.connected_state == 1)
    {
        s_report_handler.sts = E_REPORT_GETRSSI;  //�ϱ�
    }
    else
    {
        s_report_handler.sts = E_REPORT_READY;    //׼������
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
*       �ж�NBģ���Ƿ�����ϱ�������
*/
void task_report_detect(void)
{
    u16 factor = 0;

    // ���ڼ��
    s_report_handler.timer--;
    if ( s_report_handler.timer <= 0 )
    {
        if (s_report_handler.connected_state == 1)
        {
            // �����ϱ����ڵ�һ��������ƫ��ʱ��
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
            // ���δ������2Min����һ������
            s_report_handler.timer = 2*UNIT_MIN;//��������Ҫ��ʱ��
        }

        task_report_triggered(WORKMODE_NORMAL);
    }

//    // �����ϱ������ϱ�����
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

// ��ȡ�ϱ�����״̬����״̬
u8 task_report_get_state(void)
{
    return s_report_handler.sts;
}

u8 task_report_get_net_state(void)
{
    return s_report_handler.connected_state;
}

/*! \brief
*       ���nbiotָ���·�Э�鴦��
* \param command[IN]        - ���յ�������
* \param len[OUT]           - ���ݳ���

*/
int task_report_dealwith_command(u8 *command, u16 *len)
{
    int rv = 0;
    s_report_handler.cmd = command[0];

    switch(s_report_handler.cmd)
    {
    case E_CMD_TYPE_SET_FIXTH:             // ���ò���ֵ
        rv = protocol_fix_t_h(&g_sys_params.fix_temp, &g_sys_params.fix_humi, (s8*)command, len);
        if (rv == 0)
        {
            //�洢 ����ֵ
            sysparams_write(E_ADDR_OFFSET_FIX_TEMP, (u8*)&g_sys_params.fix_temp, sizeof(g_sys_params.fix_temp));
            sysparams_write(E_ADDR_OFFSET_FIX_HUMI, (u8*)&g_sys_params.fix_humi, sizeof(g_sys_params.fix_humi));
            #if 0
            printf("g_sys_params.fix_temp=%d\r\n", g_sys_params.fix_temp);
            printf("g_sys_params.fix_humi=%d\r\n", g_sys_params.fix_humi);
            #endif

            return 0;
        }
        break;
    case E_CMD_TYPE_SET_REPORTCYCLE:      // �����ϱ�����
        rv = protocol_update_upload_cycle(&g_sys_params.upload_cycle, command, len);
        if (rv == 0)
        {
            //�洢 �ϱ�����
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


//        rtc_read(&t);// ��ů���ж�
//        sum = (t.tm_mon+1)*100 + t.tm_mday;
//        if (sum>=g_sys_params.hp_st || sum<=g_sys_params.hp_et)
//        {//�ڹ�ů��

//         }

//          printf("Time:%02X%02X-%02X-%02X %02X:%02X:%02X\r\n", g_sys_params.t[0], g_sys_params.t[1], g_sys_params.t[2], g_sys_params.t[3], g_sys_params.t[4], g_sys_params.t[5], g_sys_params.t[6] );






