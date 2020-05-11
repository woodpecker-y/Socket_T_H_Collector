/* This file has been prepared for Doxygen automatic documentation generation.*/
/*! \file *********************************************************************
*
* \brief
*    board support package--eeprom driver
* \author
*    zhou_sm(at)blackants.com.cn
* \date
*    2014-07-16
* \version
*    v1.0
* Copyright (c) 2010-2014, Blackants Energy Saving Technology co., LTD All rights reserved.
******************************************************************************/

#ifndef _BSP_EXTI_H
#define _BSP_EXTI_H

#ifdef HAVE_EXTI

/*! \brief exti confirgure Definition */
typedef struct _EXTIConfig{
    PortTypeDef gpio;       /*!< \brief exti port */
    u8          trigger;    /*!< \brief exti trigger mode */
}EXTIConfig;

/*! \brief
*      Load Key Port Configure
* \param handler[IN]        - board configure handler
* \param cnt[IN]            - extern interrupt count
* \note
*      1.exti port confirure array must be ended with "NULL".
*      ex.
*           __far PortTypeDef exti_handler[4] = {
*               {PORTA, PIN0},      // EXTI 1
*               {PORTA, PIN1},      // EXTI 2
*               {PORTA, PIN2},      // EXTI 3
*            };
*/
void BSP_EXTI_LoadConfig(const EXTIConfig *handler, u8 cnt);

/*! \brief
*      �����ⲿ�ж�ģʽ
*\param mode[IN]        ���ŵĹ���ģʽ
*       GPIO_Mode_In_FL_IT    �����жϵĸ�������
*       GPIO_Mode_In_PU_IT    �����жϵ���������
*/
void BSP_EXTI_SetPinMode(u8 mode);

/*! \brief
*      exti initilization
*/
void BSP_EXTI_Init(void);
void BSP_EXTI_InitSingle(u8 port, u8 pin, uint8_t mode);

void BSP_EXTI_Init_ByIndex(u8 idx);
void BSP_EXTI_Close_ByIndex(u8 idx);

/*! \brief
*      extern interrupt pin state
* \param idx[IN]        extern interrupt pin array index
* \return
*      0    - low level
*      1    - high level
*/
u8 BSP_EXTI_ReadState(u8 idx);
u8 BSP_EXTI_ReadStateSingle(u8 port, u8 pin);

#endif

#endif