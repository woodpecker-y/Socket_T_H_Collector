/* This file has been prepared for Doxygen automatic documentation generation.*/
/*! \file *********************************************************************
*
* \brief
*    hardware abstract layer--rtc sample driver
* \author
*    zhou_sm(at)blackants.com.cn
* \date
*    2014-07-13
* \version
*    v1.0
* \note
*    调用流程:
*       |-->rtc_init()
*           |-->rtc_read()
*           |-->rtc_write()
*           |-->rtc_read_bcd()
*           |-->rtc_write_bcd()
* Copyright (c) 2010-2013, Blackants Energy Saving Technology co., LTD All rights reserved.
******************************************************************************/
#ifndef _RTC_H
#define _RTC_H

#include "bsp.h"
#ifdef HAVE_RTC

#define FMT_DATETIME    0x00
#define FMT_DATE        0x01
#define FMT_TIME        0x02

/*! \brief
*       rtc initilization
* \return
*       none
*/
void rtc_init(void);

/*! \brief
*       rtc read time of the format of struct tm
* \param t[IN]        - time of the format of struct tm
* \return
*       none
*/
void rtc_read(struct tm *t);

/*! \brief
*       rtc write time of the format of struct tm
* \param t[IN]        - time of the format of struct tm
* \return
*       none
*/
void rtc_write(struct tm t);

/*! \brief
*       rtc read time of the format of bcd bytes
* \param bcd_datetime[OUT]        - time of the format of struct tm
* \param len[IN]                 - the length of bcd bytes array 
* \return
*       none
*/
void rtc_read_bcd(unsigned char *bcd_datetime, unsigned char len);

/*! \brief
*       rtc write time of the format of bcd bytes
* \param bcd_datetime[IN]        - time of the format of struct tm
* \param len[IN]                 - the length of bcd bytes array 
* \return
*       none
*/
void rtc_write_bcd(unsigned char *bcd_datetime, unsigned char len);

#endif

#endif
