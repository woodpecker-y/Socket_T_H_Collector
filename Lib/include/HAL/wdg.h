/* This file has been prepared for Doxygen automatic documentation generation.*/
/*! \file *********************************************************************
*
* \brief
*    hardware abstract layer--watch dog driver
* \author
*    zhou_sm(at)blackants.com.cn
* \date
*    2014-07-13
* \version
*    v1.0
* \note
*    调用流程:
*       |-->wdg_init()
*           |-->wdg_reset()
*           |-->wdg_reset()
*           |-->wdg_reset()
* Copyright (c) 2010-2013, Blackants Energy Saving Technology co., LTD All rights reserved.
******************************************************************************/

#ifndef _WDG_H
#define _WDG_H

#ifdef HAVE_WDG

/*! \brief
*       内部看门狗初始化
* \param _1s[IN]			- 看门狗溢出时间
*/
void wdg_init(u32 _1ms);

/*! \brief
*       内部看门狗喂狗
*/
void wdg_reset(void);

#endif

#endif
