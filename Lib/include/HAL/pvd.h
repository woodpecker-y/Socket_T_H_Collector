/* This file has been prepared for Doxygen automatic documentation generation.*/
/*! \file *********************************************************************
*
* \brief
*    board support package--pvd driver
* \author
*    zhou_sm(at)blackants.com.cn
* \date
*    2014-09-16
* \version
*    v1.0
* \Note
* 	????��?����??
*		1.?????????��?��????????????pvd_init
*		2.?????????��PVD??????��?????????��????????��????????��???����??
* 		void PVD_IRQHandler(void)
*		{
*		}
*       3.????????????????????��???����????????����????1???��??????????????????��?
* Copyright (c) 2010-2014, Blackants Energy Saving Technology co., LTD All rights reserved.
******************************************************************************/

#ifndef _PVD_H
#define _PVD_H

/*! \brief
*       PVD Init
*/
void pvd_init(void);

#endif