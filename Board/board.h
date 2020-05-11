#ifndef _BOARD_H
#define _BOARD_H

#include "bsp.h"
#include "nb.h"
#include "sht3x.h"
#include "bsp1_adc.h"

//////////////////////////////��Ʒ����////////////////////////////////////
extern const char *manufacturer;
extern const char *product_model;
//extern const char *product_model_ba2804;
//extern const char *product_model_v5;
//extern const char *product_model_v1;
extern const char *hw_ver;
extern const char *sw_ver;

//////////////////////////////��������///////////////////////
#define MAX_COM_BUFF_SIZE   128
extern uint8_t com_global_buffer[MAX_COM_BUFF_SIZE+1];

extern COMConfig com_cfg[1];

//////////////////// light���� //////////////////////////////
//typedef enum _LightIndex{
//    E_LIGHT_1 = 0,
//    E_LIGHT_2
//}LightIndex;

//extern PortTypeDef light_config[1];

//extern EXTIConfig light_check_config[2];

//////////////////// KEY���� //////////////////////////////
//#define KEY_CNT 2
//extern PortTypeDef key_config[2];

//////////////////// �������ݵ�ѹ��⼰������� /////////////
//#define SUPER_CAPACITOR_RECHARGE 0

//extern ADCConfig adc_cfg[1];

//extern PortTypeDef super_capacitor_recharge_cfg;

/////////////////// NB-IoT���� //////////////////////////
extern NBConfig nb_config;

/////////////////// ��ʪ�ȴ��������� //////////////////////
extern SHT3xConfig sht30_cfg;

//////////////////// LCD���� //////////////////////////////
//extern LCDPortConfig lcd_config;

#endif