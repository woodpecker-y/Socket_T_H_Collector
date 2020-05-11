/* This file has been prepared for Doxygen automatic documentation generation.*/
/*! \file *********************************************************************
 * 
 * \brief 
 *    板级驱动程序--重映射
 * \author 
 *    zhou_sm(at)blackants.com.cn
 * \date 
 *    2013-01-19
 * \version 
 *    v1.0
 * Copyright (c) 2010-2013, Blackants Energy Saving Technology co., LTD All rights reserved.
 ******************************************************************************/
#ifndef _REMAP_H
#define _REMAP_H

#ifdef HAVE_STM8S

// 函数名称重定义
#define GPIO_SetBits			GPIO_WriteHigh
#define GPIO_ResetBits		        GPIO_WriteLow
#define GPIO_ReadInputDataBit	        GPIO_ReadInputPin

// 类型名称重定义
#define EXTI_Pin_TypeDef                
#endif

#endif