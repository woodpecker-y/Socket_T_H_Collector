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
*    ????????:
*       |-->BSP_ADC_Init()
*           |-->BSP_ADC_SampleInit()
*               |-->BSP_ADC_SampleOnce()
*               |-->...
*               |-->BSP_ADC_SampleOnce()
*           |-->BSP_ADC_SampleFinish()
*       |-->BSP_ADC_Close()
* Copyright (c) 2010-2013, Blackants Energy Saving Technology co., LTD All rights reserved.
******************************************************************************/
#include "stm8l15x.h"
#include "bsp1_adc.h"

/*! \brief adc sample configure handler pointer */
static ADCConfig *s_adc_config_handler;
static u8 s_adc_num = 0;
ADC_TypeDef *adc_list[1] = {ADC1};

/*! \brief
 *       AD??????????????
 *
 * \param us[IN]	    ??????
 *
 */
//void ADC_DelayUs(u16 us)
//{
//    u16 i = 0;
//    u16 j = 0;
//
//    for (i=0; i<us; ++i)
//    {
//        for (j=0; j<4; ++j)
//            ;
//    }
//}
uint16_t BSP_ADC_GetChannel(u8 port, u8 pin)
{
    switch(port)
    {
    case PORTA:
        switch(pin)
        {
        case PIN4:
            return ADC_Channel_2;
        //break;
        case PIN5:
            return ADC_Channel_1;
        //break;
        case PIN6:
            return ADC_Channel_0;
        //break;
        default:
            break;
        }
        break;
    case PORTB:
        switch(pin)
        {
        case PIN0:
            return ADC_Channel_18;
        case PIN1:
            return ADC_Channel_17;
        case PIN2:
            return ADC_Channel_16;
        case PIN3:
            return ADC_Channel_15;
        case PIN4:
            return ADC_Channel_14;
        case PIN5:
            return ADC_Channel_13;
        case PIN6:
            return ADC_Channel_12;
        case PIN7:
            return ADC_Channel_11;
        default:
            break;
        }
        break;
    case PORTC:
        switch(pin)
        {
        case PIN2:
            return ADC_Channel_6;
        //break;
        case PIN3:
            return ADC_Channel_5;
        //break;
        case PIN4:
            return ADC_Channel_4;
        //break;
        case PIN7:
            return ADC_Channel_3;
        //break;
        default:
            break;
        }
        break;
    case PORTD:
        switch(pin)
        {
        case PIN0:
            return ADC_Channel_22;
        //break;
        case PIN1:
            return ADC_Channel_21;
        //break;
        case PIN2:
            return ADC_Channel_20;
        //break;
        case PIN3:
            return ADC_Channel_19;
        //break;
        case PIN4:
            return ADC_Channel_10;
        //break;
        case PIN5:
            return ADC_Channel_9;
        //break;
        case PIN6:
            return ADC_Channel_8;
        //break;
        case PIN7:
            return ADC_Channel_7;
        //break;
        default:
            break;
        }
    case PORTE:
        switch(pin)
        {
        case PIN5:
            return ADC_Channel_23;
        default:
            break;
        }
    case PORTF:
        switch(pin)
        {
        case PIN0:
            return ADC_Channel_24;
        case PIN1:
            return ADC_Channel_25;
        case PIN2:
            return ADC_Channel_26;
        case PIN3:
            return ADC_Channel_27;
        default:
            break;
        }
    default:
        break;
    }

    return 0xFF;
}

/*! \brief
 *       Load ADC Sample port configure
 * \param handler[IN]        - adc sample configure handler
 */
void BSP_ADC_LoadConfig(ADCConfig *handler, u8 num)
{
    s_adc_config_handler = handler;
    s_adc_num            = num;
}

/*! \brief
 *       adc sample all pin initilization
 * \param idx[IN]        - adc sample configure index which is in the handler array.
 */
void BSP_ADC_PowerOn(u8 idx)
{
    assert_param(idx < s_adc_num);

    // power on
    if (NULL != &(s_adc_config_handler[idx].ctrl))
    {
        if (s_adc_config_handler[idx].pwr_le == E_LE_LOW)
        {
            BSP_GPIO_Init(s_adc_config_handler[idx].ctrl.port, s_adc_config_handler[idx].ctrl.pin, GPIO_Mode_Out_PP_Low_Fast);
        }
        else
        {
            BSP_GPIO_Init(s_adc_config_handler[idx].ctrl.port, s_adc_config_handler[idx].ctrl.pin, GPIO_Mode_Out_PP_High_Fast);
        }
    }

    //v1.0.15.180403 ????????IC???????????????ADC??????????????
//    // pin init
//    BSP_GPIO_Init(s_adc_config_handler[idx].ai.port, s_adc_config_handler[idx].ai.pin, GPIO_Mode_In_FL_No_IT);
}

/*! \brief
 *       adc sample all pin initilization
 * \param idx[IN]        - adc sample configure index which is in the handler array.
 */
void BSP_ADC_PowerOn_OpenDrain(u8 idx)
{
    assert_param(idx < s_adc_num);

    // power on
    if (NULL != &(s_adc_config_handler[idx].ctrl))
    {
        BSP_GPIO_Init(s_adc_config_handler[idx].ctrl.port, s_adc_config_handler[idx].ctrl.pin, GPIO_Mode_Out_OD_Low_Fast);

        if (s_adc_config_handler[idx].pwr_le == E_LE_LOW)
        {
            BSP_GPIO_SetLow(s_adc_config_handler[idx].ctrl.port, s_adc_config_handler[idx].ctrl.pin);
        }
        else
        {
            BSP_GPIO_SetHigh(s_adc_config_handler[idx].ctrl.port, s_adc_config_handler[idx].ctrl.pin);
            //BSP_GPIO_Init(s_adc_config_handler[idx].ctrl.port, s_adc_config_handler[idx].ctrl.pin, GPIO_Mode_Out_OD_HiZ_Fast);
        }
    }

    //v1.0.15.180403 ????????IC???????????????ADC??????????????
//    // pin init
//    BSP_GPIO_Init(s_adc_config_handler[idx].ai.port, s_adc_config_handler[idx].ai.pin, GPIO_Mode_In_FL_No_IT);
}

/*! \brief
 *       adc sample all pin close
 * \param idx[IN]        - adc sample configure index which is in the handler array.
 */
void BSP_ADC_PowerOff(u8 idx)
{
    assert_param(idx < s_adc_num);

    // power off
    if (NULL != &(s_adc_config_handler[idx].ctrl))
    {
        if (s_adc_config_handler[idx].pwr_le == E_LE_LOW)
        {
            BSP_GPIO_Init(s_adc_config_handler[idx].ctrl.port, s_adc_config_handler[idx].ctrl.pin, GPIO_Mode_Out_PP_High_Slow);
        }
        else
        {
            BSP_GPIO_Init(s_adc_config_handler[idx].ctrl.port, s_adc_config_handler[idx].ctrl.pin, GPIO_Mode_Out_PP_Low_Slow);
        }
    }

    //v1.0.15.180403 ????????IC???????????????ADC??????????????
//    // pin close
//    BSP_GPIO_Init(s_adc_config_handler[idx].ai.port, s_adc_config_handler[idx].ai.pin, GPIO_Mode_Out_PP_Low_Slow);
    //BSP_GPIO_Init(s_adc_config_handler[idx].ai.port, s_adc_config_handler[idx].ai.pin, GPIO_Mode_Out_PP_High_Slow);
}

/*! \brief
 *       adc sample all pin close
 * \param idx[IN]        - adc sample configure index which is in the handler array.
 */
void BSP_ADC_PowerOff_OpenDrain(u8 idx)
{
    assert_param(idx < s_adc_num);

    // power off
    if (NULL != &(s_adc_config_handler[idx].ctrl))
    {
        if (s_adc_config_handler[idx].pwr_le == E_LE_LOW)
        {
            BSP_GPIO_Init(s_adc_config_handler[idx].ctrl.port, s_adc_config_handler[idx].ctrl.pin, GPIO_Mode_Out_OD_HiZ_Fast);
        }
        else
        {
            BSP_GPIO_Init(s_adc_config_handler[idx].ctrl.port, s_adc_config_handler[idx].ctrl.pin, GPIO_Mode_Out_OD_Low_Fast);
        }
    }

    //v1.0.15.180403 ????????IC???????????????ADC??????????????
//    // pin close
//    BSP_GPIO_Init(s_adc_config_handler[idx].ai.port, s_adc_config_handler[idx].ai.pin, GPIO_Mode_Out_PP_Low_Slow);
    //BSP_GPIO_Init(s_adc_config_handler[idx].ai.port, s_adc_config_handler[idx].ai.pin, GPIO_Mode_Out_PP_High_Slow);
}

CLK_Peripheral_TypeDef BSP_ADC_GetCLK(ADC_TypeDef* adc)
{
    assert_param(adc==ADC1);

    if (adc == ADC1)
    {
        return CLK_Peripheral_ADC1;
    }

    return 0x00;
}

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
int BSP_ADC_Init(u8 idx)
{
    uint16_t ch = 0xFF;
    CLK_Peripheral_TypeDef clk = 0;
    
    assert_param(idx < s_adc_num);

    ch = BSP_ADC_GetChannel(s_adc_config_handler[idx].ai.port, s_adc_config_handler[idx].ai.pin);
    if (ch == 0xFF)
    {
        return -1;
    }
    
    //v1.0.15.180403 ????????IC???????????????ADC??????????????
    // pin init
    BSP_GPIO_Init(s_adc_config_handler[idx].ai.port, s_adc_config_handler[idx].ai.pin, GPIO_Mode_In_FL_No_IT);

    clk = BSP_ADC_GetCLK(s_adc_config_handler[idx].adc);
    if (clk == 0)
    {
        return -2;
    }
    //printf("clk:%02X\r\n", clk);
    /* Enable ADC clock */
    CLK_PeripheralClockConfig(clk, ENABLE);

    /* de-initialize ADC */
    ADC_DeInit(s_adc_config_handler[idx].adc);

    ADC_Cmd(s_adc_config_handler[idx].adc, ENABLE);
    ADC_Init(s_adc_config_handler[idx].adc, ADC_ConversionMode_Single, ADC_Resolution_12Bit, ADC_Prescaler_1);

    if ((ADC_Channel_TypeDef)ch != ADC_Channel_24)
    {
        ADC_SamplingTimeConfig(s_adc_config_handler[idx].adc, ADC_Group_SlowChannels, ADC_SamplingTime_96Cycles);
    }
    else
    {
        ADC_SamplingTimeConfig(s_adc_config_handler[idx].adc, ADC_Group_FastChannels, ADC_SamplingTime_192Cycles);
        /* disable SchmittTrigger for ADC_Channel_24, to save power */
        ADC_SchmittTriggerConfig(s_adc_config_handler[idx].adc, ADC_Channel_24, DISABLE);
    }

    ADC_ChannelCmd(s_adc_config_handler[idx].adc, (ADC_Channel_TypeDef)ch, ENABLE);

    return 0;
}

/*! \brief
 *       adc sample once
 * \return
 *       adc sample value
 * \note
 *       1?????????BSP_ADC_SampleInit???????????
 *       2??BSP_ADC_SampleInit???????????????????25us????????????
 */
u32 BSP_ADC_SampleOnce(u8 idx)
{
    u32 res = 0;

    assert_param(idx < s_adc_num);

    /* start ADC convertion by software */
    ADC_SoftwareStartConv(s_adc_config_handler[idx].adc);
    /* wait until end-of-covertion */
    while( ADC_GetFlagStatus(s_adc_config_handler[idx].adc, ADC_FLAG_EOC) == 0 ) ;
    /* read ADC convertion result */
    res = ADC_GetConversionValue(s_adc_config_handler[idx].adc);

    return res;
}

/*! \brief
 *       adc sample finish
 * \param adc_channel[IN]        - adc sample channel
 */
void BSP_ADC_Close(u8 idx)
{
    uint16_t ch = 0xFF;
    CLK_Peripheral_TypeDef clk = 0;

    ch = BSP_ADC_GetChannel(s_adc_config_handler[idx].ai.port, s_adc_config_handler[idx].ai.pin);
    if (ch == 0xFF)
    {
        return;
    }

    ADC_DeInit(s_adc_config_handler[idx].adc);

    /* disable SchmittTrigger for ADC_Channel_24, to save power */
    if ((ADC_Channel_TypeDef)ch == ADC_Channel_24)
    {
        ADC_SchmittTriggerConfig(s_adc_config_handler[idx].adc, ADC_Channel_24, DISABLE);
        ADC_ChannelCmd(s_adc_config_handler[idx].adc, (ADC_Channel_TypeDef)ch, DISABLE);
    }
    else
    {
        ADC_ChannelCmd(s_adc_config_handler[idx].adc, (ADC_Channel_TypeDef)ch, DISABLE);
    }
    //ADC_SchmittTriggerConfig(s_adc_config_handler[idx].adc, ADC_Channel_24, DISABLE);
    clk = BSP_ADC_GetCLK(s_adc_config_handler[idx].adc);
    if (clk == 0)
    {
        return;
    }    
    
    CLK_PeripheralClockConfig(clk, DISABLE);
    //ADC_ChannelCmd(s_adc_config_handler[idx].adc, (ADC_Channel_TypeDef)s_adc_config_handler[idx].ch, DISABLE);
    
    //v1.0.15.180403 ????????IC???????????????ADC??????????????
    // pin close
    BSP_GPIO_Init(s_adc_config_handler[idx].ai.port, s_adc_config_handler[idx].ai.pin, GPIO_Mode_Out_PP_Low_Slow);

    return;
}

/*! \brief
 *       adc sample initilization with VREFINT Channel
 * \return
 *       0  - successful
 */
int BSP_ADC_Init_Vrefint(void)
{
    CLK_PeripheralClockConfig(CLK_Peripheral_ADC1, ENABLE);
    ADC_DeInit(ADC1);

    ADC_Cmd(ADC1, ENABLE);
    ADC_Init(ADC1, ADC_ConversionMode_Single, ADC_Resolution_12Bit, ADC_Prescaler_1);
    ADC_SamplingTimeConfig(ADC1, ADC_Group_SlowChannels, ADC_SamplingTime_96Cycles);

    ADC_VrefintCmd(ENABLE);
    ADC_ChannelCmd(ADC1,ADC_Channel_Vrefint,ENABLE);

    return 0;
}

/*! \brief
 *       ??VREFINT??????????AD????
 * \return
 *       AD?????
 * \note
 *       1?????????BSP_ADC_Init_Vrefint???????????
 *       2??BSP_ADC_Init_Vrefint???????????????????25us????????????
 */
u32 BSP_ADC_SampleOnceVrefint(void)
{
    u32 res = 0;

    /* start ADC convertion by software */
    ADC_SoftwareStartConv(ADC1);
    /* wait until end-of-covertion */
    while( ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC) == 0 ) ;
    /* read ADC convertion result */
    res = ADC_GetConversionValue(ADC1);

    return res;
}

/*! \brief
 *       adc sample close with VREFINT Channel
 */
void BSP_ADC_CloseVrefint(void)
{
    ADC_ChannelCmd(ADC1,ADC_Channel_Vrefint,DISABLE);
    ADC_VrefintCmd(DISABLE);
    ADC_Cmd(ADC1, DISABLE);
    CLK_PeripheralClockConfig(CLK_Peripheral_ADC1, DISABLE);

    return;
}

/*! \brief
 *       adc sample initilization single
 * \param adc_idx[IN]           - adc idx:E_ADC1
 * \param port[IN]              - adc idx:ADC Sample Port
 * \param pin[IN]               - adc idx:ADC Sample Pin
 * \return
 *       none
 */
int BSP_ADC_Init_Single(u8 adc_idx, u8 port, u8 pin)
{
    uint16_t ch = 0xFF;
    
    ADC_TypeDef* adc = adc_list[adc_idx];;
    
    assert_param(adc_idx == E_ADC1);    

    ch = BSP_ADC_GetChannel(port, pin);
    if (ch == 0xFF)
    {
        return -1;
    }
    
    //v1.0.15.180403 ????????IC???????????????ADC??????????????
    // pin init
    BSP_GPIO_Init(port, pin, GPIO_Mode_In_FL_No_IT);

    /* Enable ADC clock */
    CLK_PeripheralClockConfig(BSP_ADC_GetCLK(adc), ENABLE);

    /* de-initialize ADC */
    ADC_DeInit(adc);

    ADC_Cmd(adc, ENABLE);
    ADC_Init(adc, ADC_ConversionMode_Single, ADC_Resolution_12Bit, ADC_Prescaler_1);

    if ((ADC_Channel_TypeDef)ch != ADC_Channel_24)
    {
        ADC_SamplingTimeConfig(adc, ADC_Group_SlowChannels, ADC_SamplingTime_96Cycles);
    }
    else
    {
        ADC_SamplingTimeConfig(adc, ADC_Group_FastChannels, ADC_SamplingTime_192Cycles);
        /* disable SchmittTrigger for ADC_Channel_24, to save power */
        ADC_SchmittTriggerConfig(adc, ADC_Channel_24, DISABLE);
    }

    ADC_ChannelCmd(adc, (ADC_Channel_TypeDef)ch, ENABLE);

    return 0;
}

/*! \brief
 *       adc sample once single
 * \return
 *       adc sample value
 * \note
 *       1?????????BSP_ADC_SampleInit???????????
 *       2??BSP_ADC_SampleInit???????????????????25us????????????
 */
u32 BSP_ADC_SampleOnce_Single(u8 adc_idx)
{
    u32 res = 0;
    
    ADC_TypeDef* adc = adc_list[adc_idx];

   assert_param(adc_idx == E_ADC1);

    /* start ADC convertion by software */
    ADC_SoftwareStartConv(adc);
    /* wait until end-of-covertion */
    while( ADC_GetFlagStatus(adc, ADC_FLAG_EOC) == 0 ) ;
    /* read ADC convertion result */
    res = ADC_GetConversionValue(adc);

    return res;
}

/*! \brief
 *       adc sample finish single
 * \param adc_channel[IN]        - adc sample channel
 */
void BSP_ADC_Close_Single(u8 adc_idx, u8 port, u8 pin)
{
    uint16_t ch = 0xFF;
    
    ADC_TypeDef* adc = adc_list[adc_idx];
    
    assert_param(adc_idx == E_ADC1);

    ch = BSP_ADC_GetChannel(port, pin);
    if (ch == 0xFF)
    {
        return;
    }

    ADC_DeInit(adc);

    /* disable SchmittTrigger for ADC_Channel_24, to save power */
    if ((ADC_Channel_TypeDef)ch == ADC_Channel_24)
    {
        ADC_SchmittTriggerConfig(adc, ADC_Channel_24, DISABLE);
        ADC_ChannelCmd(adc, (ADC_Channel_TypeDef)ch, DISABLE);
    }
    else
    {
        ADC_ChannelCmd(adc, (ADC_Channel_TypeDef)ch, DISABLE);
    }
    //ADC_SchmittTriggerConfig(adc, ADC_Channel_24, DISABLE);
    CLK_PeripheralClockConfig(BSP_ADC_GetCLK(adc), DISABLE);
    //ADC_ChannelCmd(adc, (ADC_Channel_TypeDef)ch, DISABLE);
    
    //v1.0.15.180403 ????????IC???????????????ADC??????????????
    // pin close
    BSP_GPIO_Init(port, pin, GPIO_Mode_Out_PP_Low_Slow);

    return;
}

