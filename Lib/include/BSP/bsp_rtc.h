/* This file has been prepared for Doxygen automatic documentation generation.*/
/*! \file *********************************************************************
*
* \brief
*    board support package-- rtc driver
* \author
*    zhou_sm(at)blackants.com.cn
* \date
*    2013-01-19
* \version
*    v1.0
* Copyright (c) 2010-2014, Blackants Energy Saving Technology co., LTD All rights reserved.
******************************************************************************/
#ifndef _BSP_RTC_H
#define _BSP_RTC_H

#ifdef HAVE_RTC

typedef unsigned long time_t;

//#include <time.h>

// ʱ�ӽṹ
struct tm{
    u8 tm_sec;      /* seconds after the minute - [0,59] */
    u8 tm_min;      /* minutes after the hour - [0,59] */
    u8 tm_hour;     /* hours since midnight - [0,23] */
    u8 tm_mday;     /* day of the month - [1,31] */
    u8 tm_mon;      /* months since January - [0,11] */
    u8 tm_year;     /* years since 1900 */
    u8 tm_wday;     /* days since Sunday - [0,6] */
    //u8 tm_yday;   /* days since January 1 - [0,365] */
    u16 tm_isdst;   /* daylight savings time flag */
};

/*! \brief
*       rtc initilization
*/
void BSP_RTC_Init(void);


/*! \brief
*       rtc read time with tm format
* \param t[OUT]         - time format named "tm"
*/
void BSP_RTC_Read(struct tm *t);

/*! \brief
*       rtc write time with tm format
* \param t[IN]         - time format named "tm"
*/
void BSP_RTC_Write(struct tm t);

#endif

#endif