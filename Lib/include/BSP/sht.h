#ifndef _SHT_H
#define _SHT_H

typedef enum _SHTRet{
    E_SHT_OK = 0,
    E_SHT_ERR_NO_RESPONSE = -1,
}SHTRet;

typedef enum {
    ACK_OK                     = 0x00,  
    ACK_ERROR                  = 0x01 
} SHTI2cAck;

#endif