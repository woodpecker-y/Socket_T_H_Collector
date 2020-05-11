#ifndef _CHECK_H
#define _CHECK_H

extern const unsigned short crc_tab16[256];

unsigned char check_sum(unsigned char *dat, unsigned char len);

unsigned short crc_16_modbus(const unsigned char *data, unsigned short len);

unsigned short crc_ccitt_16(unsigned char *data, unsigned short len);

#endif
