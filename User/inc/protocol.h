#ifndef _PROTOCOL_H
#define _PROTOCOL_H

#include "log_data.h"
#include "sysparams.h"
#include "adf.h"

typedef enum _CommandType{
    E_CMD_TYPE_REPORT = 0,      //上报
    E_CMD_TYPE_SET_FIXTH,       //下发补偿值
    E_CMD_TYPE_SET_REPORTCYCLE, //下发上传周期
    E_CMD_TYPE_SET_FIXTH_RESPONSE,
    E_CMD_TYPE_SET_REPORTCYCLE_RESPONSE
}CommandType;

// 定时上报
int protocol_report_request(u8* dat, u16 *len, u8 *addr, LogData *log);

//设定温湿度补偿值 解包
int protocol_fix_t_h(s8 *fix_temp, s8 *fix_humi, s8 *dat, u16 *len);

// 设定上报周期
int protocol_update_upload_cycle(u16 *upload_cycle, u8 *dat, u16 *len);



#endif