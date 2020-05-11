/* This file has been prepared for Doxygen automatic documentation generation.*/
/*! \file *********************************************************************
*
* \brief
*    board support package-- watch dog driver
* \author
*    zhou_sm(at)blackants.com.cn
* \date
*    2013-01-19
* \version
*    v1.0
* Copyright (c) 2010-2014, Blackants Energy Saving Technology co., LTD All rights reserved.
******************************************************************************/
#ifndef _BSP_UID_H
#define _BSP_UID_H

#ifdef HAVE_UID

#include "stm8l15x.h"

/*! \brief
*       watch dog initilization
* \param init[IN]        watch dog reset time.
*/
void BSP_UID_Read(u8 *uid);

#endif

#endif