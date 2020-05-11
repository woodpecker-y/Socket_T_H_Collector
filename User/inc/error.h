#ifndef _ERROR_H
#define _ERROR_H

#include "stm8l15x.h"

typedef enum _ErrorSet{
    FAULT_TEMP				= 0x0001,  // 温度传感器故障
    FAULT_HUMI				= 0x0002   // 温度传感器故障
//    FAULT_TEMP_SENSOR_INDOOR	        = 0x0100,  // 室内机温度传感器故障
//    FAULT_BATT_SENDOR_INDOOR	        = 0x0200,  // 电池传感器故障
    //FAULT_VALVE_IS_ACTION               = 0x0400,  // 阀门动作中
}ErrorSet;

void error_set(u16 code);

void error_clr(u16 code);

#endif