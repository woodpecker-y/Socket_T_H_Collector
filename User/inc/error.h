#ifndef _ERROR_H
#define _ERROR_H

#include "stm8l15x.h"

typedef enum _ErrorSet{
    FAULT_TEMP				= 0x0001,  // �¶ȴ���������
    FAULT_HUMI				= 0x0002   // �¶ȴ���������
//    FAULT_TEMP_SENSOR_INDOOR	        = 0x0100,  // ���ڻ��¶ȴ���������
//    FAULT_BATT_SENDOR_INDOOR	        = 0x0200,  // ��ش���������
    //FAULT_VALVE_IS_ACTION               = 0x0400,  // ���Ŷ�����
}ErrorSet;

void error_set(u16 code);

void error_clr(u16 code);

#endif