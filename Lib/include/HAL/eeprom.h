/* This file has been prepared for Doxygen automatic documentation generation.*/
/*! \file *********************************************************************
*
* \brief
*    hardware abstract layer--eeprom driver
* \author
*    zhou_sm(at)blackants.com.cn
* \date
*    2014-07-16
* \version
*    v1.0
* \note
*    调用流程:
*       写EEPROM必须初始化
*       |-->eeprom_init()
*           |-->eeprom_write_byte()
*           |-->eeprom_write()
*       |-->eeprom_close()
*       读EEPROM可以直接读
*       |-->eeprom_read_byte()
*       |-->eeprom_read()
* Copyright (c) 2010-2014, Blackants Energy Saving Technology co., LTD All rights reserved.
******************************************************************************/
#ifndef _EEPROM_H
#define _EEPROM_H

#ifdef HAVE_EEPROM

#include "bsp.h"


/*! \brief
*       eeprom initilization
*/
void eeprom_init(void);

/*! \brief
*       eeprom write one byte
* \param addr[IN]       - dest address to be written
* \param dat[IN]        - data to be written
*/
void eeprom_write_byte(u32 addr, u8 byte);

/*! \brief
*       eeprom read one byte
* \param addr[IN]       - dest address to be read
* \return
*       readed data
*/
u8   eeprom_read_byte(u32 addr);

/*! \brief
*       eeprom write multiply bytes
* \param addr[IN]       - dest address to be written
* \param dat[IN]        - data to be written
* \param len[IN]        - length of data
*/
void eeprom_write(u32 addr, u8 *dat, u8 len);

/*! \brief
*       eeprom read multiply bytes
* \param addr[IN]       - dest address to be written
* \param dat[OUT]       - data buffer
* \param len[IN]        - length will be readed.
*/
void eeprom_read(u32 addr, u8 *dat, u8 len);

/*! \brief
*       eeprom close
* \note
*       it will be used when the function 'eeprom_write' or 'eeprom_write_byte' is used.
*/
void eeprom_close(void);

#endif

#endif
