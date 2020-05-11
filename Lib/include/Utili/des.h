#ifndef _DES_H
#define _DES_H

typedef enum _DES_MODE{
    DES_ENCRYPT = 0,        // º”√‹
    DES_DESCRYPT,           // Ω‚√‹
}DES_MODE;

void des(unsigned char *plain_strng, unsigned char *key, unsigned char d, unsigned char *ciph_strng);

#endif
