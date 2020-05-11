#include "adf.h"
#include "sysparams.h"
#include "protocol.h"
#include "log_data.h"

///*! \brief
//*       见数字字符串解析为十进制数字,数字不能超过0xFF FF FF FF
//* \param msg[IN]             - 从A6模块接收数据的指针
//*
//* \return
//*       E_GPRS_OK       - 成功
//*       Others          - 失败
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
//*       将指定长度字符串转换成数字
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
//    //去除首尾空格(取出从i-1到j+1之间的字符):
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
//*       将十进制日期转换为字符串格式
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
//*       将供热开始截止的十进制日期转换为字符串格式
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
//*       将十进制的设备地址转换为字符串格式
//*/
//void protocol_set_addr(char* addr,SysParams *sys_params)
//{
//     sprintf(addr,"%s",sys_params->addr);
//
//     return;
//}

////组包
//int protocol_report_request(u8* dat, u16 *len, SysParams *sys_params, RunParams *run_params)
//{
//    u8 offset = 0;
//    dat[offset] = E_CMD_TYPE_WIRELESS_REPORT;
//    offset += 1;
//    //发射功率
//    dec_2_hex_type(&dat[offset], 2, run_params->nbiot_module_state.tx_power, ORD_MOTOR);
//    offset += 2;
//    //基站ID
//    dec_2_hex_type(&dat[offset], 4, protocol_set_atoul((char*)run_params->nbiot_module_state.cell_id), ORD_MOTOR);
//    offset += 4;
//    //信号强度
//    dec_2_hex_type(&dat[offset], 2, run_params->nbiot_module_state.total_power, ORD_MOTOR);
//    offset += 2;
//    //信噪比
//    dec_2_hex_type(&dat[offset], 1, run_params->nbiot_module_state.snr, ORD_MOTOR);
//    offset += 1;
//    //发射信号的质量
//    dec_2_hex_type(&dat[offset], 2, run_params->nbiot_module_state.signal_power, ORD_MOTOR);
//    offset += 2;
//    //接受信号的质量
//    dec_2_hex_type(&dat[offset], 2, run_params->nbiot_module_state.rsrq, ORD_MOTOR);
//    offset += 2;
//    //NBIoT模组累计工作时间
//    dec_2_hex_type(&dat[offset], 4, sys_params->nbiot_module_activetime, ORD_MOTOR);
//    offset += 4;
//    //NBIoT累计通讯次数
//    dec_2_hex_type(&dat[offset], 4, sys_params->nbiot_module_numofconnections, ORD_MOTOR);
//    offset += 4;
//
//    *len = offset;
//
//    return 0;
//
//}

//温度湿度组包
int protocol_report_request(u8* dat, u16 *len, u8 *addr, LogData *log)
{
    u8 offset = 0;

    dat[offset] = E_CMD_TYPE_REPORT;            //上报
    offset += 1;
    //设备ID地址
    memcpy(&dat[offset], addr, strlen((char*)addr));
    offset +=15;
    //温度
    dec_2_hex_type(&dat[offset], 2, log->temp, ORD_MOTOR);
    offset += 2;
    //湿度
    dec_2_hex_type(&dat[offset], 2, log->humi, ORD_MOTOR);
    offset += 2;
    // 信号强度
    dat[offset] = log->rssi;
    offset += 1;
    //电池电量
    dat[offset] = 0x64;
    offset += 1;
    //状态
    dat[offset] = log->st;
    offset += 1;
    //采集时间
    bytes_2_ascii((char*)dat+offset, 9, log->sample_time, 4);
    dat[offset+8] = 'T';
    bytes_2_ascii((char*)dat+offset+9, 7, log->sample_time+4, 3);
    dat[offset+15] = 'Z';
    offset += 16;

    *len = offset;

    return 0;
}

////下发设定供暖期 解包
//int protocol_update_heat_period(u16 *hp_st, u16 *hp_et, u8 *dat, u16 *len)
//{
//    u8  offset = 0;
//    u16 mid = 0;
//    u8  tmp[2] = {0};
//
//    //命令码的处理
//    if ( dat[0] != E_CMD_TYPE_SET_HEATPERIOD_REQUEST )
//    {
//        return -1;
//    }
//    offset += 1;
//
//    // 消息会话提取
//    mid = hex_2_dec_type(&dat[offset], 2, ORD_MOTOR);
//    offset += 2;
//    // 供暖期开始时间
//    ascii_2_bytes(tmp, 2, (char*)dat+offset, 4);
//    *hp_st = bcd_2_dec_type(tmp, 2, ORD_MOTOR);
//    //sys_params->hp_st = protocol_set_atoul_len(&dat[offset], 4);
//    offset += 4;
//    // 供暖期结束时间
//    ascii_2_bytes(tmp, 2, (char*)dat+offset, 4);
//    *hp_et = bcd_2_dec_type(tmp, 2, ORD_MOTOR);
//    //sys_params->hp_et = protocol_set_atoul_len(&dat[offset], 4);
//    offset += 4;
//
//    //===反馈组包===
//    // 命令码
//    offset = 0;
//    dat[offset] = E_CMD_TYPE_SET_HEATPERIOD_RESPONSE;
//    offset += 1;
//    // 消息会话
//    dec_2_hex_type(&dat[offset], 2, mid, ORD_MOTOR);
//    offset += 2;
//    //响应码
//    dat[offset] = 0;
//    offset += 1;
//
//    *len = offset;
//
//    return 0;
//}

//设定温湿度补偿值 解包
int protocol_fix_t_h(s8 *fix_temp, s8 *fix_humi, s8 *dat, u16 *len)
{
    u8 offset = 0;
    s8 value = 0;

    // 命令码的处理
    if ( dat[0] != E_CMD_TYPE_SET_FIXTH )
    {
        return -1;
    }
    offset++;

    //上报周期
    value = dat[offset++];
    if (value<=100 && value>=-100)
    {
        *fix_temp = value;//单位为0.1℃
    }

    value = dat[offset++];
    if (value<=100 && value>=-100)
    {
        *fix_humi = value;//单位为0.1%
    }

//    //===反馈组包===
//    // 命令码
//    offset = 0;
//    dat[offset] = E_CMD_TYPE_SET_FIXTH_RESPONSE;
//    offset += 1;
//    // 消息会话
//    dec_2_hex_type(&dat[offset], 2, mid, ORD_MOTOR);
//    offset += 2;
//    //响应码
//    dat[offset] = 0;
//    offset += 1;
//
//    *len = offset;

    return 0;
}

//设定上报周期 解包
int protocol_update_upload_cycle(u16 *upload_cycle, u8 *dat, u16 *len)
{
    u8 offset = 0;
    u8 value = 0;

    // 命令码的处理
    if ( dat[0] != E_CMD_TYPE_SET_REPORTCYCLE )
    {
        return -1;
    }
    offset++;

    //上报周期
    value = hex_2_dec_type(&dat[offset], 1, ORD_MOTOR);
    if (value != 0 && value < 100)
    {
        *upload_cycle = value * 60;//下发的周期参数 单位为hour 本地的参数单位为分钟
    }

//    //===反馈组包===
//    // 命令码
//    offset = 0;
//    dat[offset] = E_CMD_TYPE_SET_REPORTCYCLE_RESPONSE;
//    offset += 1;
//    // 消息会话
//    dec_2_hex_type(&dat[offset], 2, mid, ORD_MOTOR);
//    offset += 2;
//    //响应码
//    dat[offset] = 0;
//    offset += 1;
//
//    *len = offset;

    return 0;
}



////设定采集周期 解包
//int protocol_update_sample_cycle(u16 *sample_cycle, u8 *dat, u16 *len)
//{
//    u16 value = 0;
//    u16 mid = 0;
//    u8  offset = 0;
//    u8  ret = 0;
//
//    // 命令码的处理
//    if ( dat[0] != E_CMD_TYPE_SET_SAMPLE_CYCLE_REQUEST )
//    {
//        return -1;
//    }
//    offset += 1;
//    //消息会话提取
//    mid = hex_2_dec_type(&dat[offset], 2, ORD_MOTOR);
//    offset += 2;
//    //上报周期
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
//    //===组包反馈===
//    //命令码
//    offset = 0;
//    dat[offset] = E_CMD_TYPE_SET_SAMPLE_CYCLE_RESPONSE;
//    offset += 1;
//    //会话ID
//    dec_2_hex_type(&dat[offset], 2, mid, ORD_MOTOR);
//    offset += 2;
//    //状态码
//    dat[offset] = ret;
//    offset += 1;
//
//    *len = offset;
//
//    return 0;
//}

////设定服务器
//int protocol_update_server_config(char *ip, u16 *port, char *apn, u8 *dat, u16 *len)
//{
//    u8 offset = 0;
//    u16 mid = 0;
//    u8  ip_len = 0;
//    u8  apn_len = 0;
//
//    // 命令码的处理
//    if ( dat[0] != E_CMD_TYPE_SET_SERVER_CONFIG_REQUEST )
//    {
//        return -1;
//    }
//    offset += 1;
//    //消息会话提取
//    mid = hex_2_dec_type(&dat[offset], 2, ORD_MOTOR);
//    offset += 2;
//    // IP地址长度
//    ip_len = dat[offset];
//    offset++;
//    // IP地址长度
//    memcpy(ip, dat+offset, ip_len);
//    //printf("[1]ip:%s\r\n", ip);
////    protocol_format_netaddr(ip);
////    printf("[2]ip:%s\r\n", ip);
//    offset += ip_len;
//    // 端口号
//    *port = hex_2_dec_type(&dat[offset], 2, ORD_MOTOR);
//    offset += 2;
//    //printf("port:%u\r\n", *port);
//    // APN长度
//    apn_len = dat[offset];
//    offset++;
//    memcpy(apn, dat+offset, apn_len);
////    protocol_format_netaddr(apn);
//    offset += apn_len;
//    //printf("apn:%s\r\n", apn);
//
//    //===组包反馈===
//    //命令码
//    offset = 0;
//    dat[offset] = E_CMD_TYPE_SET_SERVER_CONFIG_RESPONSE;
//    offset += 1;
//    //会话ID
//    dec_2_hex_type(&dat[offset], 2, mid, ORD_MOTOR);
//    offset += 2;
//    //状态码
//    dat[offset] = 0;
//    offset += 1;
//
//    *len = offset;
//
//    return 0;
//}

//// 设备重启
//int protocol_remote_reboot(u8 *dat, u16 *len)
//{
//    u8 offset = 0;
//    u16 mid = 0;
//
//    // 命令码的处理
//    if ( dat[0] != E_CMD_TYPE_REMOTE_REBOOT_REQUEST )
//    {
//        return -1;
//    }
//    offset += 1;
//    //消息会话提取
//    mid = hex_2_dec_type(&dat[offset], 2, ORD_MOTOR);
//    offset += 2;
//
//    //===组包反馈===
//    //命令码
//    offset = 0;
//    dat[offset] = E_CMD_TYPE_REMOTE_REBOOT_RESPONSE;
//    offset += 1;
//    //会话ID
//    dec_2_hex_type(&dat[offset], 2, mid, ORD_MOTOR);
//    offset += 2;
//    //状态码
//    dat[offset] = 0;
//    offset += 1;
//
//    *len = offset;
//
//    return 0;
//}

////获取配置信息
//int protocol_get_config(u8 *dat, u16 *len, u16 hp_st, u16 hp_et, u16 upload_cycle, u16 sample_cycle)
//{
//    u8  offset = 0;
//    u8  tmp[2] = {0};
//    u16 mid = 0;
//
//    //命令码的处理
//    if ( dat[0] != E_CMD_TYPE_GET_SYSCONFIG_REQUEST )
//    {
//        return -1;
//    }
//    offset += 1;
//    //消息会话提取
//    mid = hex_2_dec_type(&dat[offset], 2, ORD_MOTOR);
//    offset += 2;
//
//     //===组包反馈===
//    //命令码
//    offset = 0;
//    dat[offset] = E_CMD_TYPE_GET_SYSCONFIG_RESPONSE;
//    offset += 1;
//    //会话ID
//    dec_2_hex_type(&dat[offset], 2, mid, ORD_MOTOR);
//    offset += 2;
//    //状态码
//    dat[offset] = 0;
//    offset += 1;
//
//    // 供暖期开始日期
//    dec_2_bcd_type(tmp, 2, hp_st, ORD_MOTOR);
//    bytes_2_ascii((char*)dat+offset, 5, tmp, 2);
//    offset += 4;
//    // 供暖期结束日期
//    dec_2_bcd_type(tmp, 2, hp_et, ORD_MOTOR);
//    bytes_2_ascii((char*)dat+offset, 5, tmp, 2);
//    offset += 4;
//    // 上报周期
//    dec_2_hex_type(&dat[offset], 2, upload_cycle, ORD_MOTOR);
//    offset += 2;
//    // 采集周期
//    dec_2_hex_type(&dat[offset], 2, sample_cycle, ORD_MOTOR);
//    offset += 2;
//
//    *len = offset;
//
//    return 0;
//}

////设定系统时间，解包
//int port_set_time_proc(SysParams *sys_params, u8 *dat, u16 *len)
//{
//    u8 offset = 0;
//    u16 mid = 0;
//
//    //命令码的处理
//    if ( dat[0] != E_CMD_TYPE_SET_SYSTIME_REQUEST )
//    {
//        return -1;
//    }
//    offset += 1;
//    //消息会话提取
//    mid = hex_2_dec_type(&dat[offset], 2, ORD_MOTOR);
//    offset += 2;
////    //系统时间,年，月，日，时，分，秒
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
//    //===组包反馈===
//    //命令码
//    offset = 0;
//    dat[offset] = E_CMD_TYPE_SET_SYSTIME_RESPONSE;
//    offset += 1;
//    //会话ID
//    dec_2_hex_type(&dat[offset], 2, mid, ORD_MOTOR);
//    offset += 2;
//    //状态码
//    dat[offset] = 0;
//    offset += 1;
//
//    *len = offset;
//
//    return 0;
//}

////获取设备信息
//int port_get_hardware_proc(SysParams *sys_params, RunParams *run_params, u8 *dat, u16 *len)
//{
//    u8 offset = 0;
//    u16 mid = 0;
//
//    //命令码的处理
//    if ( dat[0] != E_CMD_TYPE_GET_HARDWARE_REQUEST )
//    {
//        return -1;
//    }
//    offset += 1;
//    //消息会话提取
//    mid = hex_2_dec_type(dat+offset, 2, ORD_MOTOR);
//    offset += 2;
//
//
//    //===反馈组包===
//    //命令码
//    offset = 0;
//    dat[offset] = E_CMD_TYPE_GET_HARDWARE_RESPONSE;
//    offset += 1;
//    //会话ID
//    dec_2_hex_type(&dat[offset], 2, mid, ORD_MOTOR);
//    offset += 2;
//    //状态码
//    dat[offset] = 0;
//    offset += 1;
//    //固件版本
//    memcpy(&dat[offset], run_params->nbiot_module_state.firmware, sizeof(run_params->nbiot_module_state.firmware));
//    offset += 8;
//    //硬件版本
//    memcpy(&dat[offset], sys_params->hw_ver, sizeof(sys_params->hw_ver));
//    offset += 8;
//    //软件版本
//    memcpy(&dat[offset], sys_params->sw_ver, sizeof(sys_params->sw_ver));
//    offset += 8;
//    //模组唯一码IMEI
//    memcpy(&dat[offset], run_params->nbiot_module_state.imei, sizeof(run_params->nbiot_module_state.imei));
//    offset += 15;
//    //SIM卡唯一码
//    memcpy(&dat[offset], run_params->nbiot_module_state.iccid, sizeof(run_params->nbiot_module_state.iccid));
//    offset += 20;
//    //APN
//    memcpy(&dat[offset], sys_params->apn, sizeof(sys_params->apn));
//    offset += 16;
//    //服务器IP
//    memcpy(&dat[offset], sys_params->ip, sizeof(sys_params->ip));
//    offset += 15;
//    //服务器端口
//    dec_2_hex_type(&dat[offset], 2, sys_params->ip_port, ORD_MOTOR);
//    offset += 2;
//
//    *len = offset;
//
//    return 0;
//}


