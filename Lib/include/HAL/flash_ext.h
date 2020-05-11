#ifdef HAVE_FLASH_EXT

#ifndef _FLASH_EXT_H
#define _FLAHS_EXT_H

#include "bsp.h"
#include "bsp_flash_ext.h"

/*! \brief
*       记载FLASH配置参数
* \param cfg[IN]			- flash配置参数
*/
void flash_ext_load_config(FlashConfig *cfg);

/*! \brief
*       初始化Flash
*/
void flash_ext_init(void);

/*! \brief
*       读Flash类型ID
* \return
*		value						- 返回Flash类型码
*		- E_FLASH_TYPE_M25PE16		- ST 2M
*		- E_FLASH_TYPE_M25PE32		- ST 4M
*		- E_FLASH_TYPE_M25PE64		- ST 8M
*		- E_FLASH_TYPE_W25Q40BV		- Winbond 512K
*		- E_FLASH_TYPE_W25Q32FV		- Winbond 4M
*		- E_FLASH_TYPE_W25Q64FV		- Winbond 8M
*		- E_FLASH_TYPE_W25Q128FV	- Winbond 16M		
*		- E_FLASH_TYPE_GD25Q16B		- GD 2M
*		- E_FLASH_TYPE_GD25Q32B		- GD 4M
*/
u32 flash_ext_read_id(void);

/*! \brief
*       获得芯片的最大页数
* \return
*		pages count
*/
u32 flash_ext_get_max_phy_pages(void);

/*! \brief
*       向Flash写入字节数据
* \param addr[IN]			- 地址码		
* \param dat[IN]			- 数据
* \param len[IN]			- 数据长度
* \return
*		Writen Bytes Count	- 已写入的字节数
*			>0		- 写入成功
*			=0		- 写入失败
* \notes
*		1.该函数写入时不擦除扇区，需要程序员手工擦除
*/
u32 flash_ext_write(u32 addr, u8 *dat, u32 len);

/*! \brief
*       从Flash读取字节数据
* \param addr[IN]			- 起始地址	
* \param dat[OUT]			- 数据
* \param len[IN]			- 长度
* \return
*		已读取的数据长度	
*			>0		- 读取成功
*			=0		- 读取失败
*/
u32 flash_ext_read(u32 addr, u8 *dat, u32 len);

/*! \brief
*       擦除整个FLASH芯片的数据
* \param page[IN]		- 页地址		
*/
void flash_ext_erase_chip(void);

/*! \brief
*       擦除4K数据
* \param page[IN]		- 页地址		
*/
void flash_ext_erase_4k(u32 page);

/*! \brief
*       擦除多个4K数据
* \param addr[IN]		- 地址
* \param size[IN]		- 大小
*/
void flash_ext_erase_multi_4k(u32 addr, u32 size);

#endif

#endif
