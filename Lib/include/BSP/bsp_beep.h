/* This file has been prepared for Doxygen automatic documentation generation.*/
/*! \file *********************************************************************
*
* \brief
*    board support package--beep driver
* \author
*    zhou_sm(at)blackants.com.cn
* \date
*    2014-09-16
* \version
*    v1.0
* Copyright (c) 2010-2014, Blackants Energy Saving Technology co., LTD All rights reserved.
******************************************************************************/
#ifndef _BSP_BEEP_H
#define _BSP_BEEP_H

#ifdef HAVE_BEEP

#include "bsp_gpio.h"

/*! \brief
*       beep load configure
* \param handler[IN]                - beep handler
*/
void BSP_BEEP_LoadConfig(PortTypeDef *handler);

/*! \brief
*       beep initilization
* \param handler[IN]                - beep handler
*/
void BSP_BEEP_Init(void);

/*! \brief
*       beep set on
*/
void BSP_BEEP_On(void);

/*! \brief
*       beep set off
*/
void BSP_BEEP_Off(void);

#endif

#endif