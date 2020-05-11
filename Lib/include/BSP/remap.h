/* This file has been prepared for Doxygen automatic documentation generation.*/
/*! \file *********************************************************************
 * 
 * \brief 
 *    �弶��������--��ӳ��
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

// ���������ض���
#define GPIO_SetBits			GPIO_WriteHigh
#define GPIO_ResetBits		        GPIO_WriteLow
#define GPIO_ReadInputDataBit	        GPIO_ReadInputPin

// ���������ض���
#define EXTI_Pin_TypeDef                
#endif

#endif