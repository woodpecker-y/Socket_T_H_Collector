#ifndef _BU9792_H
#define _BU9792_H

/*! \brief LCD初始化 */
void STM8L15x_LCD_Init(void);
/*! \brief LCD关闭 */
void STM8L15x_LCD_Close(void);
/*! \brief LCD上电 */
void STM8L15x_LCD_PowerOn(void);
/*! \brief LCD掉电 */
void STM8L15x_LCD_PowerOff(void);
/*! \brief LCD写数据 */
void STM8L15x_LCD_WriteData(unsigned char addr, const unsigned char *data, unsigned char len);

#endif