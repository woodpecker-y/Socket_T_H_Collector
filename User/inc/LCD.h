#ifndef  __LCD_H__
#define  __LCD_H__

#include "stm8l15x.h"
#include "stm8l15x_lcd.h"

/*************************************************************************************

              1A             2A                3A
           ------          ------            -----
          |      |        |      |          |     |
        1F|      |1B    2F|      |2B      3F|     |2B
          |  1G  |        |      |          |     |
           ------          -----             ------
          |      |        |      |          |     |
        1E|      |1C    2E|      |2C      3E|     |3C
          |      |        |      |          |     |
           ------          ------            -----
             1D              2D                 3D


      COM3 COM2 COM1 COM0
SEG0 { A  , F  , E  , D  }
SEG1 { B  , G  , C  , S7 }

如显示‘0 ’，则

 { 1 , 1 , 1 , 1 }
 { 1 , 0 , 1 , 0 }
-------------------
=  3   1   3   1  hex
=> '0' = 0x3131
*************************************************************************************/

#define Mark_S0 	    0x00 	//代表 信号标志0
#define Mark_S1 		0x01 	//代表 信号标志1
#define Mark_S2 		0x02 	//代表 信号标志2
#define Mark_S3 		0x03 	//代表 信号标志3
#define Mark_S4 		0x04 	//代表 信号标志4
#define Mark_T 		    0x05 	//代表 温度标志
#define Mark_RH 		0x06 	//代表 湿度标志
#define Mark_PT 		0x07 	//代表 小数点标志
#define Mark_PC		    0x08 	//代表 湿度百分比标志
#define Mark_DC 		0x09 	//代表 Degree Centigrade 摄氏度标志
#define Mark_FD 		0x0A 	//代表 Fahrenheit degree 华氏度标志

#define Mark_Error      0x0B    //检测温湿度错误

typedef enum {
    WRITE = 0,
    CLEAR = !WRITE
}LCD_WE_FLAG;         // LCD写擦除标志


void LCD_Config(void);

void LCD_FullScreen(void);
void LCD_ClearScreen(void);

void LCD_DisplayMARK(uint8_t Mark, LCD_WE_FLAG flag);
void LCD_WE_NUM(uint8_t ConvertNum, uint8_t position, LCD_WE_FLAG flag);

void LCD_Display_T(uint16_t DispTemprature,LCD_WE_FLAG flag);
void LCD_Display_H(uint16_t DispTemprature,LCD_WE_FLAG flag);
void LCD_disp_ant(u8 ant_lv);
void LCD_poll_disp(void);
void LCD_disp_clearant(void);

#endif