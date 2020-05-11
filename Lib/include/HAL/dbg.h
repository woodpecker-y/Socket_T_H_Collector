/* This file has been prepared for Doxygen automatic documentation generation.*/
/*! \file *********************************************************************
* 
* \brief 
*    调试接口
* \author 
*    zhou_shimin(at)demeisol.com 
* \date 
*    2011-09-05
* \version 
*    v1.0
* \note
*    调用流程:
*       |-->MYLOG_WARN()
*       |-->MYLOG_ERROR()
*       |-->MYLOG_TRACE()
*       |-->MYLOG_INFO()
*       |-->MYLOG_DEBUG()
*       |-->MYLOG_DEBUG_HEXDUMP()
* Copyright (c) 2011, Demei Corporation All rights reserved.
******************************************************************************/
#ifndef _DEBUG_H
#define _DEBUG_H

#ifdef HAVE_DBG

#include <stdio.h>

/*! \brief 调试级别 */
typedef enum _DebugLevel{
	L_NONE  = 0,	/*!< \brief 关闭 */
	L_ERROR = 1,	/*!< \brief 错误 */
	L_WARN  = 2,	/*!< \brief 警告 */
	L_TRACE = 3,	/*!< \brief 轨迹 */
	L_INFO  = 4,	/*!< \brief 信息 */
	L_DEBUG = 5,	/*!< \brief 调试 */
	L_ALL   = 6	/*!< \brief 全部 */
}DebugLevel;

#define DEBUG_PRINT(x)		printf(x)		

#define MAX_DEBUG_LENGTH	128

#define DEBUG_LEVEL	L_ALL

/*! \brief 输出警告 */
void MYLOG_WARN(const char *fmt, ...);

/*! \brief 输出错误 */
void MYLOG_ERROR(const char *fmt, ...);

/*! \brief 输出轨迹 */
void MYLOG_TRACE(const char *fmt, ...);

/*! \brief 输出信息 */
void MYLOG_INFO(const char *fmt, ...);

/*! \brief 输出调试信息 */
void MYLOG_DEBUG(const char *fmt, ...);

/*! \brief 输出二进制数据 */
void MYLOG_DEBUG_HEXDUMP(char *msg, unsigned char* bytes, unsigned int len);

#endif

#endif
