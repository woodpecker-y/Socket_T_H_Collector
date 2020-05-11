#include "adf.h"
#include "sysparams.h"
#include "protocol.h"
#include "log_data.h"

///*! \brief
//*       �������ַ�������Ϊʮ��������,���ֲ��ܳ���0xFF FF FF FF
//* \param msg[IN]             - ��A6ģ��������ݵ�ָ��
//*
//* \return
//*       E_GPRS_OK       - �ɹ�
//*       Others          - ʧ��
//*/
//u32 protocol_set_atoul(char* msg)
//{
//    int i = 0;
//    u32 num = 0;
//
//    while(msg[i]>='0' && msg[i]<='9')
//    {
//        num = num*10 + msg[i] - '0';
//        i++;
//    }
//
//    return num;
//}
//
///*! \brief
//*       ��ָ�������ַ���ת��������
//*/
//u16 protocol_set_atoul_len(u8* msg,u8 len)
//{
//    int i = 0;
//    u16 num = 0;
//    for(i=0; i<len; i++)
//    {
//        num = num*10 + *(msg+i) - '0';
//    }
//    return num;
//}

//u8  protocol_format_netaddr(char *ip)
//{
//    int i = 0, j = 0, k = 0, m = 0;
//    int len = strlen(ip);
//
//    j = len - 1;
//
//    //ȥ����β�ո�(ȡ����i-1��j+1֮����ַ�):
//    while (!(ip[i]>='0' && ip[i]<='9' || ip[i]=='.' || (ip[i]>='a' && ip[i]<='z') || (ip[i]>='A' && ip[i]<='Z')))
//    {
//        i++;
//    }
//
//    while (!(ip[j]>='0' && ip[j]<='9' || ip[j]=='.' || (ip[j]>='a' && ip[j]<='z') || (ip[j]>='A' && ip[j]<='Z')))
//    {
//        j--;
//    }
//
//    m = 0;
//    for (k=i-1; k<j+1; ++k)
//    {
//        ip[m] = ip[k];
//    }
//
//    ip[m] = '\0';
//
//    return 0;
//}

///*! \brief
//*       ��ʮ��������ת��Ϊ�ַ�����ʽ
//*/
//void protocol_set_date( char* date,SysParams *sys_params)
//{
//    sprintf(date,"%02x%02x%02x%02xT%02x%02x%02xZ",
//            sys_params->t[0],
//            sys_params->t[1],
//            sys_params->t[2],
//            sys_params->t[3],
//            sys_params->t[4],
//            sys_params->t[5],
//            sys_params->t[6]);
//
//    return;
//}

///*! \brief
//*       �����ȿ�ʼ��ֹ��ʮ��������ת��Ϊ�ַ�����ʽ
//*/
//void protocol_set_heatdate(char* heatstart,char* heatstop,SysParams *sys_params)
//{
//     sprintf(heatstart,"%04d",sys_params->hp_st);
//     sprintf(heatstop,"%04d",sys_params->hp_et);
//
//     return;
//}
//
///*! \brief
//*       ��ʮ���Ƶ��豸��ַת��Ϊ�ַ�����ʽ
//*/
//void protocol_set_addr(char* addr,SysParams *sys_params)
//{
//     sprintf(addr,"%s",sys_params->addr);
//
//     return;
//}

////���
//int protocol_report_request(u8* dat, u16 *len, SysParams *sys_params, RunParams *run_params)
//{
//    u8 offset = 0;
//    dat[offset] = E_CMD_TYPE_WIRELESS_REPORT;
//    offset += 1;
//    //���书��
//    dec_2_hex_type(&dat[offset], 2, run_params->nbiot_module_state.tx_power, ORD_MOTOR);
//    offset += 2;
//    //��վID
//    dec_2_hex_type(&dat[offset], 4, protocol_set_atoul((char*)run_params->nbiot_module_state.cell_id), ORD_MOTOR);
//    offset += 4;
//    //�ź�ǿ��
//    dec_2_hex_type(&dat[offset], 2, run_params->nbiot_module_state.total_power, ORD_MOTOR);
//    offset += 2;
//    //�����
//    dec_2_hex_type(&dat[offset], 1, run_params->nbiot_module_state.snr, ORD_MOTOR);
//    offset += 1;
//    //�����źŵ�����
//    dec_2_hex_type(&dat[offset], 2, run_params->nbiot_module_state.signal_power, ORD_MOTOR);
//    offset += 2;
//    //�����źŵ�����
//    dec_2_hex_type(&dat[offset], 2, run_params->nbiot_module_state.rsrq, ORD_MOTOR);
//    offset += 2;
//    //NBIoTģ���ۼƹ���ʱ��
//    dec_2_hex_type(&dat[offset], 4, sys_params->nbiot_module_activetime, ORD_MOTOR);
//    offset += 4;
//    //NBIoT�ۼ�ͨѶ����
//    dec_2_hex_type(&dat[offset], 4, sys_params->nbiot_module_numofconnections, ORD_MOTOR);
//    offset += 4;
//
//    *len = offset;
//
//    return 0;
//
//}

//�¶�ʪ�����
int protocol_report_request(u8* dat, u16 *len, u8 *addr, LogData *log)
{
    u8 offset = 0;

    dat[offset] = E_CMD_TYPE_REPORT;            //�ϱ�
    offset += 1;
    //�豸ID��ַ
    memcpy(&dat[offset], addr, strlen((char*)addr));
    offset +=15;
    //�¶�
    dec_2_hex_type(&dat[offset], 2, log->temp, ORD_MOTOR);
    offset += 2;
    //ʪ��
    dec_2_hex_type(&dat[offset], 2, log->humi, ORD_MOTOR);
    offset += 2;
    // �ź�ǿ��
    dat[offset] = log->rssi;
    offset += 1;
    //��ص���
    dat[offset] = 0x64;
    offset += 1;
    //״̬
    dat[offset] = log->st;
    offset += 1;
    //�ɼ�ʱ��
    bytes_2_ascii((char*)dat+offset, 9, log->sample_time, 4);
    dat[offset+8] = 'T';
    bytes_2_ascii((char*)dat+offset+9, 7, log->sample_time+4, 3);
    dat[offset+15] = 'Z';
    offset += 16;

    *len = offset;

    return 0;
}

////�·��趨��ů�� ���
//int protocol_update_heat_period(u16 *hp_st, u16 *hp_et, u8 *dat, u16 *len)
//{
//    u8  offset = 0;
//    u16 mid = 0;
//    u8  tmp[2] = {0};
//
//    //������Ĵ���
//    if ( dat[0] != E_CMD_TYPE_SET_HEATPERIOD_REQUEST )
//    {
//        return -1;
//    }
//    offset += 1;
//
//    // ��Ϣ�Ự��ȡ
//    mid = hex_2_dec_type(&dat[offset], 2, ORD_MOTOR);
//    offset += 2;
//    // ��ů�ڿ�ʼʱ��
//    ascii_2_bytes(tmp, 2, (char*)dat+offset, 4);
//    *hp_st = bcd_2_dec_type(tmp, 2, ORD_MOTOR);
//    //sys_params->hp_st = protocol_set_atoul_len(&dat[offset], 4);
//    offset += 4;
//    // ��ů�ڽ���ʱ��
//    ascii_2_bytes(tmp, 2, (char*)dat+offset, 4);
//    *hp_et = bcd_2_dec_type(tmp, 2, ORD_MOTOR);
//    //sys_params->hp_et = protocol_set_atoul_len(&dat[offset], 4);
//    offset += 4;
//
//    //===�������===
//    // ������
//    offset = 0;
//    dat[offset] = E_CMD_TYPE_SET_HEATPERIOD_RESPONSE;
//    offset += 1;
//    // ��Ϣ�Ự
//    dec_2_hex_type(&dat[offset], 2, mid, ORD_MOTOR);
//    offset += 2;
//    //��Ӧ��
//    dat[offset] = 0;
//    offset += 1;
//
//    *len = offset;
//
//    return 0;
//}

//�趨��ʪ�Ȳ���ֵ ���
int protocol_fix_t_h(s8 *fix_temp, s8 *fix_humi, s8 *dat, u16 *len)
{
    u8 offset = 0;
    s8 value = 0;

    // ������Ĵ���
    if ( dat[0] != E_CMD_TYPE_SET_FIXTH )
    {
        return -1;
    }
    offset++;

    //�ϱ�����
    value = dat[offset++];
    if (value<=100 && value>=-100)
    {
        *fix_temp = value;//��λΪ0.1��
    }

    value = dat[offset++];
    if (value<=100 && value>=-100)
    {
        *fix_humi = value;//��λΪ0.1%
    }

//    //===�������===
//    // ������
//    offset = 0;
//    dat[offset] = E_CMD_TYPE_SET_FIXTH_RESPONSE;
//    offset += 1;
//    // ��Ϣ�Ự
//    dec_2_hex_type(&dat[offset], 2, mid, ORD_MOTOR);
//    offset += 2;
//    //��Ӧ��
//    dat[offset] = 0;
//    offset += 1;
//
//    *len = offset;

    return 0;
}

//�趨�ϱ����� ���
int protocol_update_upload_cycle(u16 *upload_cycle, u8 *dat, u16 *len)
{
    u8 offset = 0;
    u8 value = 0;

    // ������Ĵ���
    if ( dat[0] != E_CMD_TYPE_SET_REPORTCYCLE )
    {
        return -1;
    }
    offset++;

    //�ϱ�����
    value = hex_2_dec_type(&dat[offset], 1, ORD_MOTOR);
    if (value != 0 && value < 100)
    {
        *upload_cycle = value * 60;//�·������ڲ��� ��λΪhour ���صĲ�����λΪ����
    }

//    //===�������===
//    // ������
//    offset = 0;
//    dat[offset] = E_CMD_TYPE_SET_REPORTCYCLE_RESPONSE;
//    offset += 1;
//    // ��Ϣ�Ự
//    dec_2_hex_type(&dat[offset], 2, mid, ORD_MOTOR);
//    offset += 2;
//    //��Ӧ��
//    dat[offset] = 0;
//    offset += 1;
//
//    *len = offset;

    return 0;
}



////�趨�ɼ����� ���
//int protocol_update_sample_cycle(u16 *sample_cycle, u8 *dat, u16 *len)
//{
//    u16 value = 0;
//    u16 mid = 0;
//    u8  offset = 0;
//    u8  ret = 0;
//
//    // ������Ĵ���
//    if ( dat[0] != E_CMD_TYPE_SET_SAMPLE_CYCLE_REQUEST )
//    {
//        return -1;
//    }
//    offset += 1;
//    //��Ϣ�Ự��ȡ
//    mid = hex_2_dec_type(&dat[offset], 2, ORD_MOTOR);
//    offset += 2;
//    //�ϱ�����
//    value = hex_2_dec_type(&dat[offset], 2, ORD_MOTOR);
//    if (value != 0)
//    {
//        *sample_cycle = value;
//    }
//    else
//    {
//        ret = 1;
//    }
//    offset += 2;
//
//    //===�������===
//    //������
//    offset = 0;
//    dat[offset] = E_CMD_TYPE_SET_SAMPLE_CYCLE_RESPONSE;
//    offset += 1;
//    //�ỰID
//    dec_2_hex_type(&dat[offset], 2, mid, ORD_MOTOR);
//    offset += 2;
//    //״̬��
//    dat[offset] = ret;
//    offset += 1;
//
//    *len = offset;
//
//    return 0;
//}

////�趨������
//int protocol_update_server_config(char *ip, u16 *port, char *apn, u8 *dat, u16 *len)
//{
//    u8 offset = 0;
//    u16 mid = 0;
//    u8  ip_len = 0;
//    u8  apn_len = 0;
//
//    // ������Ĵ���
//    if ( dat[0] != E_CMD_TYPE_SET_SERVER_CONFIG_REQUEST )
//    {
//        return -1;
//    }
//    offset += 1;
//    //��Ϣ�Ự��ȡ
//    mid = hex_2_dec_type(&dat[offset], 2, ORD_MOTOR);
//    offset += 2;
//    // IP��ַ����
//    ip_len = dat[offset];
//    offset++;
//    // IP��ַ����
//    memcpy(ip, dat+offset, ip_len);
//    //printf("[1]ip:%s\r\n", ip);
////    protocol_format_netaddr(ip);
////    printf("[2]ip:%s\r\n", ip);
//    offset += ip_len;
//    // �˿ں�
//    *port = hex_2_dec_type(&dat[offset], 2, ORD_MOTOR);
//    offset += 2;
//    //printf("port:%u\r\n", *port);
//    // APN����
//    apn_len = dat[offset];
//    offset++;
//    memcpy(apn, dat+offset, apn_len);
////    protocol_format_netaddr(apn);
//    offset += apn_len;
//    //printf("apn:%s\r\n", apn);
//
//    //===�������===
//    //������
//    offset = 0;
//    dat[offset] = E_CMD_TYPE_SET_SERVER_CONFIG_RESPONSE;
//    offset += 1;
//    //�ỰID
//    dec_2_hex_type(&dat[offset], 2, mid, ORD_MOTOR);
//    offset += 2;
//    //״̬��
//    dat[offset] = 0;
//    offset += 1;
//
//    *len = offset;
//
//    return 0;
//}

//// �豸����
//int protocol_remote_reboot(u8 *dat, u16 *len)
//{
//    u8 offset = 0;
//    u16 mid = 0;
//
//    // ������Ĵ���
//    if ( dat[0] != E_CMD_TYPE_REMOTE_REBOOT_REQUEST )
//    {
//        return -1;
//    }
//    offset += 1;
//    //��Ϣ�Ự��ȡ
//    mid = hex_2_dec_type(&dat[offset], 2, ORD_MOTOR);
//    offset += 2;
//
//    //===�������===
//    //������
//    offset = 0;
//    dat[offset] = E_CMD_TYPE_REMOTE_REBOOT_RESPONSE;
//    offset += 1;
//    //�ỰID
//    dec_2_hex_type(&dat[offset], 2, mid, ORD_MOTOR);
//    offset += 2;
//    //״̬��
//    dat[offset] = 0;
//    offset += 1;
//
//    *len = offset;
//
//    return 0;
//}

////��ȡ������Ϣ
//int protocol_get_config(u8 *dat, u16 *len, u16 hp_st, u16 hp_et, u16 upload_cycle, u16 sample_cycle)
//{
//    u8  offset = 0;
//    u8  tmp[2] = {0};
//    u16 mid = 0;
//
//    //������Ĵ���
//    if ( dat[0] != E_CMD_TYPE_GET_SYSCONFIG_REQUEST )
//    {
//        return -1;
//    }
//    offset += 1;
//    //��Ϣ�Ự��ȡ
//    mid = hex_2_dec_type(&dat[offset], 2, ORD_MOTOR);
//    offset += 2;
//
//     //===�������===
//    //������
//    offset = 0;
//    dat[offset] = E_CMD_TYPE_GET_SYSCONFIG_RESPONSE;
//    offset += 1;
//    //�ỰID
//    dec_2_hex_type(&dat[offset], 2, mid, ORD_MOTOR);
//    offset += 2;
//    //״̬��
//    dat[offset] = 0;
//    offset += 1;
//
//    // ��ů�ڿ�ʼ����
//    dec_2_bcd_type(tmp, 2, hp_st, ORD_MOTOR);
//    bytes_2_ascii((char*)dat+offset, 5, tmp, 2);
//    offset += 4;
//    // ��ů�ڽ�������
//    dec_2_bcd_type(tmp, 2, hp_et, ORD_MOTOR);
//    bytes_2_ascii((char*)dat+offset, 5, tmp, 2);
//    offset += 4;
//    // �ϱ�����
//    dec_2_hex_type(&dat[offset], 2, upload_cycle, ORD_MOTOR);
//    offset += 2;
//    // �ɼ�����
//    dec_2_hex_type(&dat[offset], 2, sample_cycle, ORD_MOTOR);
//    offset += 2;
//
//    *len = offset;
//
//    return 0;
//}

////�趨ϵͳʱ�䣬���
//int port_set_time_proc(SysParams *sys_params, u8 *dat, u16 *len)
//{
//    u8 offset = 0;
//    u16 mid = 0;
//
//    //������Ĵ���
//    if ( dat[0] != E_CMD_TYPE_SET_SYSTIME_REQUEST )
//    {
//        return -1;
//    }
//    offset += 1;
//    //��Ϣ�Ự��ȡ
//    mid = hex_2_dec_type(&dat[offset], 2, ORD_MOTOR);
//    offset += 2;
////    //ϵͳʱ��,�꣬�£��գ�ʱ���֣���
////    sys_params->t[0] = protocol_set_atoul_len(&dat[offset], 4)/100;
////    offset += 4;
////    sys_params->t.tm_mon = protocol_set_atoul_len(&dat[offset], 2) - 1;
////    offset += 2;
////    sys_params->t.tm_mday = protocol_set_atoul_len(&dat[offset], 2) ;
////    offset += 3;
////    sys_params->t.tm_hour = protocol_set_atoul_len(&dat[offset], 2) ;
////    offset += 2;
////    sys_params->t.tm_min = protocol_set_atoul_len(&dat[offset], 2) ;
////    offset += 2;
////    sys_params->t.tm_sec = protocol_set_atoul_len(&dat[offset], 2) ;
////    offset += 3;
//
//    //===�������===
//    //������
//    offset = 0;
//    dat[offset] = E_CMD_TYPE_SET_SYSTIME_RESPONSE;
//    offset += 1;
//    //�ỰID
//    dec_2_hex_type(&dat[offset], 2, mid, ORD_MOTOR);
//    offset += 2;
//    //״̬��
//    dat[offset] = 0;
//    offset += 1;
//
//    *len = offset;
//
//    return 0;
//}

////��ȡ�豸��Ϣ
//int port_get_hardware_proc(SysParams *sys_params, RunParams *run_params, u8 *dat, u16 *len)
//{
//    u8 offset = 0;
//    u16 mid = 0;
//
//    //������Ĵ���
//    if ( dat[0] != E_CMD_TYPE_GET_HARDWARE_REQUEST )
//    {
//        return -1;
//    }
//    offset += 1;
//    //��Ϣ�Ự��ȡ
//    mid = hex_2_dec_type(dat+offset, 2, ORD_MOTOR);
//    offset += 2;
//
//
//    //===�������===
//    //������
//    offset = 0;
//    dat[offset] = E_CMD_TYPE_GET_HARDWARE_RESPONSE;
//    offset += 1;
//    //�ỰID
//    dec_2_hex_type(&dat[offset], 2, mid, ORD_MOTOR);
//    offset += 2;
//    //״̬��
//    dat[offset] = 0;
//    offset += 1;
//    //�̼��汾
//    memcpy(&dat[offset], run_params->nbiot_module_state.firmware, sizeof(run_params->nbiot_module_state.firmware));
//    offset += 8;
//    //Ӳ���汾
//    memcpy(&dat[offset], sys_params->hw_ver, sizeof(sys_params->hw_ver));
//    offset += 8;
//    //����汾
//    memcpy(&dat[offset], sys_params->sw_ver, sizeof(sys_params->sw_ver));
//    offset += 8;
//    //ģ��Ψһ��IMEI
//    memcpy(&dat[offset], run_params->nbiot_module_state.imei, sizeof(run_params->nbiot_module_state.imei));
//    offset += 15;
//    //SIM��Ψһ��
//    memcpy(&dat[offset], run_params->nbiot_module_state.iccid, sizeof(run_params->nbiot_module_state.iccid));
//    offset += 20;
//    //APN
//    memcpy(&dat[offset], sys_params->apn, sizeof(sys_params->apn));
//    offset += 16;
//    //������IP
//    memcpy(&dat[offset], sys_params->ip, sizeof(sys_params->ip));
//    offset += 15;
//    //�������˿�
//    dec_2_hex_type(&dat[offset], 2, sys_params->ip_port, ORD_MOTOR);
//    offset += 2;
//
//    *len = offset;
//
//    return 0;
//}


