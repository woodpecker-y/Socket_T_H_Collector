/* This file has been prepared for Doxygen automatic documentation generation.*/
/*! \file *********************************************************************
*
* \brief
*    hardware abstract layer--led driver
* \author
*    zhou_sm(at)blackants.com.cn
* \date
*    2014-07-16
* \version
*    v1.0
* Copyright (c) 2010-2014, Blackants Energy Saving Technology co., LTD All rights reserved.
******************************************************************************/
#ifndef _LED_H
#define _LED_H

#ifdef HAVE_LED

#include "bsp.h"

/*! \brief
*      Load LED Port Configure
* \param handler[IN]        - board configure handler
* \param cnt[IN]            - LED Count
* \note
*      ex.
*           const PortTypeDef led_handler[] = {
*               {PORTA, PIN0},      // LED 1
*               {PORTA, PIN1},      // LED 2
*               {PORTA, PIN2},      // LED 3
*            };
*/
void led_load_config(PortTypeDef *handler, u8 cnt);

/*! \brief
*      Load LED Port Init
*/
void led_init(void);

/*! \brief
*      set led on
* \param led_idx[IN]        - led index of led configure array
*/
void led_on(u8 idx);

/*! \brief
*      set led off
* \param led_idx[IN]        - led index of led configure array
*/
void led_off(u8 idx);

#endif

#endif
