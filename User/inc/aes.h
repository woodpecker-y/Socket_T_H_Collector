#ifdef HAVE_AES128_ECB_PKCS7PADDING

#ifndef _AES_H_
#define _AES_H_

//#include <stdint.h>
#include "stm8l15x.h"

// The #ifndef-guard allows it to be configured before #include'ing or at compile time.

//#define AES_BLOCKLEN 16 // Block length in bytes - AES is 128b block only
//#define AES_KEYLEN 16   // Key length in bytes
#define AES_keyExpSize 176

struct AES_ctx
{
  uint8_t RoundKey[AES_keyExpSize];
};

static void AES_init_ctx(struct AES_ctx* ctx, const uint8_t* key);

// buffer size is exactly AES_BLOCKLEN bytes;
// you need only AES_init_ctx as IV is not used in ECB
// NB: ECB is considered insecure for most uses
static void AES_ECB_encrypt(const struct AES_ctx* ctx, uint8_t* buf);
static void AES_ECB_decrypt(const struct AES_ctx* ctx, uint8_t* buf);

extern uint8_t encrypt_ecb(uint8_t *key_buffer, uint8_t *in_buffer, uint16_t in_buffer_size, uint8_t *out_buffer, uint16_t *out_buffer_size);
extern uint8_t decrypt_ecb(uint8_t *key_buffer, uint8_t *in_buffer, uint16_t in_buffer_size, uint8_t *out_buffer, uint16_t *out_buffer_size);
//extern void phex(uint8_t* str, uint16_t len);
#endif // _AES_H_

#endif //#ifdef HAVE_AES128_ECB_PKCS7PADDING
