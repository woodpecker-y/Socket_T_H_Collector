#ifdef HAVE_DEVMGR

#ifndef _DEV_MGR_H
#define _DEV_MGR_H

#define CMD_AT_PREFIX				"AT"
#define CMD_AT_PREFIX_DLAD			"AT+DLAD"
#define CMD_AT_PREFIX_LOGIN			"AT+LOGIN="
#define CMD_AT_PREFIX_MACID_SET		"AT+MACID="
#define CMD_AT_PREFIX_MACID_GET		"AT+MACID?"

typedef enum _DevMgrType{
	E_DEVMGR_TYPE_AT,
	E_DEVMGR_TYPE_DLAD,
	E_DEVMGR_TYPE_LOGIN,
	E_DEVMGR_TYPE_MACID_SET,
	E_DEVMGR_TYPE_MACID_GET,
	E_DEVMGR_TYPE_UNKNOWN,
}DevMgrType;

typedef enum _DevMgrRet{
	E_DEVMGR_OK,
	E_DEVMGR_ERR_INVALID_USERNAME_OR_PASSWD,
	E_DEVMGR_ERR_INVALID_PACKAGE,
	E_DEVMGR_ERR_NO_ENOUGH_MEMORY,
}DevMgrRet;

typedef enum _DevMgrResult{
	E_DEVMGR_RES_SUCCESS,
	E_DEVMGR_RES_FAILED,
}DevMgrResult;

void dev_mgr_response(char *pack_data, int pack_size, char *body_data, int body_size);

unsigned char dev_mgr_unpack(unsigned char *type, char *body_data, int body_len, char *pack_data, int pack_size);

unsigned char dev_mgr_pack(unsigned char type, unsigned char *body_data, unsigned char body_len, unsigned char *pak_data, unsigned char *pack_size);

unsigned char dev_mgr_request_login(char *name, char *pass, char *body_data);

unsigned char dev_mgr_request_set_macid(unsigned char *macid, unsigned char len, char *body_data);

unsigned char dev_mgr_response_get_macid(char *pack_data, int pack_size, unsigned char *macid, unsigned char len);

#endif

#endif
