#ifndef _PROTOCOL_H
#define _PROTOCOL_H

#include "log_data.h"
#include "sysparams.h"
#include "adf.h"

typedef enum _CommandType{
    E_CMD_TYPE_REPORT = 0,      //�ϱ�
    E_CMD_TYPE_SET_FIXTH,       //�·�����ֵ
    E_CMD_TYPE_SET_REPORTCYCLE, //�·��ϴ�����
    E_CMD_TYPE_SET_FIXTH_RESPONSE,
    E_CMD_TYPE_SET_REPORTCYCLE_RESPONSE
}CommandType;

// ��ʱ�ϱ�
int protocol_report_request(u8* dat, u16 *len, u8 *addr, LogData *log);

//�趨��ʪ�Ȳ���ֵ ���
int protocol_fix_t_h(s8 *fix_temp, s8 *fix_humi, s8 *dat, u16 *len);

// �趨�ϱ�����
int protocol_update_upload_cycle(u16 *upload_cycle, u8 *dat, u16 *len);



#endif