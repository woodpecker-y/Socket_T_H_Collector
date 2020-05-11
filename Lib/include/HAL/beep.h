/* This file has been prepared for Doxygen automatic documentation generation.*/
/*! \file *********************************************************************
*
* \brief
*    hardware abstract layer--beep driver
* \author
*    zhou_sm(at)blackants.com.cn
* \date
*    2014-07-16
* \version
*    v1.0
* Copyright (c) 2010-2014, Blackants Energy Saving Technology co., LTD All rights reserved.
******************************************************************************/
#ifndef _BEEP_H
#define _BEEP_H

#ifdef HAVE_BEEP

/*! \brief
*       beep load configure
* \param handler[IN]        - beep port configure handler
*/
void beep_load_config(PortTypeDef *handler);

/*! \brief
*       beep initilization
*/
void beep_init(void);

/*! \brief
*       beep on
*/
void beep_on(void);

/*! \brief
*       beep off
*/
void beep_off(void);

#endif

#endif

