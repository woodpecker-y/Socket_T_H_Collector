/* This file has been prepared for Doxygen automatic documentation generation.*/
/*! \file *********************************************************************
 *
 * \brief
 *    board support package-- uart driver
 * \author
 *    zhou_sm(at)blackants.com.cn
 * \date
 *    2013-01-19
 * \version
 *    v1.0
 * Copyright (c) 2010-2013, Blackants Energy Saving Technology co., LTD All rights reserved.
 ******************************************************************************/
#ifndef _BSP_COM_H
#define _BSP_COM_H

#ifdef HAVE_COM

#include "stm8l15x.h"
#include "bsp.h"

/*! \brief com index Definition */
typedef enum _COM{
    COM1 = 0,
    COM2,
    COM3,
}COM;

/*! \brief Com Configure Definition */
typedef struct _ComConfig{
    COM          com;                   /*!< \brief COM Object */
    PortTypeDef  tx;                    /*!< \brief COM Tx Pin */
    PortTypeDef  rx;                    /*!< \brief COM Rx Pin */
    PortTypeDef  ctrl;                  /*!< \brief COM Ctrl Pin, Only used with RS485 */
    uint8_t*	 rcv_ptr;               /*!< \brief COM recv buffer poniter */
    uint16_t	 max_rcv_size;          /*!< \brief COM recv buffer max size */
}COMConfig;

/*! \brief 串口接口反馈码 */
typedef enum _eCOMRet{
    E_COM_OK = 0,
    E_COM_ERR_NOT_DEFINED_COM_ID,
    E_COM_ERR_NOT_SUPPORT_COM,
    E_COM_ERR_INPUT_NULL_VALUE,
    E_COM_ERR_NO_DATA,
    E_COM_ERR_NOT_ENOUGH_MEMORY,
}eCOMRet;

/*! \brief
*       com confirure handler
* \param com[IN]        uart com no.
*/
void BSP_COM_LoadConfig(COMConfig *com_cfg, uint8_t cnt);

/*! \brief
*       uart power on
* \param com[IN]        uart com no.
*/
uint8_t BSP_COM_PowerOn(COM com);

/*! \brief
*       uart power off
* \param com[IN]        uart com no.
*/
uint8_t BSP_COM_PowerOff(COM com);

void BSP_COM_SetRxPinMode(u8 mode);

/*! \brief
*       uart initilization
* \param com[IN]                uart com no.
* \param baudrate[IN]           uart baudrate
* \param wordlength[IN]         databits
* \param stopbits[IN]           stopbits
* \param mode[IN]               USART_Mode_Tx or USART_Mode_Rx
* \param rx_interrupt_state[IN] uart recv interrupt enable state
* \return
*       none
* \notes
*       
*/
uint8_t BSP_COM_Init(COM com, u32 baudrate, u8 wordlength, u8 stopbits, u8 parity, u8 mode, u8 rx_interrupt_state, uint8_t  rx_interrupt_prority);

/*! \brief
*       uart close
* \param com[IN]            uart com no.
* \return
*       none
*/
uint8_t BSP_COM_Close(COM com);

/*! \brief
*       uart send one char
* \param com[IN]            uart com no.
* \param ch[IN]             one char
* \return
*       none
*/
uint8_t BSP_COM_SendChar(COM com, u8 ch);

/*! \brief
*       uart send string
* \param com[IN]            uart com no.
* \param str[IN]            string
* \return
*       none
*/
uint8_t BSP_COM_SendStr(COM com, const char *str);

/*! \brief
*       uart send data
* \param com[IN]            uart com no.
* \param val[IN]            bytes array to be sended
* \param len[IN]            size of bytes array
* \return
*       none
*/
uint8_t BSP_COM_SendData(COM com, const u8 *val, u16 len);

/*! \brief
*       uart get recv flag
* \param com[IN]            uart com no.
* \return
*       RESET       - False
*       Others      - True
*/
FlagStatus BSP_COM_GetRxFlag(COM com);

/*! \brief
*       uart get recv one char
* \param com[IN]            uart com no.
* \return
*       value       - one char
*/
eCOMRet BSP_COM_RecvChar(COM com, uint16_t *dat);

/*! \brief
*       uart recv initilization
* \param com[IN]            uart com no.
* \note
*       1.clear data buffer;
*       2.make size of data already recved to 0
*       3.make flag to be UART_RX_FLAG_RECVING.
* \note
*       you must call BSP_COM_RecvInit function before recving data.
*/
uint8_t BSP_COM_RecvInit(COM com);

/*! \brief
*       uart read data from recv data buffer.
* \param com[IN]            uart com no.
* \param pval[OUT]          data ptr point to recv data buffer.
* \param len[OUT]           length of recved data.
* \return
*       =0          read data successfully.
*       <0          no data
*/
u8   BSP_COM_RecvData(COM com, u8 **pval, u16 *len);

/*! \brief
*       uart read data from recv data buffer.
* \param com[IN]            uart com no.
* \param pval[OUT]          data ptr point to recv data buffer.
* \param len[OUT]           length of recved data.
* \return
*       >0          read data successfully.
*       =0          no data
*/
u8   BSP_COM_RecvDataStream(COM com, u8 **pval, u16 *len);

/*! \brief
*       uart interrupt service routine.
* \param com[IN]            uart com no.
* \return
*      none
* \note
*       1.Uart recv interrupt service runtine is valid when rx_interrupt state is enabled
*       2.Uart starts to recv data When recv flag is UART_RX_FLAG_RECVING;
*       3.Uart makes flags to UART_RX_FLAG_RECV_COMPLETE when idle interrupt of uart is triggered.
*/
void BSP_COM_ISR(COM com);

/**  @brief   COM Read Data In Lower Power Mode
*  @param   rcv_buf [OUT]     - ?óê??o′?
*  @param   rcv_len [OUT]     - ?óê?êy?Y3¤?è
*
*  @retval  none
*/
u8 BSP_COM_LprRecvData(COM com, u8 **rcv_buf, u16 *rcv_len);

#endif

#endif
