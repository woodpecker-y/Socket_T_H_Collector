#include <stdio.h>
#include <string.h>
#include "delay.h"
#include "sht.h"
#include "sht3x.h"
#include "utili.h"
#include "bsp_gpio.h"

// 解决SHT3X 调用端口映射带来的开销可能影响I2C高速通讯的稳定性的问题
typedef struct _SHT3xRemapConfig{
    GPIO_TypeDef*       scl_port;
    GPIO_Pin_TypeDef    scl_pin;
    GPIO_TypeDef*       sda_port;
    GPIO_Pin_TypeDef    sda_pin;
    GPIO_TypeDef*       pwr_port;
    GPIO_Pin_TypeDef    pwr_pin;
}SHT3xRemapConfig;

#define RETRY_CNT 3

static SHT3xConfig *sg_sht3x_cfg_handler = NULL;
static SHT3xRemapConfig sg_sht3x_remap_config;

void BSP_SHT3x_Load_Config(SHT3xConfig *handler)
{
    sg_sht3x_cfg_handler = handler;

    // 缓存SHT3X I2C端口配置
    sg_sht3x_remap_config.scl_port = BSP_GPIO_Port_Remap(sg_sht3x_cfg_handler->scl.port);
    sg_sht3x_remap_config.scl_pin = BSP_GPIO_Pin_Remap(sg_sht3x_cfg_handler->scl.pin);
    sg_sht3x_remap_config.sda_port = BSP_GPIO_Port_Remap(sg_sht3x_cfg_handler->sda.port);
    sg_sht3x_remap_config.sda_pin = BSP_GPIO_Pin_Remap(sg_sht3x_cfg_handler->sda.pin);
    sg_sht3x_remap_config.pwr_port = BSP_GPIO_Port_Remap(sg_sht3x_cfg_handler->pwr.port);
    sg_sht3x_remap_config.pwr_pin = BSP_GPIO_Pin_Remap(sg_sht3x_cfg_handler->pwr.pin);
}

void BSP_SHT3x_SCL_SetOutput(void)
{
    GPIO_Init(sg_sht3x_remap_config.scl_port, sg_sht3x_remap_config.scl_pin, GPIO_Mode_Out_OD_Low_Fast);

}

void BSP_SHT3x_SDA_SetOutput(void)
{
    GPIO_Init(sg_sht3x_remap_config.sda_port, sg_sht3x_remap_config.sda_pin, GPIO_Mode_Out_OD_Low_Fast);
}

void BSP_SHT3x_SDA_SetInput(void)
{
    GPIO_Init(sg_sht3x_remap_config.sda_port, sg_sht3x_remap_config.sda_pin, GPIO_Mode_In_FL_No_IT);
}

void BSP_SHT3x_SCL_SetInput(void)
{
    GPIO_Init(sg_sht3x_remap_config.scl_port, sg_sht3x_remap_config.scl_pin, GPIO_Mode_In_FL_No_IT);
}

void BSP_SHT3x_SCL_SetHigh(void)
{
    GPIO_SetBits(sg_sht3x_remap_config.scl_port, sg_sht3x_remap_config.scl_pin);
}

void BSP_SHT3x_SCL_SetLow(void)
{
    GPIO_ResetBits(sg_sht3x_remap_config.scl_port, sg_sht3x_remap_config.scl_pin);
}

void BSP_SHT3x_SDA_SetHigh(void)
{
    GPIO_SetBits(sg_sht3x_remap_config.sda_port, sg_sht3x_remap_config.sda_pin);
}

void BSP_SHT3x_SDA_SetLow(void)
{
    GPIO_ResetBits(sg_sht3x_remap_config.sda_port, sg_sht3x_remap_config.sda_pin);
}

u8 BSP_SHT3x_SDA_ReadState(void)
{
    return GPIO_ReadInputDataBit(sg_sht3x_remap_config.sda_port, sg_sht3x_remap_config.sda_pin) == RESET ? 0 : 1;
}

u8 BSP_SHT3x_SCK_ReadState(void)
{
    return GPIO_ReadInputDataBit(sg_sht3x_remap_config.scl_port, sg_sht3x_remap_config.scl_pin) == RESET ? 0 : 1;
}

/*! \brief
*      SHT3X传感器上电
*/
void BSP_SHT3x_PowerOn(void)
{
    if (NULL != &(sg_sht3x_cfg_handler->pwr))
    {
        if (E_LE_HIGH == sg_sht3x_cfg_handler->pwr_le)
        {
            BSP_GPIO_Init(sg_sht3x_cfg_handler->pwr.port,sg_sht3x_cfg_handler->pwr.pin, GPIO_Mode_Out_PP_High_Fast);
            BSP_GPIO_SetHigh(sg_sht3x_cfg_handler->pwr.port,sg_sht3x_cfg_handler->pwr.pin);
        }
        else
        {
            BSP_GPIO_Init(sg_sht3x_cfg_handler->pwr.port,sg_sht3x_cfg_handler->pwr.pin, GPIO_Mode_Out_PP_Low_Fast);
            BSP_GPIO_SetLow(sg_sht3x_cfg_handler->pwr.port,sg_sht3x_cfg_handler->pwr.pin);
        }
    }
}

/*! \brief
*      SHT3X传感器断电
*/
void BSP_SHT3x_PowerOff(void)
{
    if (NULL != &(sg_sht3x_cfg_handler->pwr))
    {
        if (E_LE_HIGH == sg_sht3x_cfg_handler->pwr_le)
        {
            BSP_GPIO_Init(sg_sht3x_cfg_handler->pwr.port,sg_sht3x_cfg_handler->pwr.pin, GPIO_Mode_Out_PP_Low_Fast);
            BSP_GPIO_SetLow(sg_sht3x_cfg_handler->pwr.port,sg_sht3x_cfg_handler->pwr.pin);
        }
        else
        {
            BSP_GPIO_Init(sg_sht3x_cfg_handler->pwr.port,sg_sht3x_cfg_handler->pwr.pin, GPIO_Mode_Out_PP_High_Slow);
            BSP_GPIO_SetHigh(sg_sht3x_cfg_handler->pwr.port,sg_sht3x_cfg_handler->pwr.pin);
        }
    }
}



/*! \brief
*      SHT3X传感器关闭
*/
void BSP_SHT3x_Close(void)
{
    BSP_SHT3x_SCL_SetOutput();
    BSP_SHT3x_SDA_SetOutput();

    BSP_SHT3x_SCL_SetLow();
    BSP_SHT3x_SDA_SetLow();
}

/*! \brief
*      IIC开始时序
*/
void BSP_SHT3x_I2CStart(void)
{
    BSP_SHT3x_SCL_SetOutput();
    BSP_SHT3x_SDA_SetOutput();

    BSP_SHT3x_SCL_SetHigh();
    BSP_SHT3x_SDA_SetHigh();

    BSP_SHT3x_SDA_SetLow();
    BSP_SHT3x_Delay(30);

    BSP_SHT3x_SCL_SetLow();
    BSP_SHT3x_Delay(30);
}

/*! \brief
*      IIC停止时序
*/
void BSP_SHT3x_I2CStop(void)
{
    BSP_SHT3x_SCL_SetOutput();
    BSP_SHT3x_SDA_SetOutput();

    BSP_SHT3x_SDA_SetLow();
    BSP_SHT3x_SCL_SetLow();

    BSP_SHT3x_SCL_SetHigh();
    BSP_SHT3x_Delay(30);

    BSP_SHT3x_SDA_SetHigh();
    BSP_SHT3x_Delay(30);
}

/*! \brief
*      IIC主机应答时序
*/
void BSP_SHT3x_I2CAcknowledge(void)
{
    BSP_SHT3x_SCL_SetOutput();
    BSP_SHT3x_SDA_SetOutput();

    BSP_SHT3x_SDA_SetLow();

    BSP_SHT3x_SCL_SetHigh();
    BSP_SHT3x_Delay(80);

    BSP_SHT3x_SCL_SetLow();
    BSP_SHT3x_Delay(80);
}
/*! \brief
*      IIC主机不应答时序
*/
void BSP_SHT3x_I2CNoAcknowledge(void)
{
    BSP_SHT3x_SCL_SetOutput();
    BSP_SHT3x_SDA_SetOutput();

    BSP_SHT3x_SDA_SetHigh();

    BSP_SHT3x_SCL_SetHigh();
    BSP_SHT3x_Delay(80);

    BSP_SHT3x_SCL_SetLow();
    BSP_SHT3x_Delay(80);
}
/*! \brief
*      IIC读一个字节
*
* \return
*       val                                     - 返回读取出的一个字节
*/
u8 BSP_SHT3x_I2CReadByte(void)
{
    u8 i, val = 0;

    BSP_SHT3x_SCL_SetOutput();
    BSP_SHT3x_SDA_SetOutput();

    BSP_SHT3x_SDA_SetHigh();
    BSP_SHT3x_SDA_SetInput();

    for (i = 0; i < 8; i++)
    {
        val <<= 1;

        BSP_SHT3x_SCL_SetHigh();

        if (Bit_SET == BSP_SHT3x_SDA_ReadState())
        {
            val |= 0x01;
        }

        BSP_SHT3x_SCL_SetLow();
    }

    BSP_SHT3x_SDA_SetOutput();
    BSP_SHT3x_SDA_SetHigh();

    return (val);
}
/*! \brief
*      IIC写一个字节
* \param byte[IN]				- 写入的字节
*
* \return
*       ACK_OK = 0x00                           - 成功
*       ACK_ERROR = 0x01                        - 失败
*/
u8 BSP_SHT3x_I2CWriteByte(u8 byte)
{
    u8 i = 0;
    u8 ack = 0;

    BSP_SHT3x_SCL_SetOutput();
    BSP_SHT3x_SDA_SetOutput();

    for (i = 0; i < 8; i++)
    {
        if (byte & 0x80)
        {
            BSP_SHT3x_SDA_SetHigh();
        }
        else
        {
            BSP_SHT3x_SDA_SetLow();
        }

        BSP_SHT3x_SCL_SetHigh();
        BSP_SHT3x_Delay(80);

        BSP_SHT3x_SCL_SetLow();
        BSP_SHT3x_Delay(80);

        byte <<= 1;
    }

    BSP_SHT3x_SDA_SetInput();
    BSP_SHT3x_SCL_SetHigh();

    if (Bit_SET == BSP_SHT3x_SDA_ReadState())
    {
        ack = ACK_ERROR;
    }
    else
    {
        ack = ACK_OK;
    }

    BSP_SHT3x_SCL_SetLow();

    BSP_SHT3x_SDA_SetOutput();

    BSP_SHT3x_SDA_SetHigh();

    return (ack);
}
/*! \brief
*      软件复位
*/
void BSP_SHT3x_SoftReset(void)
{
    BSP_SHT3x_I2CStart();
    BSP_SHT3x_I2CWriteByte(SHT3x_I2C_ADR_W);
    //0x30A2软件复位指令
    BSP_SHT3x_I2CWriteByte(0x30);
    BSP_SHT3x_I2CWriteByte(0xA2);

    BSP_SHT3x_I2CStop();
    BSP_SHT3x_Delay(80);
}

/*! \brief
*      SHT3X传感器初始化
*/
void BSP_SHT3x_Init(void)
{
    BSP_SHT3x_SCL_SetOutput();
    BSP_SHT3x_SDA_SetOutput();

    BSP_SHT3x_SCL_SetHigh();
    BSP_SHT3x_SDA_SetHigh();
    BSP_SHT3x_Delay(80);
    BSP_SHT3x_SoftReset();

    BSP_SHT3x_Delay(100);
    BSP_SHT3x_I2CStart();
    BSP_SHT3x_I2CWriteByte(SHT3x_I2C_ADR_W);
    BSP_SHT3x_I2CWriteByte(0x20);
    BSP_SHT3x_I2CWriteByte(0x32);
    BSP_SHT3x_I2CStop();
}

/*! \brief
*      SHT3X传感器采集
* \param temp[IN]				- 温度
* \param humi[IN]				- 湿度
* \return
*       E_SHT_OK = 0,                           - 正常
*       E_SHT_ERR_NO_RESPONSE = -1,             - 读没有反馈
*/
int BSP_SHT3x_Acquisition(s16 *temp,s16 *humi)
{
    u16 number,sensor = 0;
    u8 ack = 0;
    u16 tmpMSB, tmpLSB = 0;
    u16 humiMSB, humiLSB = 0;
    u8 retry_cnt = 0;

    BSP_SHT3x_I2CStart();
    BSP_SHT3x_I2CWriteByte(SHT3x_I2C_ADR_W);
    BSP_SHT3x_I2CWriteByte(0xE0);
    BSP_SHT3x_I2CWriteByte(0x00);
//    BSP_SHT3x_I2CWriteByte(0x24);
//    BSP_SHT3x_I2CWriteByte(0x16);

    BSP_SHT3x_I2CStop();
    do
    {
        BSP_SHT3x_Delay(20);  //20us
        BSP_SHT3x_I2CStart();
        ack = BSP_SHT3x_I2CWriteByte(SHT3x_I2C_ADR_R);

        ++retry_cnt;
    } while(ACK_ERROR == ack && retry_cnt <= RETRY_CNT);

    if (retry_cnt > RETRY_CNT)
    {
        return E_SHT_ERR_NO_RESPONSE;
    }

    // 开始读温度
    tmpMSB = BSP_SHT3x_I2CReadByte();
    BSP_SHT3x_I2CAcknowledge();

    tmpLSB = BSP_SHT3x_I2CReadByte();
    BSP_SHT3x_I2CAcknowledge();

    BSP_SHT3x_I2CReadByte();// 温度末尾的校验没有处理
    BSP_SHT3x_I2CAcknowledge();


    // 开始读湿度
    humiMSB = BSP_SHT3x_I2CReadByte();
    BSP_SHT3x_I2CAcknowledge();

    humiLSB = BSP_SHT3x_I2CReadByte();
    BSP_SHT3x_I2CAcknowledge();

    BSP_SHT3x_I2CReadByte();// 温度末尾的校验没有处理
    BSP_SHT3x_I2CNoAcknowledge();

    BSP_SHT3x_I2CStop();

    //number = ((tmpMSB  *256) + tmpLSB);
    number = (tmpMSB << 8) | tmpLSB;
    number = (int)(((float)number*175.0/65535.0 - 45.0)*10.0);
    //number= (s16)(number/37.5)-450;

    //sensor = (humiMSB *256) + humiLSB;
    sensor = humiMSB << 8 | humiLSB;
    semsor = (int)((100.0 * (float)sensor / 65535.0) * 10.0);
    //sensor = (int)(((float)sensor * 100.0 / 65535.0) * 10.0);

    *temp = number;
    *humi = sensor;

    return (0);

}