#ifndef _RC522_H
#define _RC522_H

#ifdef HAVE_RFID

//// SCK
//#define RC522_SPI_SCK_Init()    (GPIO_Init(RC522_SPI_GPIO_SCK, RC522_SPI_PIN_SCK, GPIO_Mode_Out_PP_Low_Fast))
//#define RC522_SPI_SCK_H()       (GPIO_SetBits(RC522_SPI_GPIO_SCK, RC522_SPI_PIN_SCK))
//#define RC522_SPI_SCK_L()       (GPIO_ResetBits(RC522_SPI_GPIO_SCK, RC522_SPI_PIN_SCK))
//// MOSI
//#define RC522_SPI_MOSI_Init()   (GPIO_Init(RC522_SPI_GPIO_MOSI, RC522_SPI_PIN_MOSI, GPIO_Mode_Out_PP_Low_Fast))
//#define RC522_SPI_MOSI_H()      (GPIO_SetBits(RC522_SPI_GPIO_MOSI, RC522_SPI_PIN_MOSI))
//#define RC522_SPI_MOSI_L()      (GPIO_ResetBits(RC522_SPI_GPIO_MOSI, RC522_SPI_PIN_MOSI))
//// MISO
//#define RC522_SPI_MISO_Init()   (GPIO_Init(RC522_SPI_GPIO_MISO, RC522_SPI_PIN_MISO, GPIO_Mode_In_PU_No_IT))
//#define RC522_SPI_MISO_Lpr()    (GPIO_Init(RC522_SPI_GPIO_MISO, RC522_SPI_PIN_MISO, GPIO_Mode_Out_PP_Low_Fast))
//#define RC522_SPI_MISO_H()      (GPIO_SetBits(RC522_SPI_GPIO_MISO, RC522_SPI_PIN_MISO))
//#define RC522_SPI_MISO_L()      (GPIO_ResetBits(RC522_SPI_GPIO_MISO, RC522_SPI_PIN_MISO))
//#define RC522_SPI_MISO_Read()   (GPIO_ReadInputDataBit(RC522_SPI_GPIO_MISO, RC522_SPI_PIN_MISO))
//// NSS
//#define RC522_SPI_NSS_Init()    (GPIO_Init(RC522_SPI_GPIO_NSS, RC522_SPI_PIN_NSS, GPIO_Mode_Out_PP_Low_Fast))
//#define RC522_SPI_NSS_H()       (GPIO_SetBits(RC522_SPI_GPIO_NSS, RC522_SPI_PIN_NSS))
//#define RC522_SPI_NSS_L()       (GPIO_ResetBits(RC522_SPI_GPIO_NSS, RC522_SPI_PIN_NSS))
//
//// RC522 RST
//#define RC522_RST_Init()        (GPIO_Init(RC522_GPIO_RST, RC522_PIN_RST, GPIO_Mode_Out_PP_Low_Fast))
//#define RC522_RST_H()           (GPIO_SetBits(RC522_GPIO_RST, RC522_PIN_RST))
//#define RC522_RST_L()           (GPIO_ResetBits(RC522_GPIO_RST, RC522_PIN_RST))
//
//// RC522 PWR
//#define RC522_PWR_Init()        (GPIO_Init(RC522_GPIO_PWR, RC522_PIN_PWR, GPIO_Mode_Out_PP_Low_Fast))
//#define RC522_PWR_H()           (GPIO_SetBits(RC522_GPIO_PWR, RC522_PIN_PWR))
//#define RC522_PWR_L()           (GPIO_ResetBits(RC522_GPIO_PWR, RC522_PIN_PWR))

/*! \brief rc522 authentication key mode definition */
typedef enum _AuthKeyMode{
    KEY_A = 0x60,
    KEY_B = 0x61,
}AuthKeyMode;

/*! \brief rc522 value block operate mode definition */
typedef enum _ValueOpMode{
    VAL_DEC = 0xC0,
    VAL_INC = 0xC1,
}ValueOpMode;

////////////////////////////////////////////////////////////////////
//MF522命令字
/////////////////////////////////////////////////////////////////////
#define PCD_IDLE              0x00               //取消当前命令
#define PCD_AUTHENT           0x0E               //验证密钥
#define PCD_RECEIVE           0x08               //接收数据
#define PCD_TRANSMIT          0x04               //发送数据
#define PCD_TRANSCEIVE        0x0C               //发送并接收数据
#define PCD_RESETPHASE        0x0F               //复位
#define PCD_CALCCRC           0x03               //CRC计算

/////////////////////////////////////////////////////////////////////
//Mifare_One卡片命令字
/////////////////////////////////////////////////////////////////////
#define PICC_REQIDL           0x26               //寻天线区内未进入休眠状态
#define PICC_REQALL           0x52               //寻天线区内全部卡
#define PICC_ANTICOLL1        0x93               //防冲撞
#define PICC_ANTICOLL2        0x95               //防冲撞
#define PICC_AUTHENT1A        0x60               //验证A密钥
#define PICC_AUTHENT1B        0x61               //验证B密钥
#define PICC_READ             0x30               //读块
#define PICC_WRITE            0xA0               //写块
#define PICC_DECREMENT        0xC0               //扣款
#define PICC_INCREMENT        0xC1               //充值
#define PICC_RESTORE          0xC2               //调块数据到缓冲区
#define PICC_TRANSFER         0xB0               //保存缓冲区中数据
#define PICC_HALT             0x50               //休眠

/////////////////////////////////////////////////////////////////////
//MF522 FIFO长度定义
/////////////////////////////////////////////////////////////////////
#define DEF_FIFO_LENGTH       64                 //FIFO size=64byte

/////////////////////////////////////////////////////////////////////
//MF522寄存器定义
/////////////////////////////////////////////////////////////////////
// PAGE 0
#define     RFU00                 0x00
#define     CommandReg            0x01
#define     ComIEnReg             0x02
#define     DivlEnReg             0x03
#define     ComIrqReg             0x04
#define     DivIrqReg             0x05
#define     ErrorReg              0x06
#define     Status1Reg            0x07
#define     Status2Reg            0x08
#define     FIFODataReg           0x09
#define     FIFOLevelReg          0x0A
#define     WaterLevelReg         0x0B
#define     ControlReg            0x0C
#define     BitFramingReg         0x0D
#define     CollReg               0x0E
#define     RFU0F                 0x0F
// PAGE 1
#define     RFU10                 0x10
#define     ModeReg               0x11
#define     TxModeReg             0x12
#define     RxModeReg             0x13
#define     TxControlReg          0x14
#define     TxAutoReg             0x15
#define     TxSelReg              0x16
#define     RxSelReg              0x17
#define     RxThresholdReg        0x18
#define     DemodReg              0x19
#define     RFU1A                 0x1A
#define     RFU1B                 0x1B
#define     MifareReg             0x1C
#define     RFU1D                 0x1D
#define     RFU1E                 0x1E
#define     SerialSpeedReg        0x1F
// PAGE 2
#define     RFU20                 0x20
#define     CRCResultRegM         0x21
#define     CRCResultRegL         0x22
#define     RFU23                 0x23
#define     ModWidthReg           0x24
#define     RFU25                 0x25
#define     RFCfgReg              0x26
#define     GsNReg                0x27
#define     CWGsCfgReg            0x28
#define     ModGsCfgReg           0x29
#define     TModeReg              0x2A
#define     TPrescalerReg         0x2B
#define     TReloadRegH           0x2C
#define     TReloadRegL           0x2D
#define     TCounterValueRegH     0x2E
#define     TCounterValueRegL     0x2F
// PAGE 3
#define     RFU30                 0x30
#define     TestSel1Reg           0x31
#define     TestSel2Reg           0x32
#define     TestPinEnReg          0x33
#define     TestPinValueReg       0x34
#define     TestBusReg            0x35
#define     AutoTestReg           0x36
#define     VersionReg            0x37
#define     AnalogTestReg         0x38
#define     TestDAC1Reg           0x39
#define     TestDAC2Reg           0x3A
#define     TestADCReg            0x3B
#define     RFU3C                 0x3C
#define     RFU3D                 0x3D
#define     RFU3E                 0x3E
#define     RFU3F		  		  0x3F

/////////////////////////////////////////////////////////////////////
//和MF522通讯时返回的错误代码
/////////////////////////////////////////////////////////////////////
#define 	MI_OK                 (char)0
#define 	MI_NOTAGERR           (char)(-1)
#define 	MI_ERR                (char)(-2)

/*! \brief rfid port confirgure Definition */
typedef struct _RFIDConfig{
    PortTypeDef pwr;        /*!< \brief rfid power control pin */
    PortTypeDef rst;        /*!< \brief rfid reset pin */
    PortTypeDef nss;        /*!< \brief rfid chip select */
    PortTypeDef sck;        /*!< \brief rfid serial clock */
    PortTypeDef mosi;       /*!< \brief rfid mosi pin */
    PortTypeDef miso;       /*!< \brief rfid miso pin */
}RFIDConfig;

/////////////////////////////////////////////////////////////////////
//函数原型
/////////////////////////////////////////////////////////////////////
/*! \brief
*       load rfid port configure
* \param handler[IN]                - rfid port configure handler
*/
void BSP_RFID_LoadConfig(const RFIDConfig *handler);

/*! \brief
*       rfid power on
*/
void BSP_RFID_PowerOn(void);

/*! \brief
*       rfid power off
*/
void BSP_RFID_PowerOff(void);

/*! \brief
*       rc522 reset
* \return
*       MI_OK       - success
*       MI_ERR      - failure
*/
char BSP_RFID_Reset(u8 *ctrl_reg, u8 *version);

/*! \brief
*       rc522 close
*/
void BSP_RFID_Close(void);

/*! \brief
*       rc522 request card
* \param req_code[IN]      - request mode
*           0x52    - request all card with ISO14443 Type
*           0x26    - request card with ISO14443 Type which is not sleeped.
* \param pTagType[OUT]      - card type code
*           0x4400  - Mifare_UltraLight
*           0x0400  - Mifare_One(S50)
*           0x0200  - Mifare_One(S70)
*           0x0800  - Mifare_Pro(X)
*           0x4403  - Mifare_DESFire
* \return
*       MI_OK   - success
*       MI_ERR  - failure
*/
char BSP_RFID_Request(unsigned char req_code,unsigned char *pTagType);

/*! \brief
*       rc522 anticoll and get one card number
* \param pSnr[OUT]      - card number,4bytes
* \return
*       MI_OK   - success
*       MI_ERR  - failure
*/
char BSP_RFID_Anticoll(unsigned char *pSnr);

/*! \brief
*       rc522 select card
* \param pSnr[IN]       - card number,4bytes
* \param pSize[OUT]     - card
* \return
*       MI_OK   - success
*       MI_ERR  - failure
*/
char BSP_RFID_Select(unsigned char *pSnr, unsigned char *pSize);


/*! \brief
*       rc522 authentication card password
* \param auth_mode[IN]      - card authentication mode
*           KEY_A(0x60)     - authentication key A
*           KEY_B(0x61)     - authentication key B
* \param addr[IN]           - card block address
* \param pKey[IN]           - card key password
* \param pSnr[IN]           - card number,4bytes
* \return
*       MI_OK   - success
*       MI_ERR  - failure
*/
char BSP_RFID_AuthState(unsigned char auth_mode,unsigned char addr,unsigned char *pKey,unsigned char *pSnr);

/*! \brief
*       rc522 read card block data
* \param addr[IN]           - card block address
* \param pData[OUT]         - card block data, 16bytes
* \return
*       MI_OK   - success
*       MI_ERR  - failure
*/
char BSP_RFID_Read(unsigned char addr,unsigned char *pData);

/*! \brief
*       rc522 write card block data
* \param addr[IN]           - card block address
* \param pData[IN]          - card block data, 16bytes
* \return
*       MI_OK   - success
*       MI_ERR  - failure
*/
char BSP_RFID_Write(unsigned char addr,unsigned char *pData);

/*! \brief
*       rc522 operate card value block data:increase value or descrease value
* \param dd_mode[IN]        - value block operate mode
*           VAL_DEC(0xC0)   - descrease value
*           VAL_INC(0xC1)   - increase value
* \param addr[IN]           - card block address
* \param pValue[IN]         - value, 4bytes, Intel Byte Order
* \return
*       MI_OK   - success
*       MI_ERR  - failure
*/
char BSP_RFID_Value(unsigned char dd_mode,unsigned char addr,unsigned char *pValue);

/*! \brief
*       rc522 backup wallet
* \param sourceaddr[IN]        - wallet source address
* \param goaladdr[IN]          - wallet goal address
* \return
*       MI_OK   - success
*       MI_ERR  - failure
*/
char BSP_RFID_BakValue(unsigned char sourceaddr, unsigned char goaladdr);

/*! \brief
*       rc522 make card sleep
* \return
*       MI_OK   - success
*       MI_ERR  - failure
*/
char BSP_RFID_Halt(void);

//void PcdPowerOn(void);
//void PcdPowerOff(void);
//char PcdReset(void);
//void PcdAntennaOn(void);
//void PcdAntennaOff(void);
//char PcdConfigISOType(unsigned char type);
//char PcdRequest(unsigned char req_code,unsigned char *pTagType);
//char PcdAnticoll(unsigned char *pSnr);
//char PcdSelect(unsigned char *pSnr, unsigned char *pSize);
//char PcdAuthState(unsigned char auth_mode,unsigned char addr,unsigned char *pKey,unsigned char *pSnr);
//char PcdRead(unsigned char addr,unsigned char *pData);
//char PcdWrite(unsigned char addr,unsigned char *pData);
//char PcdValue(unsigned char dd_mode,unsigned char addr,unsigned char *pValue);
//char PcdBakValue(unsigned char sourceaddr, unsigned char goaladdr);
//char PcdHalt(void);
//void PcdClose(void);

#endif

#endif