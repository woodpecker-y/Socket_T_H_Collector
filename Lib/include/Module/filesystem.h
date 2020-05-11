/* This file has been prepared for Doxygen automatic documentation generation.*/
/*! \file *********************************************************************
*
* \brief
*    hardware abstract layer--filesystem driver
* \author
*    zhou_sm(at)blackants.com.cn
* \date
*    2014-09-16
* \version
*    v1.0
* \notes
*       1.文件系统使用前提：必须初始化外部FLASH
*       调用流程：
*       |-->fs_mount()
*           |-->fs_load()
*               |-->fs_read()
*               |-->fs_write()
*               |-->fs_delete()
* Copyright (c) 2010-2014, Blackants Energy Saving Technology co., LTD All rights reserved.
******************************************************************************/
#ifdef HAVE_FILESYSTEM

#ifndef _FILESYSTEM_H
#define _FILESYSTEM_H

#include "bsp.h"

// 最大支持文件数
#define MAX_FILE_CNT	10	

// 文件类型
typedef enum _FileType{
	E_FILE_TYPE_SYSINF,							   // 系统配置文件		
	E_FILE_TYPE_LOG,							   // 日志文件
}FileType;

// 文件信息
typedef struct _FileInfo{
	u32 file_name;		 						   // 文件名称
	u32 file_size;		 						   // 文件大小
	u32 record_size;	   						   // 记录大小	
	s32 pos;									   // 偏移量:读，写偏移量=读偏移量+1
	u8  file_type;								   // 文件类型
}FileInfo;

// 文件系统反馈码
typedef enum _eFSRet{
	E_FS_OK = 0,							  		// 成功
	E_FS_ERR_FILE_NUM_EXCEED_MAX,			   		// 文件数超过最大
	E_FS_ERR_HARDWARE,						   		// 底层硬件接口
	E_FS_ERR_NOT_EXSIT,		  						// 文件不存在
	E_FS_ERR_READ_FAIL,					   			// 读失败
	E_FS_ERR_WRITE_FAIL,							// 写失败
	E_FS_ERR_NO_RECORD,				   				// 无记录
	E_FS_ERR_EXCEED_FILE_SIZE,						// 超出文件大小
	E_FS_ERR_INVALID_PARAMS,						// 非法的参数
	E_FS_ERR_ALREADY_WRITED,						// 文件已写入
	E_FS_ERR_NOT_MATCH_READ_AFTER_WRITE,			// 读取已写入的数据后不匹配
}eFSRet;

/*! \brief
*       加载文件系统
*/
void fs_mount(void);

/*! \brief
*       加载文件到系统
* \param file_name[IN]		- 文件名
* \param file_size[IN]		- 文件大小
* \param file_type[IN]		- 文件类型
* \param record_size[IN]	- 记录大小
* \return
*		E_FS_OK				- 成功
*		E_FS_ERR_READ_FAIL	- 失败
* \notes
*		1）加载文件信息时，必须同时输入文件大小和单条记录大小
*		2）配置文件的最前面4个字节不能为0xFF,0xFF,0xFF,0xFF
*/
u8 fs_load(u8 file_type, u32 file_name, u32 file_size, u32 record_size);

/*! \brief
*       读取文件
* \param file_name[IN]		- 文件名
* \param offset[IN]			- 偏移量
* \param dat[OUT]			- 读取的数据
* \param size[IN]			- 读取的数据长度
* \return
*		E_FS_OK						- 成功
*		E_FS_ERR_NO_RECORD			- 无记录
*		E_FS_ERR_INVALID_PARAMS		- 参数不对
*		E_FS_ERR_READ_FAIL			- 失败
*		E_FS_ERR_EXCEED_FILE_SIZE	- 偏移量超出文件大小
*/
u8 fs_read(u32 file_name, u32 offset, u8 *dat, u32 size);

/*! \brief
*       写数据到文件
* \param file_name[IN]		- 文件名
* \param offset[IN]			- 偏移量
* \param dat[IN]			- 写入的数据
* \param size[IN]			- 写入的数据长度
* \return
*		E_FS_OK						- 成功
*		E_FS_ERR_NOT_EXSIT			- 文件不存在
*		E_FS_ERR_INVALID_PARAMS		- 参数不对
*		E_FS_ERR_WRITE_FAIL			- 写失败
*		E_FS_ERR_EXCEED_FILE_SIZE	- 偏移量超出文件大小
*/
u8 fs_write(u32 file_name, u32 offset, u8 *dat, u32 size);

/*! \brief
*       删除文件
* \param file_name[IN]		- 文件名
* \return
*		E_FS_OK						- 成功
*		E_FS_ERR_NOT_EXSIT			- 文件不存在
*/
u8 fs_delete(u32 file_name);

#endif

#endif
