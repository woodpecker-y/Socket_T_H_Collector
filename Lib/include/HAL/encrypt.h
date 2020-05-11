#ifndef _ENCRYPT_H
#define _ENCRYPT_H

#ifdef HAVE_CHIPVERIFY

//#include "stm8l15x_bsp.h"

void encrypt(unsigned char* v, unsigned char v_len, unsigned char* k, unsigned char k_len);

#endif

#endif