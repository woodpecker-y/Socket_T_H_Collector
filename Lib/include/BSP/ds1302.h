 //**************************************************************************//
//*公司名称：  德美技术有限公司
//*所用产品：  XMEGA_DS1302
//*模块名称：  DS1302.h
//*功能描述：  DS1302相关函数
//*说明    ：  包括DS1302初始化，读取等
//*制作人  ：  
//*日期    ：  20010-05-13
//*版本    ：  1.0V
//*********************************************************************/ 

#ifndef __DS1302_H__
#define __DS1302_H__	

#ifdef HAVE_DS1302

#include "bsp.h"

#define AM(X)	X
#define PM(X)	(X+12)            					// 转成24小时制

#define DS1302_SECOND	0x80
#define DS1302_MINUTE	0x82
#define DS1302_HOUR		0x84 
#define DS1302_WEEK		0x8A
#define DS1302_DAY		0x86
#define DS1302_MONTH	0x88
#define DS1302_YEAR		0x8C
#define DS1302_RAM(X)	(0xC0+(X)*2)   			//用于计算 DS1302_RAM 地址的宏 

//-------------------------------------常数宏---------------------------------// 
#define DS1302_SECOND_WRITE		0x80          //写时钟芯片的寄存器位置 
#define DS1302_MINUTE_WRITE		0x82 
#define DS1302_HOUR_WRITE		0x84  
#define DS1302_WEEK_WRITE		0x8A 
#define DS1302_DAY_WRITE		0x86 
#define DS1302_MONTH_WRITE		0x88 
#define DS1302_YEAR_WRITE		0x8C  

#define DS1302_SECOND_READ		0x81          //读时钟芯片的寄存器位置 
#define DS1302_MINUTE_READ		0x83 
#define DS1302_HOUR_READ		0x85  
#define DS1302_WEEK_READ		0x8B 
#define DS1302_DAY_READ			0x87 
#define DS1302_MONTH_READ		0x89 
#define DS1302_YEAR_READ		0x8D  	

/*! \brief 时钟初始化 */
void BSP_RTC_Init(void);				

/*! \brief 写时钟 */
void BSP_RTC_Write(struct tm t);

/*! \brief 读时钟 */
void BSP_RTC_Read(struct tm *t);

#endif

#endif