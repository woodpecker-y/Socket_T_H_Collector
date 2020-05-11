#ifdef HAVE_NB
//#define HAVE_NB_LOG

#include <stdlib.h>
#include <stdio.h>
#include "bsp.h"
#include "nb.h"
#include "sysparams.h"
#include "utili.h"

// 模块配置句柄
static NBConfig *sg_nb_cfg_handler = NULL;

// 模块操作句柄
static NBHandler sg_nb_handler;

/*! \brief
*       读出字符串中第一个连续的数字 字符串不能读取16进制数据A--F
*/
static char* ReadNumStr(char* str, char* num_str)
{
    int i = 0;
    u16 num = 0;

    for(i = 0; i<strlen(str); i++)
    {
        if(str[i]>='0' && str[i]<='9'){
            num++;
        }else{
            if(num>0)
            break;
        }
    }
    strncpy(num_str, str+(i-num), num);

    return num_str;
}

/*! \brief
*       见数字字符串解析为十进制数字,数字不能超过 0XFF FF FF FF
* \param msg[IN]             - 从A6模块接收数据的指针
*
* \return
*       E_GPRS_OK       - 成功
*       Others          - 失败
*/
u32 ReadNum(char* msg)
{
    int i = 0;
    u32 num = 0;

    while(msg[i]>='0' && msg[i]<='9')
    {
        num = num*10 + msg[i] - '0';
        i++;
    }

    return num;
}

//+CCLK:19/06/03,14:15:29+32
// 时间格式为:18/11/6 17:39:00 //解析NB时间
int NB_TimerParser(u8 *time, char *time_str)
{
    char *p = NULL;
    time_t t1 = 0;
    struct tm t2;

    u16 year = 0, month = 0, day = 0, hour = 0, min = 0, second = 0;

    //printf("time:%s\r\n", time_str);

    // 年
    year = ReadNum(time_str) + 2000;
    //printf("year:%u\r\n", year);

    // 月
    p = strstr((char*)time_str, "/");
    if (p == NULL)
    {
        return -1;
    }
    month = ReadNum(p+strlen("/"));

    // 日
    p = strstr((char*)p+strlen("/"), "/");
    if (p == NULL)
    {
        return -1;
    }
    day = ReadNum(p+strlen("/"));

    // 时
    p = strstr((char*)p+strlen("/"), ",");
    if (p == NULL)
    {
        return -1;
    }
    hour = ReadNum(p+strlen(","));

    // 分
    p = strstr((char*)p+strlen("/"), ":");
    if (p == NULL)
    {
        return -1;
    }
    min = ReadNum(p+strlen(":"));

    // 秒
    p = strstr((char*)p+strlen("/"), ":");
    if (p == NULL)
    {
        return -1;
    }
    second = ReadNum(p+strlen(":"));

    // 将时区修改为东八区
    t1 = mktime_user(year, month, day, hour, min, second);
    t1 += 8*3600;

    localtime_user(t1, &t2);

    // 转换为BCD码时间格式
    dec_2_bcd_type(time+0, 2, t2.tm_year+1900, ORD_MOTOR);
    dec_2_bcd_type(time+2, 1, t2.tm_mon+1,  ORD_MOTOR);
    dec_2_bcd_type(time+3, 1, t2.tm_mday, ORD_MOTOR);
    dec_2_bcd_type(time+4, 1, t2.tm_hour, ORD_MOTOR);
    dec_2_bcd_type(time+5, 1, t2.tm_min,  ORD_MOTOR);
    dec_2_bcd_type(time+6, 1, t2.tm_sec,  ORD_MOTOR);

    return 0;
}

/*! \brief
*       发送数据给模块（发送AT指令使用）
* \param dat[IN]            - 发送给模块的数据指针
* \param len[IN]            - 发送给模块的数据长度
*
* \return
*       E_NB_OK     - 成功
*       Others          - 失败
*/
u8 NB_SendData(const u8 *dat, u16 len)
{
    u8 rv = 0;

    BSP_COM_RecvInit(sg_nb_cfg_handler->com);
    rv = BSP_COM_SendData(sg_nb_cfg_handler->com, dat, len);
    if (rv != 0)
    {
        return rv;
    }

    return rv;
}

/*! \brief
*       从模块接收数据（接收模块发送的指令反馈）
* \param dat[OUT]            - 从模块接收数据的指针
* \param len[OUT]            - 从模块接收数据的长度
*
* \return
*       E_NB_OK     - 成功
*       Others          - 失败
* \notes
*       变量dat为二级指针，不需要为它分配内存缓冲，它直接指向串口内部缓冲区的数据，
*   直接使用即可，在调用NB_recv_init()或NB_send_data()函数之前数据一直存在。
*/
static u8 NB_RecvData(u8 **dat, u16 *len)
{
    return BSP_COM_RecvDataStream(sg_nb_cfg_handler->com, dat, len);
}

/*! \brief
*       检查模块的反馈是否为OK
* \param msg[IN]                - 从模块接收的数据
*
* \return
*       E_NB_OK             - 成功
*       E_NB_ERR_OTHERS       - 失败
*/
static u8 NB_CheckRespIsOK(char *msg)
{
    //printf("msg:%s", msg);
    if (strstr(msg, "ERROR") != NULL)      //只要返回的报文中有ERROR字样说明有错误
    {
        return E_NB_ERR_MODULE_RESPONSE_ERROR;
    }
    else if (strstr(msg, "OK") != NULL)
    {
        return E_NB_OK;
    }
    else
    {
        return E_NB_PROCESSING;
    }
}

/*! \brief
*       // 判断是否收到了\r\n结束符
* \param msg[IN]                - 从模块接收的数据
*
* \return
*       E_NB_OK             - 成功
*       E_NB_ERR_OTHERS       - 失败
*/
static u8 NB_CheckRespIsRecvComplete(char *msg)
{
    //printf("msg:%s", msg);
    if (strstr(msg, "ERROR") != NULL)      //只要返回的报文中有ERROR字样说明有错误
    {
        return E_NB_ERR_MODULE_RESPONSE_ERROR;
    }
    else if (strstr(msg, "\r\n") != NULL)
    {
        return E_NB_OK;
    }
    else
    {
        return E_NB_PROCESSING;
    }
}

/*! \brief
*       NB常见AT命令处理状态机
* \param cmd[IN]             - 命令类型码
* \param dat[IN]             - AT命令报文
* \param len[IN]             - AT命令报文长度
* \param delay_ack_timer[IN] - 等待AT反馈时间
* \return
*       E_NB_OK           - 成功
*       E_NB_PROCESSING   - 处理中
*       E_NB_ERR_OTHERS   - 错误
*/
static u8 NB_Config(u8 cmd, const u8* dat, u16 len, u16 delay_ack_timer)
{
    u8 rv = -1;
    u8 *rcv_ptr = NULL;
    u16 rcv_len = 0;
    //u8  pkg_data_len = 0;

    char *p = NULL;

    if (sg_nb_handler.cmd != cmd)
    {
        sg_nb_handler.sts = E_SEND_CMD;
    }

    switch(sg_nb_handler.sts)
    {
    case E_SEND_CMD:
//#ifdef HAVE_NB_LOG
        //printf("%s\r\n", dat);
//#endif
        BSP_NB_RecvInit();
        NB_SendData(dat, len);

        sg_nb_handler.sts = E_IS_OK;
        sg_nb_handler.cmd = cmd;
        break;
    case E_IS_OK://对比回复的是否是   “OK”
        rv = NB_RecvData(&rcv_ptr, &rcv_len);
        if(0 == rv)
        {
//#ifdef HAVE_NB_LOG
//            printf("%s\r\n", rcv_ptr);
//#endif
            if(sg_nb_handler.cmd == E_AT_SEND_ACK)
            {
                p = strstr((char*)rcv_ptr, "ATASTATUS:4");
                if(p != NULL)
                {
                    //printf("+QLWLDATASTATUS:4 OK\r\n");//BC95-B5(移远)+QLWULDATASTATUS :4  /  ML5515(骐俊)+MLWULDATASTATUS:4

                    //BSP_NB_RecvInit();
                    return E_NB_OK;
                }
                else
                {
                    sg_nb_handler.sts = E_IS_OK;
                    BSP_NB_RecvInit();

                    return E_NB_PROCESSING;
                }
            }

            //printf("rcv_len:%u\r\n", rcv_len);

            rv = NB_CheckRespIsOK((char*)rcv_ptr);
            if(rv == E_NB_OK)
            {
                if(    sg_nb_handler.cmd == E_AT_CGSN
                    || sg_nb_handler.cmd == E_AT_NCCID
                    || sg_nb_handler.cmd == E_AT_FIRMWARE
                    || sg_nb_handler.cmd == E_AT_CGATT_QUE
                    || sg_nb_handler.cmd == E_AT_CEREG_QUE
                    || sg_nb_handler.cmd == E_AT_CSQ
                    || sg_nb_handler.cmd == E_AT_NUESTATS
                    || sg_nb_handler.cmd == E_AT_CCLK
                    //|| sg_nb_handler.cmd == E_AT_MLWSREGIND
                    || sg_nb_handler.cmd == E_AT_NMGR
                  )
                {
                    //后面状态使用接收到的数据，故不清除串口数据
                    sg_nb_handler.sts = E_OTHER;
                    sg_nb_handler.repeat_send_timer = 0;
                }
                else
                {
//                    BSP_NB_RecvInit();
                    //printf("rcv_len:%u\r\n", rcv_len);
                    sg_nb_handler.sts = E_SEND_CMD;

                    return E_NB_OK;
                }
            }
//            else if (E_NB_ERR_MODULE_RESPONSE_ERROR == rv) // 模组反馈错误
//            {
//                sg_nb_handler.sts = E_SEND_CMD;
//
//                return rv;
//            }
//            else
//            {
//                //BSP_NB_RecvInit();
//            }
        }

        // 接收超时重发计数器或错误后延时等待发送计数器
        sg_nb_handler.repeat_send_timer++;
        //if(sg_nb_handler.repeat_send_timer >= sg_nb_handler.max_repeat_send_timer)
        if(sg_nb_handler.repeat_send_timer >= delay_ack_timer)
        {
            sg_nb_handler.repeat_send_timer = 0;
            sg_nb_handler.sts = E_SEND_CMD;
//            printf("max:%d\r\n", sg_nb_handler.max_repeat_send_timer);

            return E_NB_ERR_RECVACK;
        }
        break;
    case E_OTHER:
        // 接收超时重发计数器或错误后延时等待发送计数器
        sg_nb_handler.repeat_send_timer++;
        //if(sg_nb_handler.repeat_send_timer >= sg_nb_handler.max_repeat_send_timer)
        if(sg_nb_handler.repeat_send_timer >= delay_ack_timer)
        {
            sg_nb_handler.repeat_send_timer = 0;
            sg_nb_handler.sts = E_SEND_CMD;
            BSP_NB_RecvInit();
//            printf("max:%d\r\n", sg_nb_handler.max_repeat_send_timer);

            return E_NB_ERR_RECVACK;
        }

        NB_RecvData(&rcv_ptr, &rcv_len);

        switch(sg_nb_handler.cmd)
        {
        case E_AT_CGSN:
            p = strstr((char*)rcv_ptr, "+CGSN:");
            if (p != NULL)
            {
                memset(sg_nb_handler.imei,0,sizeof(sg_nb_handler.imei));
                ReadNumStr(p+strlen("+CGSN:"), (char*)sg_nb_handler.imei);
                //strcpy((char*)sg_nb_handler.imei, p+strlen("+CGSN:"));
                BSP_NB_RecvInit();
                //printf("nb_IMEI:%s\r\n",sg_nb_handler.imei);
                return E_NB_OK;
            }
            else
            {
                return E_NB_PROCESSING;
//                sg_nb_handler.sts = E_SEND_CMD;
//                BSP_NB_RecvInit();
//                return E_NB_ERR_REQUESTIMEI;
            }
            break;
        case E_AT_NCCID:
            p = strstr((char*)rcv_ptr, "+NCCID:");
            if (p != NULL)
            {
                memset(sg_nb_handler.iccid,0,sizeof(sg_nb_handler.iccid));
                ReadNumStr(p+strlen("+NCCID:"), (char*)sg_nb_handler.iccid);
                //strcpy((char*)sg_nb_handler.iccid, p+strlen("+NCCID:"));
                //printf("nb_ICCID:%s\r\n",sg_nb_handler.iccid);
                BSP_NB_RecvInit();
                return E_NB_OK;
            }
            else
            {
                return E_NB_PROCESSING;
//                sg_nb_handler.sts = E_SEND_CMD;
//                BSP_NB_RecvInit();
//                return E_NB_ERR_REQUESTICCID;
            }
            break;
//        case E_AT_FIRMWARE:
//            p = strstr((char*)rcv_ptr, "PROTOCOL");
//            if (p != NULL)
//            {
//                p = strstr((char*)rcv_ptr, "SP");
//                strncpy((char*)sg_nb_handler.firmware, p, 3);
//                BSP_NB_RecvInit();
//                return E_NB_OK;
//            }
//            else
//            {
//                sg_nb_handler.sts = E_SEND_CMD;
//                BSP_NB_RecvInit();
//                return E_NB_ERR_REQUESTICCID;
//            }
//            break;
        case E_AT_CGATT_QUE:
            //printf("resp[2]:%s",rcv_ptr);
            p = strstr((char*)rcv_ptr, "+CGATT:1");
            if (p != NULL)
            {
                BSP_NB_RecvInit();
                return E_NB_OK;
            }
//            else if (strstr((char*)rcv_ptr, "+CGATT:0"))
//            {
//                sg_nb_handler.repeat_send_timer = 0;
//                sg_nb_handler.sts = E_SEND_CMD;
//
//                return E_NB_ERR_RECVACK;
//            }
            else
            {
                return E_NB_PROCESSING;
//                sg_nb_handler.sts = E_SEND_CMD;
//                BSP_NB_RecvInit();
//                return E_NB_ERR_REQUESTICCID;
            }
            break;
        case E_AT_CEREG_QUE:

            p = strstr((char*)rcv_ptr, "+CEREG:1,1");
            if (p != NULL)
            {
                BSP_NB_RecvInit();
                return E_NB_OK;
            }
            else
            {
                return E_NB_PROCESSING;
//                sg_nb_handler.sts = E_SEND_CMD;
//                BSP_NB_RecvInit();
//                return E_NB_ERR_REQUESTICCID;
            }
            break;
        case E_AT_MLWSREGIND:
            //printf("resp:%s",rcv_ptr);
            p = strstr((char*)rcv_ptr, "AT+MLWEVTIND=3");
            if (p != NULL)
            {
                BSP_NB_RecvInit();
                return E_NB_OK;
            }
            else
            {
                return E_NB_PROCESSING;
//                sg_nb_handler.sts = E_SEND_CMD;
//                BSP_NB_RecvInit();
//                return E_NB_ERR_REQUESTICCID;
            }
            break;
        case E_AT_CSQ:
            p = strstr((char*)rcv_ptr, "+CSQ:");
            if (p != NULL)
            {
                //printf("%s\r\n", rcv_ptr);

                sg_nb_handler.rssi = 0;
                //sg_nb_handler.rssi = atoi(p+strlen("+CSQ:"));
                sg_nb_handler.rssi = ReadNum(p+strlen("+CSQ:"));

                if(sg_nb_handler.rssi == 99)
                {
                    sg_nb_handler.rssi = 0;
                    return E_NB_ERR_SIGNAL;
                }

                //printf("%s\r\n", rcv_ptr);

                BSP_NB_RecvInit();
                return E_NB_OK;
            }
            else
            {
                return E_NB_PROCESSING;
//                sg_nb_handler.sts = E_SEND_CMD;
//                BSP_NB_RecvInit();
//                return E_NB_ERR_SIGNAL;
            }
            break;
        case E_AT_CCLK:
            //printf("time:%s\r\n", rcv_ptr);
            p = strstr((char*)rcv_ptr, "+CCLK:");
            if (p != NULL)
            {
//                int ret = 0;
//                s16 year=0, month=0, day=0, hour=0, min=0, second=0;

                p = strstr((char*)rcv_ptr, "+CCLK:");
                //printf("time:%s\r\n", p);
                if (p != NULL)
                {
                    if (NB_TimerParser(sg_nb_handler.time, p+strlen("+CCLK:")) != 0)
                    {
                        return E_NB_ERR_TIME;
                    }
                }

                BSP_NB_RecvInit();
                return E_NB_OK;
            }
            else
            {
                return E_NB_PROCESSING;
//                sg_nb_handler.sts = E_SEND_CMD;
//                BSP_NB_RecvInit();
//                return E_NB_ERR_SIGNAL;
            }
            break;
        case E_AT_NMGR:
            //printf("rcv:%s\r\n", rcv_ptr);
            p = strstr((char*)rcv_ptr, ",");
            if (p != NULL /*&& rcv_ptr[2] !='O'*/)
            {
                //printf("rcv:%s\r\n", rcv_ptr);
                // 过滤\r\n
                sg_nb_handler.cmd_size = ReadNum((char*)rcv_ptr+2);

                //printf("cmd_size:%d\r\n", sg_nb_handler.cmd_size);

                //接收的数据
//                p = strstr((char*)rcv_ptr, ",");
//                if (p!= NULL)
//                {
                    // 跳过分隔符
                    p++;
//                    if (pkg_data_len*2 < rcv_len)
//                    {
                        //p[pkg_data_len*2] = '\0';

                        ascii_2_bytes(sg_nb_handler.cmd_dat, sg_nb_handler.cmd_size, (const char*)p, sg_nb_handler.cmd_size*2);//字符串转字节数据

                        BSP_NB_RecvInit();
                        return E_NB_OK;
//                    }
//                    else
//                    {
//                        return E_NB_ERR_MODULE_RESPONSE_ERROR;
//                    }
//                }
//                else
//                {
//                    return E_NB_PROCESSING;
//                }
            }
            else
            {
//                sg_nb_handler.sts = E_SEND_CMD;
//                BSP_NB_RecvInit();
                return E_NB_PROCESSING;
            }
            break;
//        case E_AT_NUESTATS:
////            p = strstr((char*)rcv_ptr, "Signal power:");
////            sg_nb_handler.signal_power = atoi(p+strlen("Signal power:"));
////
////            p = strstr((char*)rcv_ptr, "Total power:");
////            sg_nb_handler.total_power = atoi(p+strlen("Total power:"));
////
////            p = strstr((char*)rcv_ptr, "TX power:");
////            sg_nb_handler.tx_power = atoi(p+strlen("TX power:"));
////
////            p = strstr((char*)rcv_ptr, "Cell ID:");
////            ReadNumStr(p+strlen("Cell ID:"), (char*)sg_nb_handler.cell_id);
////
////            p = strstr((char*)rcv_ptr, "ECL:");
////            sg_nb_handler.ecl = atoi(p+strlen("ECL:"));
////
////            p = strstr((char*)rcv_ptr, "SNR:");
////            sg_nb_handler.snr = atoi(p+strlen("SNR:"));
////
////            p = strstr((char*)rcv_ptr, "PCI:");
////            sg_nb_handler.pci = atoi(p+strlen("PCI:"));
////
////            p = strstr((char*)rcv_ptr, "RSRQ:");
////            sg_nb_handler.rsrq = atoi(p+strlen("RSRQ:"));
//
//            BSP_NB_RecvInit();
//            return E_NB_OK;
//            break;
        default:
            break;
        }
        break;
    default:
        BSP_NB_RecvInit();
        return E_NB_ERR_MODULE_RESPONSE_ERROR;
        break;
    }

    return E_NB_PROCESSING;
}

/*! \brief
*       关闭AT指令回显
* \return
*       E_NB_OK           - 成功
*       E_NB_PROCESSING   - 处理中
*       E_NB_ERR_OTHERS   - 错误
*/
static u8 NB_EchoOff(void)
{
    const unsigned char cmd[] = "ATE0\r\n";
    return NB_Config(E_ATE0, cmd, strlen((char*)cmd), UNIT_SECOND/2);
}

/*! \brief
*       软件重启模组
* \return
*       E_NB_OK           - 成功
*       E_NB_PROCESSING   - 处理中
*       E_NB_ERR_OTHERS   - 错误
*/
u8 NB_SoftReboot(void)
{
    const unsigned char cmd[] = "AT+NRB\r\n";
    return NB_Config(E_AT_NRB, cmd, strlen((char*)cmd), UNIT_SECOND);
}

/*! \brief
*       使能错误提示
* \return
*       E_NB_OK           - 成功
*       E_NB_PROCESSING   - 处理中
*       E_NB_ERR_OTHERS   - 错误
*/
static u8 NB_EnableError(void)
{
    const unsigned char cmd[] = "AT+CMEE=1\r\n";
    return NB_Config(E_AT_CMEE, cmd, strlen((char*)cmd), UNIT_SECOND/2);
}

///*! \brief
//*       关机 (NB 最小功能)
//* \return
//*       E_NB_OK           - 成功
//*       E_NB_PROCESSING   - 处理中
//*       E_NB_ERR_OTHERS   - 错误
//*/
//static u8 NB_SoftOff(void)
//{
//    const unsigned char cmd[] = "AT+CFUN=0\r\n";
//    return NB_Config(E_AT_CFUN_OFF, cmd, strlen((char*)cmd), UNIT_SECOND);
//}

/*! \brief
*       配置IP及端口
* \param ip[IN]             - ip地址
* \param ip_port[IN]        - IP链接端口
* \return
*       E_NB_OK           - 成功
*       E_NB_PROCESSING   - 处理中
*       E_NB_ERR_OTHERS   - 错误
*/
static u8 NB_Ipconfig(char* ip, u16* ip_port)
{
    unsigned char cmd[100] = {0};
    sprintf((char*)cmd, "AT+NCDP=%s,%u\r\n", ip, *ip_port);
    return NB_Config(E_AT_NCDP, cmd, strlen((char*)cmd), UNIT_SECOND/2);
}

/*! \brief
*       配置APN
* \param apn[IN]        - apn
* \return
*       E_NB_OK           - 成功
*       E_NB_PROCESSING   - 处理中
*       E_NB_ERR_OTHERS   - 错误
*/
static u8 NB_Slect_APN(char* apn)
{
    unsigned char cmd[64] = {0};
    sprintf((char*)cmd, "AT+CGDCONT=1,\"IP\",\"%s\"\r\n", apn);
    return NB_Config(E_AT_CGDCONT, cmd, strlen((char*)cmd), UNIT_SECOND/2);
}

/*! \brief
*       开关机 (NB 全功能)
* \return
*       E_NB_OK           - 成功
*       E_NB_PROCESSING   - 处理中
*       E_NB_ERR_OTHERS   - 错误
*/
static u8 NB_SoftCtrl(u8 sts)
{
    unsigned char cmd[16] = {0};

    sprintf((char*)cmd, "AT+CFUN=%d\r\n", (s16)sts);

    return NB_Config(E_AT_CFUN, cmd, strlen((char*)cmd), UNIT_SECOND/2);
}

///*! \brief
//*       自动入网
//* \return
//*       E_NB_OK           - 成功
//*       E_NB_PROCESSING   - 处理中
//*       E_NB_ERR_OTHERS   - 错误
//*/
//static u8 NB_AutoConnect(void)
//{
//    const unsigned char cmd[] = "AT+NCONFIG=AUTOCONNECT,TRUE\r\n";
////    return NB_Config(E_AT_NCONFIG, cmd, strlen((char*)cmd), UNIT_SECOND);
//    return NB_Config(E_AT_NCONFIG, cmd, strlen((char*)cmd));
//}

/*! \brief
*       设置频段
* \return
*       E_NB_OK           - 成功
*       E_NB_PROCESSING   - 处理中
*       E_NB_ERR_OTHERS   - 错误
*/
static u8 NB_SetBand(char* band)
{
    unsigned char cmd[24] = {0};

#ifdef HAVE_BC95
    sprintf((char*)cmd, "AT+NBAND=?\r\n");
#endif

#ifdef HAVE_ML5515
    //sprintf((char*)cmd, "AT+NBAND=8\r\n");
    sprintf((char*)cmd, "AT+NBAND=%s\r\n", band);
#endif
    return NB_Config(E_AT_NBAND, cmd, strlen((char*)cmd), UNIT_SECOND/2);
}

/*! \brief
*       设置小区重选
* \return
*       E_NB_OK           - 成功
*       E_NB_PROCESSING   - 处理中
*       E_NB_ERR_OTHERS   - 错误
*/
static u8 NB_SetCellReselection(void)
{
    unsigned char cmd[36] = {0};

    //sprintf((char*)cmd, "AT+NCONFIG=AUTOCONNECT,TRUE\r\n");
    sprintf((char*)cmd, "AT+NCONFIG=CELL_RESELECTION,TRUE\r\n");
    return NB_Config(E_AT_CELLRESELECTION, cmd, strlen((char*)cmd), UNIT_SECOND/2);
}

/*! \brief
*       清除先验频点
* \return
*       E_NB_OK           - 成功
*       E_NB_PROCESSING   - 处理中
*       E_NB_ERR_OTHERS   - 错误
*/
static u8 NB_SetNCSEARFCN(void)
{
    const unsigned char cmd[36] = {0};
    sprintf((char*)cmd, "AT+NCSEARFCN\r\n");
    return NB_Config(E_AT_NCSEARFCN, cmd, strlen((char*)cmd), UNIT_SECOND/2);
}

/*! \brief
*       设置PSM状态
* \return
*       E_NB_OK           - 成功
*       E_NB_PROCESSING   - 处理中
*       E_NB_ERR_OTHERS   - 错误
*/
static u8 NB_SetNPSMR(void)
{
    unsigned char cmd[24] = {0};
    sprintf((char*)cmd, "AT+NPSMR=1\r\n");
    return NB_Config(E_AT_NPSMR, cmd, strlen((char*)cmd), UNIT_SECOND/2);
}

/*! \brief
*       获取PSM模式状态
*
* \return
*       E_NB_OK           - 成功
*       E_NB_ERR_RECVING  - 暂无数据
*/
u8 BSP_NB_GetPSMState(void)
{
    u8 rv = 0;
    char *recv_ptr = 0;
    u16 recv_len = 0;
//    int pkg_data_len = 0;
    char *data_ptr = NULL;

//    sg_nb_handler.cmd = E_AT_IPRECV;
    rv = NB_RecvData((u8**)&recv_ptr, &recv_len);
    if (0 == rv)
    {
        //printf("[1]recv:%s\r\n", recv_ptr);
        //<CR><LF>+NPSMR:1<CR><LF>
        data_ptr = strstr((char*)recv_ptr, "+NPSMR:1");
        if (data_ptr != NULL)
        {
            BSP_NB_RecvInit();
            return E_NB_OK;
        }
    }

    return E_NB_ERR_RECVING;
}


/*! \brief
*       查询IMEI号 此号为模组的IMEI号，为硬件的唯一号码
* \return
*       E_NB_OK           - 成功
*       E_NB_PROCESSING   - 处理中
*       E_NB_ERR_OTHERS   - 错误
*/
static u8 NB_RequestIMEI(void)
{
    const unsigned char cmd[] = "AT+CGSN=1\r\n";
    return NB_Config(E_AT_CGSN, cmd, strlen((char*)cmd), UNIT_SECOND/2);
}

/*! \brief
*       查询ICCID号 此号为卡的ID号，是硬件卡的唯一号码
* \param mode[IN]
*
* \return
*       E_NB_OK           - 成功
*       E_NB_PROCESSING   - 处理中
*       E_NB_ERR_OTHERS   - 错误
*/
static u8 NB_RequestICCID(void)
{
    const unsigned char cmd[] = "AT+NCCID\r\n";
    return NB_Config(E_AT_NCCID, cmd, strlen((char*)cmd), UNIT_SECOND/2);
}

/*! \brief
*       查询IMSI号 此号为类似手机号，是虚拟号码
* \param mode[IN]
*
* \return
*       E_NB_OK           - 成功
*       E_NB_PROCESSING   - 处理中
*       E_NB_ERR_OTHERS   - 错误
*/
static u8 NB_RequestIMSI(void)
{
    const unsigned char cmd[] = "AT+CIMI\r\n";
    return NB_Config(E_AT_CIMI, cmd, strlen((char*)cmd), UNIT_SECOND/2);
}

///*! \brief
//*       查询固件版本号
//* \param mode[IN]
//*
//* \return
//*       E_NB_OK           - 成功
//*       E_NB_PROCESSING   - 处理中
//*       E_NB_ERR_OTHERS   - 错误
//*/
//static u8 NB_RequestFirmware(void)
//{
//    const unsigned char cmd[] = "AT+CGMR\r\n";
////    return NB_Config(E_AT_FIRMWARE, cmd, strlen((char*)cmd), UNIT_SECOND);
//    return NB_Config(E_AT_FIRMWARE, cmd, strlen((char*)cmd));
//}

/*! \brief
*       激活网络
* \param mode[IN]
*
* \return
*       E_NB_OK           - 成功
*       E_NB_PROCESSING   - 处理中
*       E_NB_ERR_OTHERS   - 错误
*/
static u8 NB_ActivateNet(void)
{
    const unsigned char cmd[] = "AT+CGATT=1\r\n";
    return NB_Config(E_AT_CGATT, cmd, strlen((char*)cmd), UNIT_SECOND);
}

/*! \brief
*       查询是否激活网络
* \param mode[IN]
*
* \return
*       E_NB_OK           - 成功
*       E_NB_PROCESSING   - 处理中
*       E_NB_ERR_OTHERS   - 错误
*/
static u8 NB_QueActivateNet(void)
{
    const unsigned char cmd[] = "AT+CGATT?\r\n";
    return NB_Config(E_AT_CGATT_QUE, cmd, strlen((char*)cmd), UNIT_SECOND);
}

//#ifdef HAVE_ML5515
/*! \brief
*       使能EPS网络注册
* \param mode[IN]
*
* \return
*       E_NB_OK           - 成功
*       E_NB_PROCESSING   - 处理中
*       E_NB_ERR_OTHERS   - 错误
*/
static u8 NB_RegisterNBIoTPlatform(void)
{
    const unsigned char cmd[] = "AT+MLWSREGIND=0\r\n";
    return NB_Config(E_AT_MLWSREGIND, cmd, strlen((char*)cmd), UNIT_SECOND);
}
//#endif

//#ifdef HAVE_BC95
///*! \brief
//*       使能EPS网络注册
//* \param mode[IN]
//*
//* \return
//*       E_NB_OK           - 成功
//*       E_NB_PROCESSING   - 处理中
//*       E_NB_ERR_OTHERS   - 错误
//*/
//static u8 NB_EnabledESP(void)
//{
//    const unsigned char cmd[] = "AT+CEREG=1\r\n";
////    return NB_Config(E_AT_CEREG, cmd, strlen((char*)cmd), UNIT_SECOND);
//    return NB_Config(E_AT_CEREG, cmd, strlen((char*)cmd));
//}
//#endif


///*! \brief
//*       查询使能EPS网络注册
//* \param mode[IN]
//*
//* \return
//*       E_NB_OK           - 成功
//*       E_NB_PROCESSING   - 处理中
//*       E_NB_ERR_OTHERS   - 错误
//*/
//static u8 NB_QueESP(void)
//{
//    const unsigned char cmd[] = "AT+CEREG?\r\n";
////    return NB_Config(E_AT_CEREG_QUE, cmd, strlen((char*)cmd), UNIT_SECOND);
//    return NB_Config(E_AT_CEREG_QUE, cmd, strlen((char*)cmd));
//}

/*! \brief
*       关闭数据通知
* \param mode[IN]
*
* \return
*       E_NB_OK           - 成功
*       E_NB_PROCESSING   - 处理中
*       E_NB_ERR_OTHERS   - 错误
*/
static u8 NB_DisableDataNotification(void)
{
    const unsigned char cmd[] = "AT+NNMI=0\r\n";
    return NB_Config(E_AT_NNMI, cmd, strlen((char*)cmd), UNIT_SECOND/2);
}


/*! \brief
*      该命令返回最古老的缓冲消息并从缓冲区中删除它们。如果没有
* 消息将不会给出命令响应。如果新的消息指示(AT+NNMI)被打开
* 打开，则接收到的消息将无法通过此命令使用。
* \param mode[IN]
*
* \return
*       E_NB_OK           - 成功
*       E_NB_PROCESSING   - 处理中
*       E_NB_ERR_OTHERS   - 错误
*/
u8 NB_ReadNBIoTPlatformCommand(void)
{
    const unsigned char cmd[] = "AT+NMGR\r\n";
     return NB_Config(E_AT_NMGR, cmd, strlen((char*)cmd), 2*UNIT_SECOND);
}

/*! \brief
*       查询信号值
* \param mode[IN]
*
* \return
*       E_NB_OK           - 成功
*       E_NB_PROCESSING   - 处理中
*       E_NB_ERR_OTHERS   - 错误
*/
static u8 NB_Rssi(void)
{
    const unsigned char cmd[] = "AT+CSQ\r\n";
    return NB_Config(E_AT_CSQ, cmd, strlen((char*)cmd), UNIT_SECOND/2);
}

/*! \brief
*       查询系统时间
* \param mode[IN]
*
* \return
*       E_NB_OK           - 成功
*       E_NB_PROCESSING   - 处理中
*       E_NB_ERR_OTHERS   - 错误
*/
static u8 NB_GetCCLK(void)
{
    const unsigned char cmd[] = "AT+CCLK?\r\n";
    return NB_Config(E_AT_CCLK, cmd, strlen((char*)cmd), UNIT_SECOND/2);
}

///*! \brief
//*       查询NB信号状态
//* \param mode[IN]
//*
//* \return
//*       E_NB_OK           - 成功
//*       E_NB_PROCESSING   - 处理中
//*       E_NB_ERR_OTHERS   - 错误
//*/
//static u8 NB_RssiState(void)
//{
//    const unsigned char cmd[] = "AT+NUESTATS\r\n";
////    return NB_Config(E_AT_NUESTATS, cmd, strlen((char*)cmd), UNIT_SECOND);
//    return NB_Config(E_AT_NUESTATS, cmd, strlen((char*)cmd));
//}

////重设状态
//static void BSP_NB_ResetStatus(void)
//{
//    sg_nb_handler.sts = 0;
//    sg_nb_handler.cmd = 0;
//}

/*###################################################################################################*/
/*! \brief
*  加载模块配置
* \param handler[IN]        - NB config handler
*/
void BSP_NB_LoadConfig(NBConfig *handler)
{
    assert_param(handler->pwr_le==E_LE_HIGH || handler->pwr_le==E_LE_LOW);
    sg_nb_cfg_handler = handler;

    // 设定初值，当AWU周期为50ms时，重发周期为1s
    //sg_nb_handler.max_repeat_send_timer = 20;
}

/*! \brief
*       给模块上电
*/
void BSP_NB_PowerOn(void)
{
    if (E_LE_HIGH == sg_nb_cfg_handler->pwr_le)
    {
        BSP_GPIO_Init(sg_nb_cfg_handler->pwr.port, sg_nb_cfg_handler->pwr.pin, GPIO_Mode_Out_PP_High_Fast);
        BSP_GPIO_SetHigh(sg_nb_cfg_handler->pwr.port, sg_nb_cfg_handler->pwr.pin);
    }
    else
    {
        //printf("NB PWR On, port:%d, pin:%d\r\n", sg_nb_cfg_handler->pwr.port, sg_nb_cfg_handler->pwr.pin);
        BSP_GPIO_Init(sg_nb_cfg_handler->pwr.port, sg_nb_cfg_handler->pwr.pin, GPIO_Mode_Out_PP_Low_Fast);
        BSP_GPIO_SetLow(sg_nb_cfg_handler->pwr.port, sg_nb_cfg_handler->pwr.pin);
    }

//    //给reset脚高电平（低电平会发生重启）
//    if (sg_nb_cfg_handler->reset.port != 0)
//    {
//        BSP_GPIO_Init(sg_nb_cfg_handler->reset.port, sg_nb_cfg_handler->reset.pin, GPIO_Mode_Out_PP_High_Fast);//GPIO_Mode_Out_OD_Low_Slow
//        BSP_GPIO_SetLow(sg_nb_cfg_handler->reset.port, sg_nb_cfg_handler->reset.pin);
//    }
    //BSP_GPIO_SetLow(sg_nb_cfg_handler->reset.port, sg_nb_cfg_handler->reset.pin);
}

/*! \brief
*       给模块断电
*/
void BSP_NB_PowerOff(void)
{
    //printf("close power!\r\n");
    if (E_LE_HIGH == sg_nb_cfg_handler->pwr_le)
    {
        BSP_GPIO_Init(sg_nb_cfg_handler->pwr.port, sg_nb_cfg_handler->pwr.pin, GPIO_Mode_Out_OD_Low_Slow);
        BSP_GPIO_SetLow(sg_nb_cfg_handler->pwr.port, sg_nb_cfg_handler->pwr.pin);
    }
    else
    {
        //printf("NB PWR Off, port:%d, pin:%d\r\n", sg_nb_cfg_handler->pwr.port, sg_nb_cfg_handler->pwr.pin);
        BSP_GPIO_Init(sg_nb_cfg_handler->pwr.port, sg_nb_cfg_handler->pwr.pin, GPIO_Mode_Out_OD_Low_Slow);
        BSP_GPIO_SetHigh(sg_nb_cfg_handler->pwr.port, sg_nb_cfg_handler->pwr.pin);
    }
}

/*! \brief
* 模块配置初始化
* \param baudrate[IN]       - 模块串口波特率
* \param wordlength[IN]     - 模块串口数据位
* \param stopbits[IN]       - 模块串口停止位
* \param parity[IN]         - 模块串口校验位
*
* \return
*       E_NB_OK     - 成功
*       Others      - 失败
*/
u8 BSP_NB_Init(u32 baudrate, u8 wordlength, u8 stopbits, u8 parity)
{
    u8 rv = 0;

    rv = BSP_COM_Init(sg_nb_cfg_handler->com, baudrate, wordlength, stopbits, parity, USART_Mode_All, ENABLE, 0);
    if (rv != 0)
    {
        return rv;
    }

    sg_nb_handler.cmd = 0;
    sg_nb_handler.repeat_send_timer = 0;

    return rv;
}

/*! \brief
*       释放模块,单片机关闭串口
*/
void BSP_NB_Close(void)
{
    BSP_COM_Close(sg_nb_cfg_handler->com);
}

/*! \brief
*   读服务器命令
* \return
*       E_NB_OK               - 成功
*       E_NB_PROCESSING       - 处理中
*       E_NB_ERR_NO_SIM       - 无SIM卡
*       E_NB_ERR_CONFIG_PDP   - 配置PDP参数失败
*       E_NB_ERR_ACTIVE_PDP   - 激活PDP参数失败
*       E_NB_ERR_ATTACHED_NETWORK - 附着网络失败
*/
u8 BSP_NB_ReadData(u8 *cmd, u16* cmd_len)
{
    u8 rv = 0;

    rv = NB_ReadNBIoTPlatformCommand();
    if (rv == E_NB_OK)
    {
        memcpy(cmd, sg_nb_handler.cmd_dat, sg_nb_handler.cmd_size);
        *cmd_len = sg_nb_handler.cmd_size;
        //printf("CMD_SIZE:%d\r\n", sg_nb_handler.cmd_size);
        //printf("*cmd_len:%d\r\n", *cmd_len);
        return E_NB_OK;
    }

    return rv;
}

/*! \brief
* 检查NB模块是否已准备好联网,并且读出设备参数
*
* \param re_power_on_flag[IN]    - 重新上电标志
* \param band[IN]         - 频段
* \param apn[IN]          - APN
* \param ip[IN]           - IP
* \param ip_port[IN]      - IP端口
* \param imei[OUT]        - 模块IMEI号
* \param card_id[OUT]     - SIM卡的ICCID
* \return
*       E_NB_OK               - 成功
*       E_NB_PROCESSING       - 处理中
*       E_NB_ERR_NO_SIM       - 无SIM卡
*       E_NB_ERR_CONFIG_PDP   - 配置PDP参数失败
*       E_NB_ERR_ACTIVE_PDP   - 激活PDP参数失败
*       E_NB_ERR_ATTACHED_NETWORK - 附着网络失败
*/
//u8 BSP_NB_CheckIsReady(char* apn, char* ip, u16* ip_port, char* imei, char* iccid, char* firmware)
u8 BSP_NB_CheckIsReady(u8 re_power_on_flag, const char* band, char* apn, char* ip, u16* ip_port, char* imei)
{
    u8 rv = -1;


    if (sg_nb_handler.cmd == 0)
    {
        sg_nb_handler.cmd = E_ATE0;
    }

    // 为接下来的网络搜索做准备
    sg_nb_handler.net_sts = 0;

    switch(sg_nb_handler.cmd)
    {
//    case E_AT_NRB:       //关闭回显    ATE0
//        NB_SoftReboot();
//        sg_nb_handler.sts = E_IS_OK;
//        sg_nb_handler.cmd = E_ATE0;
////        if (rv != E_NB_OK)
////        {
////            return E_NB_ERR_MODULE_RESPONSE_ERROR;
////        }
//        break;
    case E_ATE0:       //关闭回显    ATE0
        rv = NB_EchoOff();
        if (rv != E_NB_OK)
        {
            return E_NB_ERR_MODULE_RESPONSE_ERROR;
        }
        sg_nb_handler.cmd = E_AT_CMEE;
        rv = E_NB_PROCESSING;
        break;
    case E_AT_CMEE:     //开启错误提示    AT+CMEE=1
        rv = NB_EnableError();
        if (rv != E_NB_OK)
        {
            if (rv != E_NB_PROCESSING)
            {
                sg_nb_handler.sts = 0;
                return E_NB_ERR_MODULE_RESPONSE_ERROR;
            }
            else
            {
                return E_NB_PROCESSING;
            }
        }

        sg_nb_handler.cmd = E_AT_CGSN;
        rv = E_NB_PROCESSING;
//        break;
    case E_AT_CGSN:     //查询IMEI号    AT+CGSN=1
        rv = NB_RequestIMEI();
        if (rv != E_NB_OK)
        {
            if (rv != E_NB_PROCESSING)
            {
                sg_nb_handler.sts = 0;
                return E_NB_ERR_REQUESTIMEI;
            }
            else
            {
                return E_NB_PROCESSING;
            }
        }
        else
        {
            strncpy(imei, (char const*)sg_nb_handler.imei, strlen((char*)sg_nb_handler.imei));
            //ReadNumStr(p+strlen("+NCCID:"), char*)sg_nb_handler.iccid);
        }

        sg_nb_handler.cmd = E_AT_NCCID;
        rv = E_NB_PROCESSING;
//        break;
    case E_AT_NCCID:      //查询卡片ICCID号    AT+NCCID
        rv = NB_RequestICCID();
        if (rv != E_NB_OK)
        {
            if (rv != E_NB_PROCESSING)
            {
                sg_nb_handler.sts = 0;
                return E_NB_ERR_REQUESTICCID;
            }
            else
            {
                return E_NB_PROCESSING;
            }
        }
        else
        {
            //strncpy(iccid, (char const*)sg_nb_handler.iccid, strlen((char*)sg_nb_handler.iccid));
            sg_nb_handler.cmd = 0;
            sg_nb_handler.sts = 0;
        }
        sg_nb_handler.cmd = E_AT_CIMI;
        rv = E_NB_PROCESSING;
        break;
    case E_AT_CIMI:      //查询号码IMSI号
        rv = NB_RequestIMSI();
        if (rv != E_NB_OK)
        {
            if (rv != E_NB_PROCESSING)
            {
                sg_nb_handler.sts = 0;
                return E_NB_ERR_REQUESTIMSI;
            }
            else
            {
                return E_NB_PROCESSING;
            }
        }
        else
        {
            //strncpy(iccid, (char const*)sg_nb_handler.iccid, strlen((char*)sg_nb_handler.iccid));
            sg_nb_handler.cmd = 0;
            sg_nb_handler.sts = 0;
        }
        sg_nb_handler.cmd = E_AT_NCDP;
        rv = E_NB_PROCESSING;
        break;
    case E_AT_NCDP:     //配置IP端口    AT+NCDP=180.101.147.115,5683
        rv = NB_Ipconfig(ip, ip_port);
        if (rv != E_NB_OK)
        {
            if (rv != E_NB_PROCESSING)
            {
                sg_nb_handler.sts = 0;
                return E_NB_ERR_IPCONFIG;
            }
            else
            {
                return E_NB_PROCESSING;
            }
        }
        sg_nb_handler.cmd = E_AT_CGDCONT;
        rv = E_NB_PROCESSING;
        break;
    case E_AT_CGDCONT:  //配置APN    AT+CGDCONT=1,"IP","ctnb"
        rv = NB_Slect_APN(apn);
        if (rv != E_NB_OK)
        {
            if (rv != E_NB_PROCESSING)
            {
                sg_nb_handler.sts = 0;
                return E_NB_ERR_CONFIG_PDP;
            }
            else
            {
                return E_NB_PROCESSING;
            }
        }

        rv = E_NB_PROCESSING;

        // 如果是首次连接需要配置频段
        if (re_power_on_flag == 1)
        {
            sg_nb_handler.cmd = E_AT_CFUN;
            sg_nb_handler.cfun_sts = 0;
        }
        else
        {
            sg_nb_handler.cmd = E_AT_CGATT;
        }
        break;
    case E_AT_CFUN:  //关机    AT+CFUN=0
        rv = NB_SoftCtrl(sg_nb_handler.cfun_sts);
        if (rv != E_NB_OK)
        {
            if (rv != E_NB_PROCESSING)
            {
                sg_nb_handler.sts = 0;
                return E_NB_ERR_SOFTON;
            }
            else
            {
                return E_NB_PROCESSING;
            }
        }

        // 如果已开机，说明BAND已经设置完毕，跳转到手动附着网络
        if (sg_nb_handler.cfun_sts == 1)
        {
            sg_nb_handler.cmd = E_AT_CGATT;
            rv = E_NB_PROCESSING;
        }
        else
        {
            sg_nb_handler.cmd = E_AT_NBAND;
            rv = E_NB_PROCESSING;
        }
        break;
    case E_AT_NBAND:     //设置频段    AT+NBAND=5
        rv = NB_SetBand((char*)band);
        if (rv != E_NB_OK)
        {
            if (rv != E_NB_PROCESSING)
            {
                sg_nb_handler.sts = 0;
                return E_NB_ERR_SETBAND;
            }
            else
            {
                return E_NB_PROCESSING;
            }
        }

//        sg_nb_handler.cfun_sts = 1;
        sg_nb_handler.cmd = E_AT_CELLRESELECTION;
        rv = E_NB_PROCESSING;
        //break;
    case E_AT_CELLRESELECTION:          //设置小区重选功能
        rv = NB_SetCellReselection();
        if (rv != E_NB_OK)
        {
            if (rv != E_NB_PROCESSING)
            {
                sg_nb_handler.sts = 0;
                return E_NB_ERR_SETBAND;
            }
            else
            {
                return E_NB_PROCESSING;
            }
        }

//        sg_nb_handler.cfun_sts = 1;
        sg_nb_handler.cmd = E_AT_NCSEARFCN;
        rv = E_NB_PROCESSING;
        //break;
    case E_AT_NCSEARFCN:          // 清除先验频点
        rv = NB_SetNCSEARFCN();
        if (rv != E_NB_OK)
        {
            if (rv != E_NB_PROCESSING)
            {
                sg_nb_handler.sts = 0;
                return E_NB_ERR_SETBAND;
            }
            else
            {
                return E_NB_PROCESSING;
            }
        }

        sg_nb_handler.cfun_sts = 1;
        sg_nb_handler.cmd = E_AT_CFUN;
        rv = E_NB_PROCESSING;
        break;
//    case E_AT_NCONFIG:  //自动附着网络连接    AT+NCONFIG=AUTOCONNECT,TRUE
//        rv = NB_AutoConnect();
//        if (rv != E_NB_OK)
//        {
//            if (rv != E_NB_PROCESSING)
//            {
//                sg_nb_handler.sts = 0;
//                return E_NB_ERR_AUTOCONNECT;
//            }
//            else
//            {
//                return E_NB_PROCESSING;
//            }
//        }
//        sg_nb_handler.cmd = E_AT_CGSN;
//        rv = E_NB_PROCESSING;
//        break;
    case E_AT_CGATT:     //查询IMEI号    AT+CGSN=1
        rv = NB_ActivateNet();
        if (rv != E_NB_OK)
        {
            if (rv != E_NB_PROCESSING)
            {
                return E_NB_ERR_CGATT;
            }
            else
            {
                return E_NB_PROCESSING;
            }
        }
        sg_nb_handler.cmd = E_AT_NNMI;
        rv = E_NB_PROCESSING;
        break;
    case E_AT_NNMI:             //开启数据通知
        rv = NB_DisableDataNotification();
        if (rv != E_NB_OK)
        {
            if (rv != E_NB_PROCESSING)
            {
//                sg_nb_handler.net_sts = 0;
                return E_NB_ERR_NNMI;
            }
            else
            {
                return E_NB_PROCESSING;
            }
        }
        else
        {
            sg_nb_handler.cmd = 0;
//            sg_nb_handler.net_sts = 0;
        }
        sg_nb_handler.cmd = E_AT_NPSMR;
        rv = E_NB_PROCESSING;
        break;
    case E_AT_NPSMR:
        rv = NB_SetNPSMR();
        if (rv != E_NB_OK)
        {
            if (rv != E_NB_PROCESSING)
            {
//                sg_nb_handler.net_sts = 0;
                return E_NB_ERR_NNMI;
            }
            else
            {
                return E_NB_PROCESSING;
            }
        }
        else
        {
            sg_nb_handler.cmd = 0;
//            sg_nb_handler.net_sts = 0;
        }
        break;
//    case E_AT_CSQ:
//        rv = NB_Rssi();
//        if (rv != E_NB_OK)
//        {
//            return E_NB_ERR_SIGNAL;
//        }
//        break;
//    case E_AT_FIRMWARE:     //NB 固件版本号
//        rv = NB_RequestFirmware();
//        if (rv != E_NB_OK)
//        {
//            if (rv != E_NB_PROCESSING)
//            {
//                sg_nb_handler.sts = 0;
//                return E_NB_ERR_REQUESTIMEI;
//            }
//            else
//            {
//                return E_NB_PROCESSING;
//            }
//        }
//        else
//        {
//            strncpy(firmware, (char const*)sg_nb_handler.firmware, strlen((char*)sg_nb_handler.firmware));
//        }
//        break;
    default:
        break;
    }

    return rv;
}

/*! \brief
* 入网
* \param rssi[OUT]           - 信号值
* \return
*       E_NB_OK               - 成功
*       E_NB_PROCESSING       - 处理中
*       E_NB_ERR_NO_SIM       - 无SIM卡
*       E_NB_ERR_CONFIG_PDP   - 配置PDP参数失败
*       E_NB_ERR_ACTIVE_PDP   - 激活PDP参数失败
*       E_NB_ERR_ATTACHED_NETWORK - 附着网络失败
*/
u8 BSP_NB_CheckNet(u8 *rssi)
{
    u8 rv = 0;

    switch(sg_nb_handler.net_sts)
    {
//    case 0:
//        rv = NB_ActivateNet();
//        if (rv != E_NB_OK)
//        {
//            if (rv != E_NB_PROCESSING)
//            {
//                sg_nb_handler.net_sts = 0;
//                return E_NB_ERR_CGATT;
//            }
//            else
//            {
//                return E_NB_PROCESSING;
//            }
//        }
//        else
//        {
//            sg_nb_handler.net_sts = 1;
//            return E_NB_PROCESSING;
//        }
//        break;
    case 0:
        rv = NB_QueActivateNet();
        if (rv != E_NB_OK)
        {
            if (rv != E_NB_PROCESSING)
            {
                sg_nb_handler.net_sts = 0;
                return E_NB_ERR_CGATT_QUE;
            }
            else
            {
                return E_NB_PROCESSING;
            }
        }
        else
        {
#ifdef HAVE_CU
            sg_nb_handler.net_sts = 1;
            return E_NB_PROCESSING;
#endif
#ifdef HAVE_CT
            sg_nb_handler.net_sts = 2;
            return E_NB_PROCESSING;
//            return E_NB_OK;
#endif
        }
        break;
    case 1:
        rv = NB_RegisterNBIoTPlatform();
        if (rv != E_NB_OK)
        {
            if (rv != E_NB_PROCESSING)
            {
                return E_NB_ERR_CEREG;
            }
            else
            {
                return E_NB_PROCESSING;
            }
        }
        else
        {
            sg_nb_handler.net_sts = 2;
            return E_NB_PROCESSING;
        }
        break;
    case 2:
        rv = NB_Rssi();
        if (rv != E_NB_OK)
        {
            return E_NB_ERR_SIGNAL;
        }
        else
        {
            sg_nb_handler.net_sts = 2;
            *rssi = sg_nb_handler.rssi;
            return E_NB_OK;
//			return E_NB_PROCESSING;
        }
		break;
//    case 3:
//        rv = NB_QueESP();
//        if (rv != E_NB_OK)
//        {
//            if (rv != E_NB_PROCESSING)
//            {
//                sg_nb_handler.net_sts = 0;
//                return E_NB_ERR_CEREG_QUE;
//            }
//            else
//            {
//                return E_NB_PROCESSING;
//            }
//        }
//        else
//        {
//            sg_nb_handler.net_sts = 0;
//        }
//    case 4:
//        rv = NB_DataMessage();
//        if (rv != E_NB_OK)
//        {
//            if (rv != E_NB_PROCESSING)
//            {
//                sg_nb_handler.net_sts = 0;
//                return E_NB_ERR_NNMI;
//            }
//            else
//            {
//                return E_NB_PROCESSING;
//            }
//        }
//        else
//        {
//            sg_nb_handler.net_sts = 0;
//        }
        break;
    default:
        break;
    }

    return rv;
}

/*! \brief
*       读取信号值
* \param rssi[OUT]           - 信号值
* \param time[OUT]           - 系统时间
* \return
*       E_NB_OK           - 成功
*       E_NB_PROCESSING   - 处理中
*       E_NB_ERR_OTHERS   - 错误
*/
u8 BSP_NB_GetRSSIAndTime(u8 *rssi, u8* time)
{
    u8 rv = 0;

    switch(sg_nb_handler.rssi_sts)
    {
    case 0:
        rv = NB_Rssi();
        if (rv != E_NB_OK)
        {
            return E_NB_ERR_SIGNAL;
        }
        else
        {
            sg_nb_handler.rssi_sts = 1;
            *rssi = sg_nb_handler.rssi;
        }
    case 1:
        rv = NB_GetCCLK();
        if (rv != E_NB_OK)
        {
            return E_NB_ERR_TIME;
        }
        else
        {
            sg_nb_handler.rssi_sts = 0;
            memcpy(time, sg_nb_handler.time, sizeof(sg_nb_handler.time));
//            printf("Time:%02X%02X-%02X-%02X %02X:%02X:%02X\r\n",
//                   sg_nb_handler.time[0],
//                   sg_nb_handler.time[1],
//                   sg_nb_handler.time[2],
//                   sg_nb_handler.time[3],
//                   sg_nb_handler.time[4],
//                   sg_nb_handler.time[5],
//                   sg_nb_handler.time[6]
//                   );
        }
        break;
//    case 1:
//        rv = NB_RssiState();
//        if (rv != E_NB_OK)
//        {
//            return E_NB_ERR_NUESTATS;
//        }
//        else
//        {
//            sg_nb_handler.rssi_sts = 0;
//
//            state->signal_power = sg_nb_handler.signal_power;
//            state->tx_power     = sg_nb_handler.tx_power;
//            strncpy((char*)state->cell_id, (char const*)sg_nb_handler.cell_id, strlen((char const*)&sg_nb_handler.cell_id));
//            state->ecl         = sg_nb_handler.ecl;
//            state->snr         = sg_nb_handler.snr;
//            state->pci         = sg_nb_handler.pci;
//            state->rsrq        = sg_nb_handler.rsrq;
//        }
//        break;
    default:
        break;
    }

    return rv;

}

/*! \brief
*       发送数据到服务器<每次发送最多为100个字节>
* \param dat[IN]           - 数据
* \param len[IN]           - 数据长度
* \param mode[IN]          - 数据发送模式
* \return
*       E_NB_OK           - 成功
*       E_NB_PROCESSING   - 处理中
*       E_NB_ERR_OTHERS   - 错误
*/
u8 BSP_NB_IPSendData(u8* dat, u16 len, u16 mode)
{
    u8  rv = 0;
    //u8 send_data_str[100+1] = {0};
    u8 cmd[128] = {0};

    //bytes_2_ascii((char*)send_data_str, (u8)sizeof(send_data_str), dat, len);
    //printf("send_data_str=%s\r\n",send_data_str);

    switch(mode)
    {
    case 0:
//        sprintf((char*)cmd, "AT+NMGS=%u,%s\r\n", len, send_data_str);
//        rv = NB_Config(E_AT_SEND_NO_ACK, cmd, strlen((char*)cmd), UNIT_SECOND);//无需回复
        sprintf((char*)cmd, "AT+NMGS=%u,", len);
        bytes_2_ascii((char*)cmd+strlen((char*)cmd), (u8)sizeof(cmd), dat, len);
        memcpy(cmd+strlen((char*)cmd), "\r\n", strlen("\r\n"));

        rv = NB_Config(E_AT_SEND_NO_ACK, cmd, strlen((char*)cmd), UNIT_SECOND);
        break;
    case 0x0001:
#ifdef HAVE_BC95
        sprintf((char*)cmd, "AT+QLWULDATAEX=%u,", len);
#endif
#ifdef HAVE_ML5515
        sprintf((char*)cmd, "AT+MLWULDATAEX=%u,", len);
#endif

        bytes_2_ascii((char*)cmd+strlen((char*)cmd), (u8)sizeof(cmd), dat, len);
        memcpy(cmd+strlen((char*)cmd), ",0x0001\r\n", strlen(",0x0001\r\n"));

//        rv = NB_Config(E_AT_SEND_NO_ACK, cmd, strlen((char*)cmd), UNIT_SECOND);//无需回复,此处需要优化，无需增加超时时间，由应用层管理
        rv = NB_Config(E_AT_SEND_NO_ACK, cmd, strlen((char*)cmd), UNIT_SECOND);
        break;
    case 0x0101:
#ifdef HAVE_BC95
        sprintf((char*)cmd, "AT+QLWULDATAEX=%u,", len);
#endif
#ifdef HAVE_ML5515
        sprintf((char*)cmd, "AT+MLWULDATAEX=%u,", len);
#endif

        bytes_2_ascii((char*)cmd+strlen((char*)cmd), (u8)sizeof(cmd), dat, len);
        memcpy(cmd+strlen((char*)cmd), ",0x0101\r\n", strlen(",0x0101\r\n"));

//        rv = NB_Config(E_AT_SEND_ACK, cmd, strlen((char*)cmd), 5*UNIT_SECOND);//成功后会收到QLWULDATASTATUS:4表示成功发送
        rv = NB_Config(E_AT_SEND_ACK, cmd, strlen((char*)cmd), UNIT_SECOND);
        break;
    default:
#ifdef HAVE_BC95
        sprintf((char*)cmd, "AT+QLWULDATAEX=%u,", len);
#endif
#ifdef HAVE_ML5515
        sprintf((char*)cmd, "AT+MLWULDATAEX=%u,", len);
#endif

        bytes_2_ascii((char*)cmd+strlen((char*)cmd), (u8)sizeof(cmd), dat, len);
        memcpy(cmd+strlen((char*)cmd), ",0x0001\r\n", strlen(",0x0001\r\n"));

        rv = NB_Config(E_AT_SEND_NO_ACK, cmd, strlen((char*)cmd), UNIT_SECOND);
        break;
    }
    return rv;
}


/*! \brief
*       接收从服务器下发的数据
* \param msg[OUT]           - 消息指针
* \param size[OUT][IN]      - 数据长度
*
* \return
*       E_NB_OK           - 成功
*       E_NB_ERR_NO_DATA  - 暂无数据
*       E_NB_ERR_OTHERS   - 错误
* \notes
*     msg为接收数据的数组，size为输入输出参数，输入接收数组的大小，输出接收数据的大小。
*/
u8 BSP_NB_IPRecvData(u8* msg, u16 *size)
{
    u8 rv = 0;
    char *recv_ptr = 0;
    u16 recv_len = 0;
    int pkg_data_len = 0;
    char *data_ptr = NULL;

    sg_nb_handler.cmd = E_AT_IPRECV;
    rv = NB_RecvData((u8**)&recv_ptr, &recv_len);
    if (0 == rv)
    {
        //printf("[1]recv:%s\r\n", recv_ptr);
        //+NNMI:4,01001D0F
        data_ptr = strstr((char*)recv_ptr, "+NNMI:");
        if (data_ptr != NULL)
        {
            // 判断是否收到了\r\n结束符
            if (NB_CheckRespIsRecvComplete(data_ptr+2) == 0)
            {
                data_ptr = strstr((char*)recv_ptr, "+NNMI:");
                //printf("[2]recv:%s\r\n", data_ptr);

                //接收数据的大小
                pkg_data_len = ReadNum(data_ptr+strlen("+NNMI:"));
                //printf("len:%d\r\n", pkg_data_len);

                //接收的数据
                data_ptr = strstr((char*)recv_ptr, ",");

                // 跳过分隔符
                data_ptr++;

                data_ptr[pkg_data_len*2] = '\0';
                ascii_2_bytes(msg, *size, (const char*)data_ptr, pkg_data_len*2);//字符串转字节数据
                //MYLOG_DEBUG_HEXDUMP("msg:", msg, strlen((char*)data_str)/2);

                //赋值给参数
                *size = pkg_data_len;

                BSP_NB_RecvInit();
                return E_NB_OK;
            }
        }
        //BSP_NB_RecvInit();
    }

    return E_NB_ERR_NO_DATA;
}

/*! \brief
*       NB模块数据接收初始化
*/
void BSP_NB_RecvInit(void)
{
    BSP_COM_RecvInit(sg_nb_cfg_handler->com);
}

/*! \brief
*       发送最大超时
*/
void BSP_NB_SetRepeatSendCounter(u16 counter)
{
    sg_nb_handler.max_repeat_send_timer = counter;
}



#endif
