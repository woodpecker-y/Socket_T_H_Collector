/* This file has been prepared for Doxygen automatic documentation generation.*/
/*! \file *********************************************************************
*
* \brief
*    board support package--AD sample driver
* \author
*    zhou_sm(at)blackants.com.cn
* \date
*    2014-07-13
* \version
*    v1.0
* \note
*    调用流程:
*       |-->BSP_ADC_LoadConfig()
*           |-->BSP_ADC_PowerOn() or BSP_ADC_PowerOn_OpenDrain()
*               |-->BSP_ADC_Init() or BSP_ADC_Init_Vrefint()
*                   |-->BSP_ADC_SampleOnce() or BSP_ADC_SampleOnceVrefint
*               |-->BSP_ADC_Close() or BSP_ADC_CloseVrefint()
*           |-->BSP_ADC_PowerOff() or BSP_ADC_PowerOff_OpenDrain
* Copyright (c) 2010-2013, Blackants Energy Saving Technology co., LTD All rights reserved.
******************************************************************************/
#ifndef _STM8L151_ADC_H
#define _STM8L151_ADC_H

#include "bsp.h"

typedef enum _E_ADC_Ret{
    E_ADC_OK = 0,
    E_ADC_ERR_EXCEED_MAX_IDX,
    E_ADC_ERR_UNKNOWN_ADC,
}E_ADC_Ret;
    
typedef enum _ADCIdx{
  E_ADC1 = 0
}ADCIdx;

typedef struct _ADCConfig{
    ADC_TypeDef*    adc;
    PortTypeDef     ctrl;
    PortTypeDef     ai;
    u8              pwr_le;
}ADCConfig;

/*! \brief
*       Load ADC Sample port configure
* \param handler[IN]        - adc sample configure handler
*/
void BSP_ADC_LoadConfig(ADCConfig *handler, u8 num);

/*! \brief
*       adc sample all pin initilization
* \param idx[IN]        - adc sample configure index which is in the handler array.
*/
void BSP_ADC_PowerOn(u8 idx);
void BSP_ADC_PowerOn_OpenDrain(u8 idx);

/*! \brief
*       adc sample all pin close
* \param idx[IN]        - adc sample configure index which is in the handler array.
*/
void BSP_ADC_PowerOff(u8 idx);
void BSP_ADC_PowerOff_OpenDrain(u8 idx);

/*! \brief
*       adc sample initilization
* \param adc_channel[IN]        - adc samle channel
*           ADC_Channel_0: Channel 0
*           ADC_Channel_1: Channel 1
*           ADC_Channel_2: Channel 2
*           ADC_Channel_3: Channel 3
*           ADC_Channel_4: Channel 4
*           ADC_Channel_5: Channel 5
*           ADC_Channel_6: Channel 6
*           ADC_Channel_7: Channel 7
*           ADC_Channel_8: Channel 8
*           ADC_Channel_9: Channel 9
*           ADC_Channel_10: Channel 10
*           ADC_Channel_11: Channel 11
*           ADC_Channel_12: Channel 12
*           ADC_Channel_13: Channel 13
*           ADC_Channel_14: Channel 14
*           ADC_Channel_15: Channel 15
*           ADC_Channel_16: Channel 16
*           ADC_Channel_17: Channel 17
*           ADC_Channel_18: Channel 18
*           ADC_Channel_19: Channel 19
*           ADC_Channel_20: Channel 20
*           ADC_Channel_21: Channel 21
*           ADC_Channel_22: Channel 22
*           ADC_Channel_23: Channel 23
*           ADC_Channel_24: Channel 24
*           ADC_Channel_25: Channel 25
*           ADC_Channel_26: Channel 26
*           ADC_Channel_27: Channel 27
* \return
*       none
*/
int BSP_ADC_Init(u8 idx);

/*! \brief
 *       adc sample initilization with VREFINT Channel
 * \return
 *       0  - successful
 */
int BSP_ADC_Init_Vrefint(void);

/*! \brief
*       进行一次AD采样
* \return
*       AD采样值
* \note
*       1、必须调用BSP_ADC_SampleInit之后才可以使用；
*       2、BSP_ADC_SampleInit初始化完毕后必须延时至少25us才能获取正确值；
*/
u32 BSP_ADC_SampleOnce(u8 idx);

/*! \brief
*       对VREFINT通道进行一次AD采样
* \return
*       AD采样值
* \note
*       1、必须调用BSP_ADC_Init_Vrefint之后才可以使用；
*       2、BSP_ADC_Init_Vrefint初始化完毕后必须延时至少25us才能获取正确值；
*/
u32 BSP_ADC_SampleOnceVrefint(void);

/*! \brief
*       AD采样结束
* \param adc_channel[IN]        采样通道
*/
void BSP_ADC_Close(u8 idx);

/*! \brief
 *       adc sample close with VREFINT Channel
 */
void BSP_ADC_CloseVrefint(void);

/*! \brief
 *       adc sample initilization single
 * \param adc_idx[IN]           - adc idx:E_ADC1
 * \param port[IN]              - adc idx:ADC Sample Port
 * \param pin[IN]               - adc idx:ADC Sample Pin
 * \return
 *       none
 */
int BSP_ADC_Init_Single(u8 adc_idx, u8 port, u8 pin);

/*! \brief
 *       adc sample once single
 * \return
 *       adc sample value
 * \note
 *       1、必须调用BSP_ADC_SampleInit之后才可以使用；
 *       2、BSP_ADC_SampleInit初始化完毕后必须延时至少25us才能获取正确值；
 */
u32 BSP_ADC_SampleOnce_Single(u8 adc_idx);

/*! \brief
 *       adc sample finish single
 * \param adc_channel[IN]        - adc sample channel
 */
void BSP_ADC_Close_Single(u8 adc_idx, u8 port, u8 pin);

#endif
