#ifndef _BU9792_H
#define _BU9792_H

/*! \brief LCD��ʼ�� */
void STM8L15x_LCD_Init(void);
/*! \brief LCD�ر� */
void STM8L15x_LCD_Close(void);
/*! \brief LCD�ϵ� */
void STM8L15x_LCD_PowerOn(void);
/*! \brief LCD���� */
void STM8L15x_LCD_PowerOff(void);
/*! \brief LCDд���� */
void STM8L15x_LCD_WriteData(unsigned char addr, const unsigned char *data, unsigned char len);

#endif