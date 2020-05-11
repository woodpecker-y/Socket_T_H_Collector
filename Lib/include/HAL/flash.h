#ifdef HAVE_FLASH

#ifndef _FLASH_H
#define _FLASH_H

/*! \brief
*       写芯片内部FLASH
* \param addr[IN]			- 写首地址
* \param dat[IN]			- 数据
* \param len[IN]			- 数据长度
* \return
*		E_FLASH_OK						- 成功
*		E_FLASH_ERR_INCORRECT_LENGTH	- 长度不正确
*		E_FLASH_ERR_WRITE_UNCOMPLETE	- 写入失败
* \notes
*		因flash写入的最小长度为2Bytes，因此长度必须是2的整数倍；
*/
u8 flash_write(u32 addr, u8* dat, u32 len);

/*! \brief
*       读芯片内部FLASH
* \param addr[IN]			- 读首地址
* \param dat[IN]			- 数据
* \param len[IN]			- 数据长度
* \return
*		E_FLASH_OK						- 成功
*/
u8 flash_read(u32 addr, u8* dat, u32 len);

/*! \brief
*       擦除FLASH
* \param addr[IN]			- 写首地址
* \param count[IN]			- 数据
* \return
*		E_FLASH_OK					- 成功
*		E_FLASH_ERR_ERASE_FAILED	- 擦除失败
*/
u8 flash_erase_sector(u32 addr, u32 count);

#endif

#endif
