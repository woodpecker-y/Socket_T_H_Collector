#ifndef _BSP_FLASH_EXT_H
#define _BSP_FLASH_EXT_H

#ifdef HAVE_FLASH_EXT

#include "bsp_gpio.h"

/*! \brief 外部FLASH一页的字节数 */
#define FLASH_EXT_PAGE_SIZE			256

/*! \brief 外部FLASH命令码定义 */
#define FLASH_INS_WRITE_ENABLE     	    	0x06
#define FLASH_INS_WRITE_DISABLE     	    0x04
#define FLASH_INS_READ_ID		     	    0x9f
#define FLASH_INS_READ_STATUS_REG     	    0x05
#define FLASH_INS_WRITE_STATUS_REG     	    0x01
#define FLASH_INS_READ_DATA		     	    0x03
#define FLASH_INS_PAGE_PROGRAM       	    0x02
#define FLASH_INS_BLOCK_ERASE_4K      	    0x20
#define FLASH_INS_CHIP_ERASE				0xC7
#define FLASH_INS_POWER_DOWN	       	    0xB9
#define FLASH_INS_RELEASE_POWER_DOWN   	    0xAB

/*! \brief Flash配置结构体 */
typedef struct _eFlashConfig{
	PortTypeDef		nss;					/*!< \brief NSS */
	PortTypeDef		sck;					/*!< \brief SCK */
	PortTypeDef		mosi;					/*!< \brief MOSI */
	PortTypeDef		miso;					/*!< \brief MISO */
}FlashConfig;

/*! \brief Flash类型定义 */
typedef enum _eFlashType{
	E_FLASH_TYPE_M25PE16	= 0x208015,		/*!< \brief ST 2M */
	E_FLASH_TYPE_M25PE32	= 0x202016,		/*!< \brief ST 4M */
	E_FLASH_TYPE_M25PE64	= 0x202017,		/*!< \brief ST 8M */
	E_FLASH_TYPE_W25Q40BV	= 0xEF4015,		/*!< \brief Winbond 512k */
	E_FLASH_TYPE_W25Q32FV	= 0xEF4016,		/*!< \brief Winbond 4M */
	E_FLASH_TYPE_W25Q64FV	= 0xEF4017,		/*!< \brief Winbond 8M */
	E_FLASH_TYPE_W25Q128FV	= 0xEF4018,		/*!< \brief Winbond 16M */
	E_FLASH_TYPE_GD25Q16B	= 0xC84015,		/*!< \brief Winbond 2M */
	E_FLASH_TYPE_GD25Q32B	= 0xC84016,		/*!< \brief Winbond 4M */
}eFlashType;

/*! \brief
*       记载FLASH配置参数
* \param cfg[IN]			- flash配置参数
*/
void BSP_FLASH_EXT_LoadConfig(FlashConfig *cfg);

/*! \brief
*       初始化Flash
*/
void BSP_FLASH_EXT_Init(void);

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
u32 BSP_FLASH_EXT_ReadID(void);

/*! \brief
*       获得芯片的最大页数
* \return
*		pages count
*/
u32 BSP_FLASH_EXT_GetMaxPhyPages(void);

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
*		1.该函数最大支持写1页数据，即256字节；
*		2.该函数不支持跨页写数据，如果从中间开始写的话，数据长度需要使用256-Addr%256计算能够写入的长度，否则会写入失败，反馈0.
*/
u32 BSP_FLASH_EXT_WriteBytes(u32 addr, u8 *dat, u32 len);

/*! \brief
*       向Flash写入字节数据
* \param page_start[IN]		- 起始页		
* \param pages[IN]			- 写入的页数
* \param dat[IN]			- 数据
* \param len[IN]			- 长度
* \return
*		Writen Pages Count	- 已写入数据的页数
* \notes
*		1.一页数据的长度是256字节，因此要写入的数据长度必须是pages*256大小；
*/
uint32_t BSP_FLASH_EXT_Write_MultiPages_Data(u32 page_start, u32 pages, u8 *dat, u32 len);

/*! \brief
*       从Flash读取字节数据
* \param addr[IN]			- 起始地址	
* \param dat[OUT]			- 数据
* \param len[IN]			- 长度
* \return
*		已读取的数据长度	
*			>0		- 读取成功
*			=0		- 读取失败
* \notes
*		1.该函数最大支持读1页数据，即256字节；
*		2.该函数不支持跨页读数据，如果从中间开始写的话，数据长度需要使用256-Addr%256计算能够读取的长度，否则会读取失败，反馈0.
*/
u32 BSP_FLASH_EXT_ReadBytes(u32 addr, u8 *dat, u32 len);

/*! \brief
*       从Flash读取字节数据
* \param page_start[IN]		- 起始页	
* \param pages[OUT]			- 页数
* \param dat[IN]			- 数据
* \param len[IN]			- 长度
* \return
*		已读取的数据页数	
* \notes
*		1.一页数据的长度是256字节，因此要读取的数据长度必须是pages*256大小，数据缓冲区必须定义好大小
*/
u32 BSP_FLASH_EXT_Read_MultiPages_Data(u32 page_start, u32 pages, u8 *dat, u32 len);

/*! \brief
*       擦除4K数据
* \param page[IN]		- 页地址		
*/
void BSP_FLASH_EXT_Erase4K(u32 page);

/*! \brief
*       擦除整个FLASH芯片	
*/
void BSP_FLASH_EXT_EraseChip(void);

#endif

#endif

