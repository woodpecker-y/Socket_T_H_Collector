#ifdef HAVE_RFID

#ifndef _RFID_H
#define _RFID_H

#include "bsp.h"

/*! \brief card authenticate mode */
#define KEY_A   0x60    /*!< \brief key A password mode */
#define KEY_B   0x61    /*!< \brief key B password mode */

/*! \brief card value block operate mode */
#define VAL_DEC 0xC0    /*!< \brief value block decrease */
#define VAL_INC 0xC1    /*!< \brief value block increase */

#define CARD_SUPPORTED      0x0400

void rfid_load_config(RFIDConfig *handler);

void rfid_power_on(void);

void rfid_power_off(void);

char rfid_init(u8 *ctrl_reg, u8 *version);

void rfid_close(void);

void rfid_ant_on(void);

void rfid_ant_off(void);

char rfid_request(u8 reg_type, u8 *card_type);

char rfid_anticoll(u8 *sn);

char rfid_select(u8 *sn, u8 *size);

char rfid_auth(u8 auth_mode, u8 addr, u8 *pKey, u8 *pSnr);

char rfid_read(unsigned char addr,unsigned char *pData);

char rfid_write(unsigned char addr,unsigned char *pData);

char rfid_value(unsigned char dd_mode, unsigned char addr, unsigned char *pValue);

char rfid_bakvalue(unsigned char sourceaddr, unsigned char goaladdr);

char rfid_halt(void);

#endif

#endif

