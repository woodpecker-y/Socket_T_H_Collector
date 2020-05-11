/* This file has been prepared for Doxygen automatic documentation generation.*/
/*! \file *********************************************************************
*
* \brief
*    board support package--BA6289 motor driver
* \author
*    zhou_sm(at)blackants.com.cn
* \date
*    2013-01-19
* \version
*    v1.0
* \notes
*    A6模块使用指南
*       1.在调用gprs_init接口之前必须先调用com_load_config()函数加载对应串口的配置;
*       2.gprs_get_rssi()函数在gprs_check_is_ready调用后，gprs_close调用前可随意使用；
*       3.如果需要连续接收多次数据，请在数据接收前调用gprs_recv_init()函数清除接收缓冲区，在发送报文后仅接收一次则不需要调用；
*       4.调用流程：
*       |-->gprs_load_config()
*           |-->gprs_power_on()
*               |-->gprs_init()
*                   |-->gprs_check_is_ready()
*                   |-->gprs_get_rssi()
*                       |-->gprs_ip_connect()   
*                           |-->gprs_send_data() 
*                           |-->gprs_recv_data() 
*                           |-->gprs_recv_init() 
*                           |-->gprs_recv_data() 
*                           |-->......
*                       |-->gprs_ip_close() 
*               |-->gprs_close()
*           |-->gprs_power_off()
* Copyright (c) 2010-2013, Blackants Energy Saving Technology co., LTD All rights reserved.
******************************************************************************/
#ifndef _GPRS_H
#define _GPRS_H

#ifdef HAVE_GPRS

/*! \brief
*       加载A6模块配置
* \param handler[IN]        - gprs config handler
*/
void gprs_load_config(GPRSConfig *handler);

/*! \brief
*       给A6模块上电
*/
void gprs_power_on(void);

/*! \brief
*       给A6模块断电
*/
void gprs_power_off(void);

/*! \brief
*       发送数据到服务器
* \param dat[IN]           - 数据
* \param len[IN]           - 数据长度
*
* \return
*       E_GPRS_OK           - 成功
*       E_GPRS_PROCESSING   - 处理中
*       E_GPRS_ERR_OTHERS   - 错误
*/
u8 gprs_transport_send_data(u8* dat, u16 len);

/*! \brief
*       接收从服务器下发的数据
* \param msg[OUT]           - 消息指针
* \param size[OUT]          - 数据长度
*
* \return
*       E_GPRS_OK           - 成功
*       E_GPRS_ERR_NO_DATA  - 暂无数据
*       E_GPRS_ERR_OTHERS   - 错误
* \notes
*       msg为二级指针，不需要为它分配内存缓冲，它直接指向串口接收缓冲区，在调用gprs_recv_init()或gprs_send_data()函数之前，一直可用。
*/
u8 gprs_transport_recv_data(u8** msg, u16 *size);

/*! \brief
*       A6模块配置初始化
* \param baudrate[IN]       - A6模块串口波特率
* \param wordlength[IN]     - A6模块串口数据位
* \param stopbits[IN]       - A6模块串口停止位
* \param parity[IN]         - A6模块串口校验位
*
* \return
*       E_GPRS_OK     - 成功
*       Others          - 失败
*/
u8 gprs_init(u32 baudrate, u8 wordlength, u8 stopbits, u8 parity);

/*! \brief
*       释放A6模块
*/
void gprs_close(void);

/*! \brief
*       GPRS模块数据接收初始化
*/
void gprs_recv_init(void);

/*! \brief
*       检查GPRS模块是否已准备好联网
* \return
*       E_GPRS_OK               - 成功
*       E_GPRS_PROCESSING       - 处理中
*       E_GPRS_ERR_NO_SIM       - 无SIM卡
*       E_GPRS_ERR_CONFIG_PDP   - 配置PDP参数失败
*       E_GPRS_ERR_ACTIVE_PDP   - 激活PDP参数失败
*       E_GPRS_ERR_ATTACHED_NETWORK - 附着网络失败
*/
u8 gprs_check_is_ready(void);

/*! \brief
*       GPRS连接远程服务器
* \param server[IN]           - 服务器：支持域名或IP
* \param port[IN]             - 端口
*
* \return
*       E_GPRS_OK           - 成功
*       E_GPRS_PROCESSING   - 处理中
*       E_GPRS_ERR_OTHERS   - 错误
*/
u8 gprs_ip_connect(char* server, int port);

/*! \brief
*       GPRS关闭链接
* \return
*       E_GPRS_OK           - 成功
*       E_GPRS_PROCESSING   - 处理中
*       E_GPRS_ERR_OTHERS   - 错误
*/
u8 gprs_ip_close(void);

/*! \brief
*       读取信号值
* \param rssi[OUT]           - 信号值
*
* \return
*       E_GPRS_OK           - 成功
*       E_GPRS_PROCESSING   - 处理中
*       E_GPRS_ERR_OTHERS   - 错误
*/
u8 gprs_get_rssi(u8 *rssi);

/*! \brief
*       发送数据到服务器
* \param dat[IN]           - 数据
* \param len[IN]           - 数据长度
*
* \return
*       E_GPRS_OK           - 成功
*       E_GPRS_PROCESSING   - 处理中
*       E_GPRS_ERR_OTHERS   - 错误
*/
u8 gprs_send_data(u8* dat, u16 len);

/*! \brief
*       接收从服务器下发的数据
* \param msg[OUT]           - 消息指针
* \param size[OUT]          - 数据长度
*
* \return
*       E_GPRS_OK           - 成功
*       E_GPRS_ERR_NO_DATA  - 暂无数据
*       E_GPRS_ERR_OTHERS   - 错误
* \notes
*       msg为二级指针，不需要为它分配内存缓冲，它直接指向串口接收缓冲区，在调用gprs_recv_init()或gprs_send_data()函数之前，一直可用。
*/
u8 gprs_recv_data(u8** msg, u16 *size);

u8 gprs_set_ipmode(u8 mode);

u8 gprs_exit_transport_mode();

void gprs_reset_status(void);

#endif
#endif