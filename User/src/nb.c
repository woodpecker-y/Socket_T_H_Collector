#ifdef HAVE_NB
//#define HAVE_NB_LOG

#include <stdlib.h>
#include <stdio.h>
#include "bsp.h"
#include "nb.h"
#include "sysparams.h"
#include "utili.h"

// ģ�����þ��
static NBConfig *sg_nb_cfg_handler = NULL;

// ģ��������
static NBHandler sg_nb_handler;

/*! \brief
*       �����ַ����е�һ������������ �ַ������ܶ�ȡ16��������A--F
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
*       �������ַ�������Ϊʮ��������,���ֲ��ܳ��� 0XFF FF FF FF
* \param msg[IN]             - ��A6ģ��������ݵ�ָ��
*
* \return
*       E_GPRS_OK       - �ɹ�
*       Others          - ʧ��
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
// ʱ���ʽΪ:18/11/6 17:39:00 //����NBʱ��
int NB_TimerParser(u8 *time, char *time_str)
{
    char *p = NULL;
    time_t t1 = 0;
    struct tm t2;

    u16 year = 0, month = 0, day = 0, hour = 0, min = 0, second = 0;

    //printf("time:%s\r\n", time_str);

    // ��
    year = ReadNum(time_str) + 2000;
    //printf("year:%u\r\n", year);

    // ��
    p = strstr((char*)time_str, "/");
    if (p == NULL)
    {
        return -1;
    }
    month = ReadNum(p+strlen("/"));

    // ��
    p = strstr((char*)p+strlen("/"), "/");
    if (p == NULL)
    {
        return -1;
    }
    day = ReadNum(p+strlen("/"));

    // ʱ
    p = strstr((char*)p+strlen("/"), ",");
    if (p == NULL)
    {
        return -1;
    }
    hour = ReadNum(p+strlen(","));

    // ��
    p = strstr((char*)p+strlen("/"), ":");
    if (p == NULL)
    {
        return -1;
    }
    min = ReadNum(p+strlen(":"));

    // ��
    p = strstr((char*)p+strlen("/"), ":");
    if (p == NULL)
    {
        return -1;
    }
    second = ReadNum(p+strlen(":"));

    // ��ʱ���޸�Ϊ������
    t1 = mktime_user(year, month, day, hour, min, second);
    t1 += 8*3600;

    localtime_user(t1, &t2);

    // ת��ΪBCD��ʱ���ʽ
    dec_2_bcd_type(time+0, 2, t2.tm_year+1900, ORD_MOTOR);
    dec_2_bcd_type(time+2, 1, t2.tm_mon+1,  ORD_MOTOR);
    dec_2_bcd_type(time+3, 1, t2.tm_mday, ORD_MOTOR);
    dec_2_bcd_type(time+4, 1, t2.tm_hour, ORD_MOTOR);
    dec_2_bcd_type(time+5, 1, t2.tm_min,  ORD_MOTOR);
    dec_2_bcd_type(time+6, 1, t2.tm_sec,  ORD_MOTOR);

    return 0;
}

/*! \brief
*       �������ݸ�ģ�飨����ATָ��ʹ�ã�
* \param dat[IN]            - ���͸�ģ�������ָ��
* \param len[IN]            - ���͸�ģ������ݳ���
*
* \return
*       E_NB_OK     - �ɹ�
*       Others          - ʧ��
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
*       ��ģ��������ݣ�����ģ�鷢�͵�ָ�����
* \param dat[OUT]            - ��ģ��������ݵ�ָ��
* \param len[OUT]            - ��ģ��������ݵĳ���
*
* \return
*       E_NB_OK     - �ɹ�
*       Others          - ʧ��
* \notes
*       ����datΪ����ָ�룬����ҪΪ�������ڴ滺�壬��ֱ��ָ�򴮿��ڲ������������ݣ�
*   ֱ��ʹ�ü��ɣ��ڵ���NB_recv_init()��NB_send_data()����֮ǰ����һֱ���ڡ�
*/
static u8 NB_RecvData(u8 **dat, u16 *len)
{
    return BSP_COM_RecvDataStream(sg_nb_cfg_handler->com, dat, len);
}

/*! \brief
*       ���ģ��ķ����Ƿ�ΪOK
* \param msg[IN]                - ��ģ����յ�����
*
* \return
*       E_NB_OK             - �ɹ�
*       E_NB_ERR_OTHERS       - ʧ��
*/
static u8 NB_CheckRespIsOK(char *msg)
{
    //printf("msg:%s", msg);
    if (strstr(msg, "ERROR") != NULL)      //ֻҪ���صı�������ERROR����˵���д���
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
*       // �ж��Ƿ��յ���\r\n������
* \param msg[IN]                - ��ģ����յ�����
*
* \return
*       E_NB_OK             - �ɹ�
*       E_NB_ERR_OTHERS       - ʧ��
*/
static u8 NB_CheckRespIsRecvComplete(char *msg)
{
    //printf("msg:%s", msg);
    if (strstr(msg, "ERROR") != NULL)      //ֻҪ���صı�������ERROR����˵���д���
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
*       NB����AT�����״̬��
* \param cmd[IN]             - ����������
* \param dat[IN]             - AT�����
* \param len[IN]             - AT����ĳ���
* \param delay_ack_timer[IN] - �ȴ�AT����ʱ��
* \return
*       E_NB_OK           - �ɹ�
*       E_NB_PROCESSING   - ������
*       E_NB_ERR_OTHERS   - ����
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
    case E_IS_OK://�ԱȻظ����Ƿ���   ��OK��
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
                    //printf("+QLWLDATASTATUS:4 OK\r\n");//BC95-B5(��Զ)+QLWULDATASTATUS :4  /  ML5515(�뿡)+MLWULDATASTATUS:4

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
                    //����״̬ʹ�ý��յ������ݣ��ʲ������������
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
//            else if (E_NB_ERR_MODULE_RESPONSE_ERROR == rv) // ģ�鷴������
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

        // ���ճ�ʱ�ط���������������ʱ�ȴ����ͼ�����
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
        // ���ճ�ʱ�ط���������������ʱ�ȴ����ͼ�����
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
                // ����\r\n
                sg_nb_handler.cmd_size = ReadNum((char*)rcv_ptr+2);

                //printf("cmd_size:%d\r\n", sg_nb_handler.cmd_size);

                //���յ�����
//                p = strstr((char*)rcv_ptr, ",");
//                if (p!= NULL)
//                {
                    // �����ָ���
                    p++;
//                    if (pkg_data_len*2 < rcv_len)
//                    {
                        //p[pkg_data_len*2] = '\0';

                        ascii_2_bytes(sg_nb_handler.cmd_dat, sg_nb_handler.cmd_size, (const char*)p, sg_nb_handler.cmd_size*2);//�ַ���ת�ֽ�����

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
*       �ر�ATָ�����
* \return
*       E_NB_OK           - �ɹ�
*       E_NB_PROCESSING   - ������
*       E_NB_ERR_OTHERS   - ����
*/
static u8 NB_EchoOff(void)
{
    const unsigned char cmd[] = "ATE0\r\n";
    return NB_Config(E_ATE0, cmd, strlen((char*)cmd), UNIT_SECOND/2);
}

/*! \brief
*       �������ģ��
* \return
*       E_NB_OK           - �ɹ�
*       E_NB_PROCESSING   - ������
*       E_NB_ERR_OTHERS   - ����
*/
u8 NB_SoftReboot(void)
{
    const unsigned char cmd[] = "AT+NRB\r\n";
    return NB_Config(E_AT_NRB, cmd, strlen((char*)cmd), UNIT_SECOND);
}

/*! \brief
*       ʹ�ܴ�����ʾ
* \return
*       E_NB_OK           - �ɹ�
*       E_NB_PROCESSING   - ������
*       E_NB_ERR_OTHERS   - ����
*/
static u8 NB_EnableError(void)
{
    const unsigned char cmd[] = "AT+CMEE=1\r\n";
    return NB_Config(E_AT_CMEE, cmd, strlen((char*)cmd), UNIT_SECOND/2);
}

///*! \brief
//*       �ػ� (NB ��С����)
//* \return
//*       E_NB_OK           - �ɹ�
//*       E_NB_PROCESSING   - ������
//*       E_NB_ERR_OTHERS   - ����
//*/
//static u8 NB_SoftOff(void)
//{
//    const unsigned char cmd[] = "AT+CFUN=0\r\n";
//    return NB_Config(E_AT_CFUN_OFF, cmd, strlen((char*)cmd), UNIT_SECOND);
//}

/*! \brief
*       ����IP���˿�
* \param ip[IN]             - ip��ַ
* \param ip_port[IN]        - IP���Ӷ˿�
* \return
*       E_NB_OK           - �ɹ�
*       E_NB_PROCESSING   - ������
*       E_NB_ERR_OTHERS   - ����
*/
static u8 NB_Ipconfig(char* ip, u16* ip_port)
{
    unsigned char cmd[100] = {0};
    sprintf((char*)cmd, "AT+NCDP=%s,%u\r\n", ip, *ip_port);
    return NB_Config(E_AT_NCDP, cmd, strlen((char*)cmd), UNIT_SECOND/2);
}

/*! \brief
*       ����APN
* \param apn[IN]        - apn
* \return
*       E_NB_OK           - �ɹ�
*       E_NB_PROCESSING   - ������
*       E_NB_ERR_OTHERS   - ����
*/
static u8 NB_Slect_APN(char* apn)
{
    unsigned char cmd[64] = {0};
    sprintf((char*)cmd, "AT+CGDCONT=1,\"IP\",\"%s\"\r\n", apn);
    return NB_Config(E_AT_CGDCONT, cmd, strlen((char*)cmd), UNIT_SECOND/2);
}

/*! \brief
*       ���ػ� (NB ȫ����)
* \return
*       E_NB_OK           - �ɹ�
*       E_NB_PROCESSING   - ������
*       E_NB_ERR_OTHERS   - ����
*/
static u8 NB_SoftCtrl(u8 sts)
{
    unsigned char cmd[16] = {0};

    sprintf((char*)cmd, "AT+CFUN=%d\r\n", (s16)sts);

    return NB_Config(E_AT_CFUN, cmd, strlen((char*)cmd), UNIT_SECOND/2);
}

///*! \brief
//*       �Զ�����
//* \return
//*       E_NB_OK           - �ɹ�
//*       E_NB_PROCESSING   - ������
//*       E_NB_ERR_OTHERS   - ����
//*/
//static u8 NB_AutoConnect(void)
//{
//    const unsigned char cmd[] = "AT+NCONFIG=AUTOCONNECT,TRUE\r\n";
////    return NB_Config(E_AT_NCONFIG, cmd, strlen((char*)cmd), UNIT_SECOND);
//    return NB_Config(E_AT_NCONFIG, cmd, strlen((char*)cmd));
//}

/*! \brief
*       ����Ƶ��
* \return
*       E_NB_OK           - �ɹ�
*       E_NB_PROCESSING   - ������
*       E_NB_ERR_OTHERS   - ����
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
*       ����С����ѡ
* \return
*       E_NB_OK           - �ɹ�
*       E_NB_PROCESSING   - ������
*       E_NB_ERR_OTHERS   - ����
*/
static u8 NB_SetCellReselection(void)
{
    unsigned char cmd[36] = {0};

    //sprintf((char*)cmd, "AT+NCONFIG=AUTOCONNECT,TRUE\r\n");
    sprintf((char*)cmd, "AT+NCONFIG=CELL_RESELECTION,TRUE\r\n");
    return NB_Config(E_AT_CELLRESELECTION, cmd, strlen((char*)cmd), UNIT_SECOND/2);
}

/*! \brief
*       �������Ƶ��
* \return
*       E_NB_OK           - �ɹ�
*       E_NB_PROCESSING   - ������
*       E_NB_ERR_OTHERS   - ����
*/
static u8 NB_SetNCSEARFCN(void)
{
    const unsigned char cmd[36] = {0};
    sprintf((char*)cmd, "AT+NCSEARFCN\r\n");
    return NB_Config(E_AT_NCSEARFCN, cmd, strlen((char*)cmd), UNIT_SECOND/2);
}

/*! \brief
*       ����PSM״̬
* \return
*       E_NB_OK           - �ɹ�
*       E_NB_PROCESSING   - ������
*       E_NB_ERR_OTHERS   - ����
*/
static u8 NB_SetNPSMR(void)
{
    unsigned char cmd[24] = {0};
    sprintf((char*)cmd, "AT+NPSMR=1\r\n");
    return NB_Config(E_AT_NPSMR, cmd, strlen((char*)cmd), UNIT_SECOND/2);
}

/*! \brief
*       ��ȡPSMģʽ״̬
*
* \return
*       E_NB_OK           - �ɹ�
*       E_NB_ERR_RECVING  - ��������
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
*       ��ѯIMEI�� �˺�Ϊģ���IMEI�ţ�ΪӲ����Ψһ����
* \return
*       E_NB_OK           - �ɹ�
*       E_NB_PROCESSING   - ������
*       E_NB_ERR_OTHERS   - ����
*/
static u8 NB_RequestIMEI(void)
{
    const unsigned char cmd[] = "AT+CGSN=1\r\n";
    return NB_Config(E_AT_CGSN, cmd, strlen((char*)cmd), UNIT_SECOND/2);
}

/*! \brief
*       ��ѯICCID�� �˺�Ϊ����ID�ţ���Ӳ������Ψһ����
* \param mode[IN]
*
* \return
*       E_NB_OK           - �ɹ�
*       E_NB_PROCESSING   - ������
*       E_NB_ERR_OTHERS   - ����
*/
static u8 NB_RequestICCID(void)
{
    const unsigned char cmd[] = "AT+NCCID\r\n";
    return NB_Config(E_AT_NCCID, cmd, strlen((char*)cmd), UNIT_SECOND/2);
}

/*! \brief
*       ��ѯIMSI�� �˺�Ϊ�����ֻ��ţ����������
* \param mode[IN]
*
* \return
*       E_NB_OK           - �ɹ�
*       E_NB_PROCESSING   - ������
*       E_NB_ERR_OTHERS   - ����
*/
static u8 NB_RequestIMSI(void)
{
    const unsigned char cmd[] = "AT+CIMI\r\n";
    return NB_Config(E_AT_CIMI, cmd, strlen((char*)cmd), UNIT_SECOND/2);
}

///*! \brief
//*       ��ѯ�̼��汾��
//* \param mode[IN]
//*
//* \return
//*       E_NB_OK           - �ɹ�
//*       E_NB_PROCESSING   - ������
//*       E_NB_ERR_OTHERS   - ����
//*/
//static u8 NB_RequestFirmware(void)
//{
//    const unsigned char cmd[] = "AT+CGMR\r\n";
////    return NB_Config(E_AT_FIRMWARE, cmd, strlen((char*)cmd), UNIT_SECOND);
//    return NB_Config(E_AT_FIRMWARE, cmd, strlen((char*)cmd));
//}

/*! \brief
*       ��������
* \param mode[IN]
*
* \return
*       E_NB_OK           - �ɹ�
*       E_NB_PROCESSING   - ������
*       E_NB_ERR_OTHERS   - ����
*/
static u8 NB_ActivateNet(void)
{
    const unsigned char cmd[] = "AT+CGATT=1\r\n";
    return NB_Config(E_AT_CGATT, cmd, strlen((char*)cmd), UNIT_SECOND);
}

/*! \brief
*       ��ѯ�Ƿ񼤻�����
* \param mode[IN]
*
* \return
*       E_NB_OK           - �ɹ�
*       E_NB_PROCESSING   - ������
*       E_NB_ERR_OTHERS   - ����
*/
static u8 NB_QueActivateNet(void)
{
    const unsigned char cmd[] = "AT+CGATT?\r\n";
    return NB_Config(E_AT_CGATT_QUE, cmd, strlen((char*)cmd), UNIT_SECOND);
}

//#ifdef HAVE_ML5515
/*! \brief
*       ʹ��EPS����ע��
* \param mode[IN]
*
* \return
*       E_NB_OK           - �ɹ�
*       E_NB_PROCESSING   - ������
*       E_NB_ERR_OTHERS   - ����
*/
static u8 NB_RegisterNBIoTPlatform(void)
{
    const unsigned char cmd[] = "AT+MLWSREGIND=0\r\n";
    return NB_Config(E_AT_MLWSREGIND, cmd, strlen((char*)cmd), UNIT_SECOND);
}
//#endif

//#ifdef HAVE_BC95
///*! \brief
//*       ʹ��EPS����ע��
//* \param mode[IN]
//*
//* \return
//*       E_NB_OK           - �ɹ�
//*       E_NB_PROCESSING   - ������
//*       E_NB_ERR_OTHERS   - ����
//*/
//static u8 NB_EnabledESP(void)
//{
//    const unsigned char cmd[] = "AT+CEREG=1\r\n";
////    return NB_Config(E_AT_CEREG, cmd, strlen((char*)cmd), UNIT_SECOND);
//    return NB_Config(E_AT_CEREG, cmd, strlen((char*)cmd));
//}
//#endif


///*! \brief
//*       ��ѯʹ��EPS����ע��
//* \param mode[IN]
//*
//* \return
//*       E_NB_OK           - �ɹ�
//*       E_NB_PROCESSING   - ������
//*       E_NB_ERR_OTHERS   - ����
//*/
//static u8 NB_QueESP(void)
//{
//    const unsigned char cmd[] = "AT+CEREG?\r\n";
////    return NB_Config(E_AT_CEREG_QUE, cmd, strlen((char*)cmd), UNIT_SECOND);
//    return NB_Config(E_AT_CEREG_QUE, cmd, strlen((char*)cmd));
//}

/*! \brief
*       �ر�����֪ͨ
* \param mode[IN]
*
* \return
*       E_NB_OK           - �ɹ�
*       E_NB_PROCESSING   - ������
*       E_NB_ERR_OTHERS   - ����
*/
static u8 NB_DisableDataNotification(void)
{
    const unsigned char cmd[] = "AT+NNMI=0\r\n";
    return NB_Config(E_AT_NNMI, cmd, strlen((char*)cmd), UNIT_SECOND/2);
}


/*! \brief
*      �����������ϵĻ�����Ϣ���ӻ�������ɾ�����ǡ����û��
* ��Ϣ���������������Ӧ������µ���Ϣָʾ(AT+NNMI)����
* �򿪣�����յ�����Ϣ���޷�ͨ��������ʹ�á�
* \param mode[IN]
*
* \return
*       E_NB_OK           - �ɹ�
*       E_NB_PROCESSING   - ������
*       E_NB_ERR_OTHERS   - ����
*/
u8 NB_ReadNBIoTPlatformCommand(void)
{
    const unsigned char cmd[] = "AT+NMGR\r\n";
     return NB_Config(E_AT_NMGR, cmd, strlen((char*)cmd), 2*UNIT_SECOND);
}

/*! \brief
*       ��ѯ�ź�ֵ
* \param mode[IN]
*
* \return
*       E_NB_OK           - �ɹ�
*       E_NB_PROCESSING   - ������
*       E_NB_ERR_OTHERS   - ����
*/
static u8 NB_Rssi(void)
{
    const unsigned char cmd[] = "AT+CSQ\r\n";
    return NB_Config(E_AT_CSQ, cmd, strlen((char*)cmd), UNIT_SECOND/2);
}

/*! \brief
*       ��ѯϵͳʱ��
* \param mode[IN]
*
* \return
*       E_NB_OK           - �ɹ�
*       E_NB_PROCESSING   - ������
*       E_NB_ERR_OTHERS   - ����
*/
static u8 NB_GetCCLK(void)
{
    const unsigned char cmd[] = "AT+CCLK?\r\n";
    return NB_Config(E_AT_CCLK, cmd, strlen((char*)cmd), UNIT_SECOND/2);
}

///*! \brief
//*       ��ѯNB�ź�״̬
//* \param mode[IN]
//*
//* \return
//*       E_NB_OK           - �ɹ�
//*       E_NB_PROCESSING   - ������
//*       E_NB_ERR_OTHERS   - ����
//*/
//static u8 NB_RssiState(void)
//{
//    const unsigned char cmd[] = "AT+NUESTATS\r\n";
////    return NB_Config(E_AT_NUESTATS, cmd, strlen((char*)cmd), UNIT_SECOND);
//    return NB_Config(E_AT_NUESTATS, cmd, strlen((char*)cmd));
//}

////����״̬
//static void BSP_NB_ResetStatus(void)
//{
//    sg_nb_handler.sts = 0;
//    sg_nb_handler.cmd = 0;
//}

/*###################################################################################################*/
/*! \brief
*  ����ģ������
* \param handler[IN]        - NB config handler
*/
void BSP_NB_LoadConfig(NBConfig *handler)
{
    assert_param(handler->pwr_le==E_LE_HIGH || handler->pwr_le==E_LE_LOW);
    sg_nb_cfg_handler = handler;

    // �趨��ֵ����AWU����Ϊ50msʱ���ط�����Ϊ1s
    //sg_nb_handler.max_repeat_send_timer = 20;
}

/*! \brief
*       ��ģ���ϵ�
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

//    //��reset�Ÿߵ�ƽ���͵�ƽ�ᷢ��������
//    if (sg_nb_cfg_handler->reset.port != 0)
//    {
//        BSP_GPIO_Init(sg_nb_cfg_handler->reset.port, sg_nb_cfg_handler->reset.pin, GPIO_Mode_Out_PP_High_Fast);//GPIO_Mode_Out_OD_Low_Slow
//        BSP_GPIO_SetLow(sg_nb_cfg_handler->reset.port, sg_nb_cfg_handler->reset.pin);
//    }
    //BSP_GPIO_SetLow(sg_nb_cfg_handler->reset.port, sg_nb_cfg_handler->reset.pin);
}

/*! \brief
*       ��ģ��ϵ�
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
* ģ�����ó�ʼ��
* \param baudrate[IN]       - ģ�鴮�ڲ�����
* \param wordlength[IN]     - ģ�鴮������λ
* \param stopbits[IN]       - ģ�鴮��ֹͣλ
* \param parity[IN]         - ģ�鴮��У��λ
*
* \return
*       E_NB_OK     - �ɹ�
*       Others      - ʧ��
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
*       �ͷ�ģ��,��Ƭ���رմ���
*/
void BSP_NB_Close(void)
{
    BSP_COM_Close(sg_nb_cfg_handler->com);
}

/*! \brief
*   ������������
* \return
*       E_NB_OK               - �ɹ�
*       E_NB_PROCESSING       - ������
*       E_NB_ERR_NO_SIM       - ��SIM��
*       E_NB_ERR_CONFIG_PDP   - ����PDP����ʧ��
*       E_NB_ERR_ACTIVE_PDP   - ����PDP����ʧ��
*       E_NB_ERR_ATTACHED_NETWORK - ��������ʧ��
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
* ���NBģ���Ƿ���׼��������,���Ҷ����豸����
*
* \param re_power_on_flag[IN]    - �����ϵ��־
* \param band[IN]         - Ƶ��
* \param apn[IN]          - APN
* \param ip[IN]           - IP
* \param ip_port[IN]      - IP�˿�
* \param imei[OUT]        - ģ��IMEI��
* \param card_id[OUT]     - SIM����ICCID
* \return
*       E_NB_OK               - �ɹ�
*       E_NB_PROCESSING       - ������
*       E_NB_ERR_NO_SIM       - ��SIM��
*       E_NB_ERR_CONFIG_PDP   - ����PDP����ʧ��
*       E_NB_ERR_ACTIVE_PDP   - ����PDP����ʧ��
*       E_NB_ERR_ATTACHED_NETWORK - ��������ʧ��
*/
//u8 BSP_NB_CheckIsReady(char* apn, char* ip, u16* ip_port, char* imei, char* iccid, char* firmware)
u8 BSP_NB_CheckIsReady(u8 re_power_on_flag, const char* band, char* apn, char* ip, u16* ip_port, char* imei)
{
    u8 rv = -1;


    if (sg_nb_handler.cmd == 0)
    {
        sg_nb_handler.cmd = E_ATE0;
    }

    // Ϊ������������������׼��
    sg_nb_handler.net_sts = 0;

    switch(sg_nb_handler.cmd)
    {
//    case E_AT_NRB:       //�رջ���    ATE0
//        NB_SoftReboot();
//        sg_nb_handler.sts = E_IS_OK;
//        sg_nb_handler.cmd = E_ATE0;
////        if (rv != E_NB_OK)
////        {
////            return E_NB_ERR_MODULE_RESPONSE_ERROR;
////        }
//        break;
    case E_ATE0:       //�رջ���    ATE0
        rv = NB_EchoOff();
        if (rv != E_NB_OK)
        {
            return E_NB_ERR_MODULE_RESPONSE_ERROR;
        }
        sg_nb_handler.cmd = E_AT_CMEE;
        rv = E_NB_PROCESSING;
        break;
    case E_AT_CMEE:     //����������ʾ    AT+CMEE=1
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
    case E_AT_CGSN:     //��ѯIMEI��    AT+CGSN=1
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
    case E_AT_NCCID:      //��ѯ��ƬICCID��    AT+NCCID
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
    case E_AT_CIMI:      //��ѯ����IMSI��
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
    case E_AT_NCDP:     //����IP�˿�    AT+NCDP=180.101.147.115,5683
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
    case E_AT_CGDCONT:  //����APN    AT+CGDCONT=1,"IP","ctnb"
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

        // ������״�������Ҫ����Ƶ��
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
    case E_AT_CFUN:  //�ػ�    AT+CFUN=0
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

        // ����ѿ�����˵��BAND�Ѿ�������ϣ���ת���ֶ���������
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
    case E_AT_NBAND:     //����Ƶ��    AT+NBAND=5
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
    case E_AT_CELLRESELECTION:          //����С����ѡ����
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
    case E_AT_NCSEARFCN:          // �������Ƶ��
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
//    case E_AT_NCONFIG:  //�Զ�������������    AT+NCONFIG=AUTOCONNECT,TRUE
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
    case E_AT_CGATT:     //��ѯIMEI��    AT+CGSN=1
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
    case E_AT_NNMI:             //��������֪ͨ
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
//    case E_AT_FIRMWARE:     //NB �̼��汾��
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
* ����
* \param rssi[OUT]           - �ź�ֵ
* \return
*       E_NB_OK               - �ɹ�
*       E_NB_PROCESSING       - ������
*       E_NB_ERR_NO_SIM       - ��SIM��
*       E_NB_ERR_CONFIG_PDP   - ����PDP����ʧ��
*       E_NB_ERR_ACTIVE_PDP   - ����PDP����ʧ��
*       E_NB_ERR_ATTACHED_NETWORK - ��������ʧ��
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
*       ��ȡ�ź�ֵ
* \param rssi[OUT]           - �ź�ֵ
* \param time[OUT]           - ϵͳʱ��
* \return
*       E_NB_OK           - �ɹ�
*       E_NB_PROCESSING   - ������
*       E_NB_ERR_OTHERS   - ����
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
*       �������ݵ�������<ÿ�η������Ϊ100���ֽ�>
* \param dat[IN]           - ����
* \param len[IN]           - ���ݳ���
* \param mode[IN]          - ���ݷ���ģʽ
* \return
*       E_NB_OK           - �ɹ�
*       E_NB_PROCESSING   - ������
*       E_NB_ERR_OTHERS   - ����
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
//        rv = NB_Config(E_AT_SEND_NO_ACK, cmd, strlen((char*)cmd), UNIT_SECOND);//����ظ�
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

//        rv = NB_Config(E_AT_SEND_NO_ACK, cmd, strlen((char*)cmd), UNIT_SECOND);//����ظ�,�˴���Ҫ�Ż����������ӳ�ʱʱ�䣬��Ӧ�ò����
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

//        rv = NB_Config(E_AT_SEND_ACK, cmd, strlen((char*)cmd), 5*UNIT_SECOND);//�ɹ�����յ�QLWULDATASTATUS:4��ʾ�ɹ�����
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
*       ���մӷ������·�������
* \param msg[OUT]           - ��Ϣָ��
* \param size[OUT][IN]      - ���ݳ���
*
* \return
*       E_NB_OK           - �ɹ�
*       E_NB_ERR_NO_DATA  - ��������
*       E_NB_ERR_OTHERS   - ����
* \notes
*     msgΪ�������ݵ����飬sizeΪ������������������������Ĵ�С������������ݵĴ�С��
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
            // �ж��Ƿ��յ���\r\n������
            if (NB_CheckRespIsRecvComplete(data_ptr+2) == 0)
            {
                data_ptr = strstr((char*)recv_ptr, "+NNMI:");
                //printf("[2]recv:%s\r\n", data_ptr);

                //�������ݵĴ�С
                pkg_data_len = ReadNum(data_ptr+strlen("+NNMI:"));
                //printf("len:%d\r\n", pkg_data_len);

                //���յ�����
                data_ptr = strstr((char*)recv_ptr, ",");

                // �����ָ���
                data_ptr++;

                data_ptr[pkg_data_len*2] = '\0';
                ascii_2_bytes(msg, *size, (const char*)data_ptr, pkg_data_len*2);//�ַ���ת�ֽ�����
                //MYLOG_DEBUG_HEXDUMP("msg:", msg, strlen((char*)data_str)/2);

                //��ֵ������
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
*       NBģ�����ݽ��ճ�ʼ��
*/
void BSP_NB_RecvInit(void)
{
    BSP_COM_RecvInit(sg_nb_cfg_handler->com);
}

/*! \brief
*       �������ʱ
*/
void BSP_NB_SetRepeatSendCounter(u16 counter)
{
    sg_nb_handler.max_repeat_send_timer = counter;
}



#endif
