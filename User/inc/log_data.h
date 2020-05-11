#ifndef _LOG_DATA_H
#define _LOG_DATA_H

#include "adf.h"

#define LOG_BASE_ADDR           85      //eeprom�洢��ʼλ��
#define LOG_MAX_RECORDS_COUNT   72      //�洢���ݵ������  //20���Ӵ洢һ������ ���洢һ���������

typedef struct _LogData{
    s16         temp;                                           // �¶�
    s16         humi;                                           // ʪ��
    u8          rssi;                                           // �ź�ǿ��
    u8          st;                                             // ������
    u8          sample_time[7];                                 // �ɼ�ʱ��
}LogData;

void log_write(LogData *log, u32 records_wr);

void log_read(LogData *log, u32 records_rd);

void log_clear_data(void);

extern LogData   g_last_log_data;

#endif