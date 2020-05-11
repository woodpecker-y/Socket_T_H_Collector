/* This file has been prepared for Doxygen automatic documentation generation.*/
/*! \file *********************************************************************
*
* \brief
*    board support package-- delay driver
* \author
*    zhou_sm(at)blackants.com.cn
* \date
*    2013-01-19
* \version
*    v1.0
* Copyright (c) 2010-2014, Blackants Energy Saving Technology co., LTD All rights reserved.
******************************************************************************/
#ifndef _DELAY_H
#define _DELAY_H

#include "bsp.h"

/*! \brief
*       delay config
* \param pre[IN]        - sysclk div
*       CLK_SYSCLKDiv_1
*       CLK_SYSCLKDiv_2
*       CLK_SYSCLKDiv_4
*       CLK_SYSCLKDiv_8
*       CLK_SYSCLKDiv_16
*/
void delay_config(u8 pre);

/*! \brief
*       millisecond delay
* \param _1ms[IN]        - delay time
*/
void delay_ms(u16 _1ms);

/*! \brief
*       microseconds delay
* \param us[IN]        - delay time
*/
void delay_us(u16 us);


/*! \brief
*       millisecond delay
* \param _1ms[IN]        - delay time
* \notes
*       仅适用于晶振分频系数为1的情况
*/
void delay_us_by_timer(u16 _1us);

#endif