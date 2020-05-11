/*! \file *********************************************************************
*
* \brief
*    SHT3x sensor
* \author
*    lv_my(at)blackants.com.cn
* \date
*    2018-08-30
* \version
*    v1.0
* \note
*    调用流程:
*       |-->BSP_SHT3x_PowerOn()
*           |-->BSP_SHT3x_Init()
*               |-->BSP_SHT3x_Acquisition()
*           |-->BSP_SHT3x_Close()
*       |-->BSP_SHT3x_PowerOff()
******************************************************************************/
/*! \example *********************************************************************
void task_sample_proc(void)
{
    switch(s_collect_handler.sts)
    {
    case E_COLLECT_IDLE:     
        s_collect_handler.cycle_timer--;
        if( 0 >= s_collect_handler.cycle_timer)
        {
            s_collect_handler.sts = E_COLLECT_REDAY;
            s_collect_handler.cycle_timer = MAX_IDLE_TEMP_TIMER;
        }
        break;
    case E_COLLECT_REDAY:
        BSP_SHT3x_PowerOn();
        BSP_SHT3x_Init();
        s_collect_handler.sts = E_COLLECT_ACTIVE;
        break;
    case E_COLLECT_ACTIVE:
         s16 temp = 0;
         s16 humi = 0;
         BSP_SHT3x_Acquisition(&temp, &humi);
         s_collect_handler.sts = E_COLLECT_FINISH ;
         break;
    case E_COLLECT_FINISH:
         BSP_SHT3x_Close();
         BSP_SHT3x_PowerOff();
         s_collect_handler.sts = E_COLLECT_IDLE;
         break;
    default:
         break;
    }
}
******************************************************************************/
#ifndef __SHT3X_H
#define __SHT3X_H

#ifdef HAVE_SHT3x
#include "bsp.h"

typedef struct _SHT3xConfig{
    PortTypeDef scl;
    PortTypeDef sda;
    PortTypeDef pwr; 
    u8          pwr_le;         /*!< \brief power level effective */    
}SHT3xConfig;

#define BSP_SHT3x_Delay(a)          delay_us(a)

#define Bit_RESET               0
#define Bit_SET                 1
//
//typedef enum _SHT3xRet{
//    E_SHT3X_OK = 0,
//    E_SHT3X_ERR_NO_RESPONSE = -1,
//}SHT3xRet;

//typedef enum _SHT2xRet{
//    E_SHT_OK = 0,
//    E_SHT_ERR_NO_RESPONSE = -1,
//}SHT2xRet;

typedef enum {
    SHT3x_I2C_ADR_W                  = 0x88, //sensor I2C address + write bit
    SHT3x_I2C_ADR_R                  = 0x89  //sensor I2C address + read bit
//    I2C_ADR_W                  = 0x88, //sensor I2C address + write bit
//    I2C_ADR_R                  = 0x89  //sensor I2C address + read bit    
} SHT3xI2cHeader;

//typedef enum {
////    E_SHT3x_ACK_OK                     = 0x00,  
////    E_SHT3x_ACK_ERROR                  = 0x01
//    ACK_OK                     = 0x00,  
//    ACK_ERROR                  = 0x01    
//} SHT3xI2cAckState;

// SHT3x配置函数
void BSP_SHT3x_Load_Config(SHT3xConfig *handler);
// SHT3x上电函数
void BSP_SHT3x_PowerOn(void);
// SHT3x断电函数
void BSP_SHT3x_PowerOff(void);
// SHT3x初始化函数
void BSP_SHT3x_Init(void);
// SHT3x关闭函数
void BSP_SHT3x_Close(void);
// SHT3x软件复位
void BSP_SHT3x_SoftReset(void);
// SHT3x采集温湿度函数
int BSP_SHT3x_Acquisition(s16 *temp,s16 *humi);

#endif

#endif

