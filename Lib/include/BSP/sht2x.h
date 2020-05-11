#ifndef __SHT2X_H
#define __SHT2X_H

#ifdef HAVE_SHT2x

typedef struct _SHT2xConfig{
    PortTypeDef scl;
    PortTypeDef sda;
    PortTypeDef pwr; 
    u8          pwr_le;         /*!< \brief power level effective */    
}SHT2xConfig;

#define BSP_SHT2x_Delay(a)          delay_us(a)

#define Bit_RESET               0
#define Bit_SET                 1

//typedef enum _SHT2xRet{
//    E_SHT_OK = 0,
//    E_SHT_ERR_NO_RESPONSE = -1,
//}SHT2xRet;

typedef enum {
    TRIG_TEMP_MEASUREMENT_HM   = 0xE3, //command trig. temp meas. hold master
    TRIG_HUMI_MEASUREMENT_HM   = 0xE5, //command trig. humidity meas. hold master
    TRIG_TEMP_MEASUREMENT_POLL = 0xF3, //command trig. temp meas. no hold master
    TRIG_HUMI_MEASUREMENT_POLL = 0xF5, //command trig. humidity meas. no hold master
    USER_REG_W                 = 0xE6, //command writing user register
    USER_REG_R                 = 0xE7, //command reading user register
    SOFT_RESET                 = 0xFE  //command soft reset
} SHT2xCommand;

typedef enum {
    SHT2x_RES_12_14BIT         = 0x00, //RH=12bit, T=14bit
    SHT2x_RES_8_12BIT          = 0x01, //RH= 8bit, T=12bit
    SHT2x_RES_10_13BIT         = 0x80, //RH=10bit, T=13bit
    SHT2x_RES_11_11BIT         = 0x81, //RH=11bit, T=11bit
    SHT2x_RES_MASK             = 0x81  //Mask for res. bits (7,0) in user reg.
} SHT2xResolution;

typedef enum {
    SHT2x_EOB_ON               = 0x40, //end of battery
    SHT2x_EOB_MASK             = 0x40  //Mask for EOB bit(6) in user reg.
} SHT2xEob;

typedef enum {
    SHT2x_HEATER_ON            = 0x04, //heater on
    SHT2x_HEATER_OFF           = 0x00, //heater off
    SHT2x_HEATER_MASK          = 0x04  //Mask for Heater bit(2) in user reg.
} SHT2xHeater;

typedef enum {
    TEMP,  
    HUMI 
} SHT2xMeasureType;

typedef enum {
    SHT2x_I2C_ADR_W                  = 0x80, //sensor I2C address + write bit
    SHT2x_I2C_ADR_R                  = 0x81  //sensor I2C address + read bit
} SHT2xI2cHeader;

//typedef enum {
//    ACK                        = 0,
//    NO_ACK                     = 1
//} SHT2xI2cAck;

//typedef enum {
//    ACK_OK                     = 0x00,  
//    ACK_ERROR                  = 0x01
//} SHT2xI2cAckState;

void BSP_SHT2x_Load_Config(SHT2xConfig *handler);
void BSP_SHT2x_PowerOn(void);
void BSP_SHT2x_PowerOff(void);
void BSP_SHT2x_Init(void);
void BSP_SHT2x_Close(void);
int  BSP_SHT2x_Acquisition(s16 *temp,s16 *humi);

#endif

#endif

