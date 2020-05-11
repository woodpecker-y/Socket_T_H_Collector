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

#ifndef _STM8L151_EEPROM_H
#define _STM8L151_EEPROM_H

#ifdef HAVE_EEPROM

#include "stm8l15x.h"

/*! \brief
*       eeprom initilization
*/
void BSP_EEPROM_Init(void);

/*! \brief
*       eeprom write one byte
* \param addr[IN]       - dest address to be written
* \param dat[IN]        - data to be written
*/
void BSP_EEPROM_WriteByte(u32 addr, u8 dat);

/*! \brief
*       eeprom read one byte
* \param addr[IN]       - dest address to be read
* \return
*       readed data
*/
u8  BSP_EEPROM_ReadByte(u32 addr);

/*! \brief
*       eeprom close
* \note
*       it will be used when the function 'BSP_EEPROM_WriteByte' is used.
*/
void BSP_EEPROM_Close(void);

#endif

#endif