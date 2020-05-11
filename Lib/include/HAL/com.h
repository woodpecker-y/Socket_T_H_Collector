/* This file has been prepared for Doxygen automatic documentation generation.*/
/*! \file *********************************************************************
*
* \brief
*    hardware abstract layer--Com sample driver
* \author
*    zhou_sm(at)blackants.com.cn
* \date
*    2014-07-13
* \version
*    v1.0
* \note
*    调用流程:
*       |-->com_load_config()
*           |-->com_init()
*               |-->com_send_char()
*               |-->com_send_data()
*               |-->com_send_str()
*               |-->com_recv_init()
*               |-->com_recv_data()
*           |-->com_close()
*       |-->INTERRUPT_HANDLER(USART1_RX_TIM5_CC_IRQHandler,28)
*           |-->com_isr()
*
* Copyright (c) 2010-2013, Blackants Energy Saving Technology co., LTD All rights reserved.
******************************************************************************/
#ifndef _COM_H
#define _COM_H

#include "bsp.h"


#ifdef HAVE_COM
/*! \brief
*       com confirure handler
* \param com_cfg[IN]        - 串口配置结构体
* \param com_cfg_cnt[IN]	- 串口配置数量		
*/
void com_load_config(COMConfig *com_cfg, uint8_t cnt);

/*! \brief
*       com confirure handler
* \param mode[IN]        - 设置串口接收引脚模式，默认为上拉输入	
*/
void com_set_rx_pin_mode(u8 mode);

/*! \brief
*       串口初始化
* \param com[IN]					- 串口号
* \param baudrate[IN]				- 波特率
* \param wordlength[IN]				- 数据位
* \param stopbits[IN]				- 停止位
* \param mode[IN]					- 工作模式
* \param rx_interrupt_state[IN]		- 接收中断状态
* \param rx_interrupt_prority[IN]	- 接收中断优先级
* \return
*		E_COM_OK						- 成功
*		E_COM_ERR_NOT_DEFINED_COM_ID	- 未定义的串口
*		E_COM_ERR_NOT_SUPPORT_COM		- 不支持的串口
*/
uint8_t com_init(COM com, uint32_t baudrate, uint16_t wordlength, uint16_t stopbits, uint16_t parity, uint16_t mode, uint8_t rx_interrupt_state,	uint8_t  rx_interrupt_prority);

/*! \brief
*       串口关闭
* \param com[IN]					- 串口号
* \return
*		E_COM_OK			        - 成功
*		E_COM_ERR_NOT_DEFINED_COM_ID	        - 未定义的串口
*		E_COM_ERR_NOT_SUPPORT_COM		- 不支持的串口
*/
uint8_t com_close(COM com);

/*! \brief
*       串口接收一个字节数据
* \param com[IN]					- 串口号
* \return
*   一个字节数据
* \notes
*   使用前必须先调用com_get_rxflag,且只有其返回值为SET值，才可以调用本函数
*/
eCOMRet com_recv_char(COM com, uint16_t *dat);

/*! \brief
*       串口初始化
* \param com[IN]					- 串口号
* \param ch[IN]						- 一个字节
* \return
*		E_COM_OK						- 成功
*		E_COM_ERR_NOT_DEFINED_COM_ID	- 未定义的串口
*		E_COM_ERR_NOT_SUPPORT_COM		- 不支持的串口
*/
uint8_t com_send_char(COM com, uint8_t ch);

/*! \brief
*       串口初始化
* \param com[IN]					- 串口号
* \param dat[IN]					- 字节数据
* \param len[IN]					- 字节数据长度
* \return
*		E_COM_OK						- 成功
*		E_COM_ERR_NOT_DEFINED_COM_ID	- 未定义的串口
*		E_COM_ERR_NOT_SUPPORT_COM		- 不支持的串口
*/
uint8_t com_send_data(COM com, uint8_t *dat, uint16_t len);

/*! \brief
*       串口初始化
* \param com[IN]					- 串口号
* \param str[IN]					- 字符串
* \return
*		E_COM_OK						- 成功
*		E_COM_ERR_NOT_DEFINED_COM_ID	- 未定义的串口
*		E_COM_ERR_NOT_SUPPORT_COM		- 不支持的串口
*/
uint8_t com_send_str(COM com, char *str);

/*! \brief
*       串口接收初始化
* \param com[IN]					- 串口号
* \return
*		E_COM_OK						- 成功
*		E_COM_ERR_NOT_DEFINED_COM_ID	- 未定义的串口
*/
uint8_t com_recv_init(COM com);

/*! \brief
*       串口接收数据
* \param com[IN]						- 串口号
* \param dat[OUT]						- 数据接收指针
* \param len[OUT]						- 数据的长度
* \return
*		E_COM_OK						- 成功
*		E_COM_ERR_NOT_DEFINED_COM_ID	- 未定义的串口
* \note
*		参数ptr是一个二级指针，在使用时不需要为ptr分配内存空间，定义一个空指针即可，输出时返回驱动层接收数据缓冲区的首地址。
*		使用案例：
*			uint8_t *rcv_ptr = NULL;
*			uint16_t rcv_len = 0;
*			
*			BSP_COM_RecvDataPtr(COM1, &rcv_ptr, &rcv_len);
*/
uint8_t com_recv_data(COM com, uint8_t **dat, uint16_t *len);

/*! \brief
*       uart read data from recv data buffer.
* \param com[IN]            uart com no.
* \param pval[OUT]          data ptr point to recv data buffer.
* \param len[OUT]           length of recved data.
* \return
*       >0          read data successfully.
*       =0          no data
*/
u8   com_recv_data_stream(COM com, u8 **pval, u16 *len);

/*! \brief
*	com interrupt service runtine
* \param com[IN]            - com no.
*/
void com_isr(COM com);

/**  @brief   com1ía2??D????D??óê?oˉêy
  *  @param   rcv_buf [OUT]     - ?óê??o′?
  *  @param   rcv_len [OUT]     - ?óê?êy?Y3¤?è
  *
  *  @retval  none
  */
u8 uart_lpr_rcv(COM com, u8 **rcv_buf, u16 *rcv_len);

#endif

#endif
