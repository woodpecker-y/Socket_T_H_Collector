#define  _INTLCD_DRIVER_C_H

#include "sysparams.h"
#include "LCD.h"

/*Local Variables -------------------------------------------------------------*/
__CONST uint8_t SetBit0 = 0x01;
__CONST uint8_t SetBit1 = 0x02;
__CONST uint8_t SetBit2 = 0x04;
__CONST uint8_t SetBit3 = 0x08;
__CONST uint8_t SetBit4 = 0x10;
__CONST uint8_t SetBit5 = 0x20;
__CONST uint8_t SetBit6 = 0x40;
__CONST uint8_t SetBit7 = 0x80;

__CONST uint8_t ClrBit0 = 0xFE;
__CONST uint8_t ClrBit1 = 0xFD;
__CONST uint8_t ClrBit2 = 0xFB;
__CONST uint8_t ClrBit3 = 0xF7;
__CONST uint8_t ClrBit4 = 0xEF;
__CONST uint8_t ClrBit5 = 0xDF;
__CONST uint8_t ClrBit6 = 0xBF;
__CONST uint8_t ClrBit7 = 0x7F;

                                /*   0       1       2       3       4       5       6       7       8       9      10'-'   */
__CONST uint16_t NumberMap[16] = {0x3131, 0x2020, 0x3211, 0x3221, 0x2320, 0x1321, 0x1331, 0x3020, 0x3331, 0x3321, 0x0200};

uint8_t Digit[4] = {0};

void LCD_Config(void)
{
    CLK_PeripheralClockConfig(CLK_Peripheral_LCD, ENABLE);
    CLK_RTCClockConfig(CLK_RTCCLKSource_LSE, CLK_RTCCLKDiv_1);

    LCD_Init(LCD_Prescaler_4, LCD_Divider_16, LCD_Duty_1_4, LCD_Bias_1_3, LCD_VoltageSource_Internal);//LCD_VoltageSource_Internal    LCD_VoltageSource_External
    LCD_PortMaskConfig(LCD_PortMaskRegister_0, 0xFF);           // Configures the LCD Port Mask.  0 --- 7  SEG
    LCD_PortMaskConfig(LCD_PortMaskRegister_1, 0x00);           // Configures the LCD Port Mask.  8 --- 15 SEG
    LCD_PortMaskConfig(LCD_PortMaskRegister_2, 0x00);           // Configures the LCD Port Mask.  16--- 23 SEG
    LCD_PortMaskConfig(LCD_PortMaskRegister_3, 0x00);           // Configures the LCD Port Mask.  24--- 31 SEG

    //LCD_Contrast_Level_7   LCD_Contrast_3V3    LCD_Contrast_Level_4
    LCD_ContrastConfig(LCD_Contrast_Level_4);                   // The contrast can be adjusted using two different methods:
    LCD_DeadTimeConfig(LCD_DeadTime_0);     		    // LCD_DeadTimeConfig(LCD_DeadTime_0);
    LCD_PulseOnDurationConfig(LCD_PulseOnDuration_7);
    LCD_Cmd(ENABLE);
}

//特殊符号
void LCD_DisplayMARK(uint8_t Mark, LCD_WE_FLAG flag)
{
    switch(Mark)
    {
        case Mark_RH:   //Mark_S0 代表湿度标志 “水滴”
            {
                //LCD->RAM[LCD_RAMRegister_0] &=  ClrBit6;

                if( CLEAR != flag)
                {
                    LCD->RAM[LCD_RAMRegister_0] |=  SetBit6;
                }
                else
                {
                   LCD->RAM[LCD_RAMRegister_0] &=  ClrBit6;
                }
            }
            break;
        case Mark_T:
            {
                //LCD->RAM[LCD_RAMRegister_4] &=  ClrBit2;

                if( CLEAR != flag)
                {
                  LCD->RAM[LCD_RAMRegister_4] |=  SetBit2;
                }
                else
                {
                   LCD->RAM[LCD_RAMRegister_4] &=  ClrBit2;
                }
            }
            break;
        case Mark_DC:
            {
//                LCD->RAM[LCD_RAMRegister_0] &=  ClrBit5;

                if(CLEAR != flag)
                {
                  LCD->RAM[LCD_RAMRegister_0] |=  SetBit5;
                }
                else
                {
                    LCD->RAM[LCD_RAMRegister_0] &=  ClrBit5;
                }
            }
            break;
        case Mark_PC:   // Mark_S3代表百分比标志
            {
//                LCD->RAM[LCD_RAMRegister_11] &=   ClrBit2;

                if( CLEAR != flag)
                {
                  LCD->RAM[LCD_RAMRegister_11] |=  SetBit2;
                }
                else
                {
                    LCD->RAM[LCD_RAMRegister_11] &=   ClrBit2;
                }
            }
            break;
        case Mark_PT:   // Mark_S3代表小数点
            {
//                LCD->RAM[LCD_RAMRegister_0] &=   ClrBit3;

                if( CLEAR != flag)
                {
                  LCD->RAM[LCD_RAMRegister_0] |=  SetBit3;
                }
                else
                {
                    LCD->RAM[LCD_RAMRegister_0] &=   ClrBit3;
                }
            }
            break;
        case Mark_FD:   // 代表 Fahrenheit degree 华氏度标志
            {
//                LCD->RAM[LCD_RAMRegister_0] &=  ClrBit5;

                if( CLEAR != flag)
                {
                    LCD->RAM[LCD_RAMRegister_0] |=  SetBit1;
                }
                else
                {
                    LCD->RAM[LCD_RAMRegister_0] &=  ClrBit5;
                }
            }
            break;
        case Mark_S0:   // Mark_S0 代表信号
            {
//                LCD->RAM[LCD_RAMRegister_7] &=   ClrBit3;

                if( CLEAR != flag)
                {
                  LCD->RAM[LCD_RAMRegister_7] |=  SetBit6;
                }
                else
                {
                    LCD->RAM[LCD_RAMRegister_7] &=   ClrBit6;
                }
            }
            break;
        case Mark_S4:   // Mark_S1 代表信号
            {
//                LCD->RAM[LCD_RAMRegister_11] &=   ClrBit3;

                if( CLEAR != flag)
                {
                  LCD->RAM[LCD_RAMRegister_11] |=  SetBit3;
                }
                else
                {
                    LCD->RAM[LCD_RAMRegister_11] &=   ClrBit3;
                }
            }
            break;
        case Mark_S3:   // Mark_S2 代表信号
            {
//                LCD->RAM[LCD_RAMRegister_7] &=   ClrBit7;

                if( CLEAR != flag)
                {
                  LCD->RAM[LCD_RAMRegister_7] |=  SetBit7;
                }
                else
                {
                    LCD->RAM[LCD_RAMRegister_7] &=   ClrBit7;
                }
            }
            break;
        case Mark_S2:   // Mark_S3 代表信号
            {
//                LCD->RAM[LCD_RAMRegister_4] &=   ClrBit3;

                if( CLEAR != flag)
                {
                  LCD->RAM[LCD_RAMRegister_4] |=  SetBit3;
                }
                else
                {
                    LCD->RAM[LCD_RAMRegister_4] &=   ClrBit3;
                }
            }
            break;

        case Mark_S1:   // Mark_S0 代表信号
            {
//                LCD->RAM[LCD_RAMRegister_0] &=   ClrBit7;

                if( CLEAR != flag)
                {
                  LCD->RAM[LCD_RAMRegister_0] |=  SetBit7;
                }
                else
                {
                    LCD->RAM[LCD_RAMRegister_0] &=   ClrBit7;
                }
            }
            break;
        default:
        break;
    }
}

/************************************************************************************
* Function Name:LCD_WE_NUM
* Purpose:      This function writes or Erase a NUM in the LCD frame buffer.
* Params:   	@param  ch: the numnber to dispaly.0 ~ 9
*           	@param  position: 第几个字【1-3】
* Return:  None
* Limitation:   Required preconditions: The LCD should be cleared before to start the
*               write operation.
*************************************************************************************/
void LCD_WE_NUM(uint8_t ConvertNum, uint8_t position, LCD_WE_FLAG flag)
{
    if(flag != CLEAR)
    {   // 将NumberMap码值16进制4bit方式赋值给Digit[]
        Digit[3] = (uint8_t)(NumberMap[ConvertNum] >> 12);
        Digit[2] = ((uint8_t)(NumberMap[ConvertNum] >> 8) & 0x0F);
        Digit[1] = ((uint8_t)(NumberMap[ConvertNum] >> 4) & 0x0F);
        Digit[0] = (((uint8_t)NumberMap[ConvertNum]) & 0x0F);
    }
    switch(position)
    {
        case 1:                                                         // 第一位8字
            {   // 首先清除第一个字的Seg
                LCD->RAM[LCD_RAMRegister_0] &=  ClrBit0;      		    // COM0--SEG
                LCD->RAM[LCD_RAMRegister_3] &=  ClrBit4;                // COM1--SEG
                LCD->RAM[LCD_RAMRegister_3] &=  ClrBit5;
                LCD->RAM[LCD_RAMRegister_7] &=  ClrBit0;                // COM2--SEG
                LCD->RAM[LCD_RAMRegister_7] &=  ClrBit1;
                LCD->RAM[LCD_RAMRegister_10] &=  ClrBit4;		        // COM3--SEG
                LCD->RAM[LCD_RAMRegister_10] &=  ClrBit5;

                if( CLEAR != flag) 	// 置位相应的Seg
                {
                    LCD->RAM[LCD_RAMRegister_0] |=  Digit[0];           // COM0--SEG
                    LCD->RAM[LCD_RAMRegister_3] |=  (Digit[1]<<4);      // COM1--SEG
                    LCD->RAM[LCD_RAMRegister_7] |=  Digit[2];           // COM2--SEG
                    LCD->RAM[LCD_RAMRegister_10] |=  (Digit[3]<<4);     // COM3--SEG
                }
            }
            break;
        case 2:                                                        //  第2位8字
            {
                LCD->RAM[LCD_RAMRegister_0] &=  ClrBit2;      		   // COM0--SEG
                LCD->RAM[LCD_RAMRegister_3] &=  ClrBit6;               // COM1--SEG
                LCD->RAM[LCD_RAMRegister_3] &=  ClrBit7;
                LCD->RAM[LCD_RAMRegister_7] &=  ClrBit2;               // COM2--SEG
                LCD->RAM[LCD_RAMRegister_7] &=  ClrBit3;
                LCD->RAM[LCD_RAMRegister_10] &=  ClrBit6;		       // COM3--SEG
                LCD->RAM[LCD_RAMRegister_10] &=  ClrBit7;

                if( CLEAR != flag)
                {
                LCD->RAM[LCD_RAMRegister_0] |=  (Digit[0]<<2);         // COM0--SEG
                LCD->RAM[LCD_RAMRegister_3] |=  (Digit[1]<<6);         // COM1--SEG
                LCD->RAM[LCD_RAMRegister_7] |=  (Digit[2]<<2);         // COM2--SEG
                LCD->RAM[LCD_RAMRegister_10] |= (Digit[3]<<6);         // COM3--SEG
                }
            }
            break;
        case 3:                                                       //  第3位8字
            {
                LCD->RAM[LCD_RAMRegister_0] &=  ClrBit4;      		   // COM0--SEG
                LCD->RAM[LCD_RAMRegister_4] &=  ClrBit0;               // COM1--SEG
                LCD->RAM[LCD_RAMRegister_4] &=  ClrBit1;
                LCD->RAM[LCD_RAMRegister_7] &=  ClrBit4;               // COM2--SEG
                LCD->RAM[LCD_RAMRegister_7] &=  ClrBit5;
                LCD->RAM[LCD_RAMRegister_11] &=  ClrBit0;		       // COM3--SEG
                LCD->RAM[LCD_RAMRegister_11] &=  ClrBit1;

                if( CLEAR != flag)
                {
                LCD->RAM[LCD_RAMRegister_0] |=  (Digit[0]<<4);         // COM0--SEG
                LCD->RAM[LCD_RAMRegister_4] |=  Digit[1];	           // COM1--SEG
                LCD->RAM[LCD_RAMRegister_7] |=  (Digit[2]<<4);	       // COM2--SEG
                LCD->RAM[LCD_RAMRegister_11] |=  Digit[3];	           // COM3--SEG
                }
            }
            break;
        default:
            break;
    }
}

//显示温度值 采集到的温度值乘以10为DispTemprature
void LCD_Display_T(uint16_t DispHumidity,LCD_WE_FLAG flag)
{
    uint8_t Display_Humidity_T = 0;      // 十位
    uint8_t Display_Humidity_U = 0;	     // 个位
    uint8_t Display_Humidity_D = 0;	     // 小数位

    //if((g_run_params.humi==0 && g_run_params.temp==0) || (g_run_params.humi==888 && g_run_params.temp==0) || g_run_params.st != 0)
    if((g_run_params.humi==0 && g_run_params.temp==0) ||  g_run_params.st != 0)
    {
        LCD_WE_NUM(10,1,flag);	// 显示 '-'
        LCD_WE_NUM(10,2,flag);	// 显示 '-'
        LCD_WE_NUM(10,3,flag);	// 显示 '-'

        LCD_DisplayMARK(Mark_PT,WRITE);     //小数点
        LCD_DisplayMARK(Mark_T, WRITE);     //温度计
        LCD_DisplayMARK(Mark_DC, WRITE);    // C 摄氏度标志

        LCD_DisplayMARK(Mark_RH, CLEAR);    //水滴
        LCD_DisplayMARK(Mark_PC, CLEAR);    //百分号
    }
    else
    {
        Display_Humidity_T = (uint8_t)((DispHumidity/100)%10);        // 十位
        Display_Humidity_U = (uint8_t)((DispHumidity/10)%10);         // 个位
        Display_Humidity_D = (uint8_t)(DispHumidity%10);              // 小数位

        LCD_WE_NUM(Display_Humidity_T,1,flag);	// 温度十位
        LCD_WE_NUM(Display_Humidity_U,2,flag);	// 温度个位
        LCD_WE_NUM(Display_Humidity_D,3,flag);	// 温度小数位

        LCD_DisplayMARK(Mark_PT,WRITE);     //小数点
        LCD_DisplayMARK(Mark_T, WRITE);     //温度计
        LCD_DisplayMARK(Mark_DC, WRITE);    // C 摄氏度标志

        LCD_DisplayMARK(Mark_RH, CLEAR);    //水滴
        LCD_DisplayMARK(Mark_PC, CLEAR);    //百分号
    }

}

//显示湿度值 采集到的温度值乘以10为DispTemprature
void LCD_Display_H(uint16_t DispTemprature,LCD_WE_FLAG flag)
{
    uint8_t Display_Temprature_T = 0;        // 十位
    uint8_t Display_Temprature_U = 0;	     // 个位
    uint8_t Display_Temprature_D = 0;	     // 小数位

    //if((g_run_params.humi==0 && g_run_params.temp==0) || (g_run_params.humi==888 && g_run_params.temp==0) ||  g_run_params.st != 0)
    if((g_run_params.humi==0 && g_run_params.temp==0) ||  g_run_params.st != 0)
    {
        LCD_WE_NUM(10,1,flag);	// 显示 '-'
        LCD_WE_NUM(10,2,flag);	// 显示 '-'
        LCD_WE_NUM(10,3,flag);	// 显示 '-'

        LCD_DisplayMARK(Mark_PT,WRITE);     //小数点
        LCD_DisplayMARK(Mark_RH, WRITE);    //水滴
        LCD_DisplayMARK(Mark_PC, WRITE);    //百分号

        LCD_DisplayMARK(Mark_T, CLEAR);     //温度计
        LCD_DisplayMARK(Mark_DC, CLEAR);    // C 摄氏度标志
    }
    else
    {
        Display_Temprature_T = (uint8_t)((DispTemprature/100)%10);        	  // 十位
        Display_Temprature_U = (uint8_t)((DispTemprature/10)%10); // 个位
        Display_Temprature_D = (uint8_t)(DispTemprature%10);               // 小数位

        LCD_WE_NUM(Display_Temprature_T,1,flag);	// 温度十位
        LCD_WE_NUM(Display_Temprature_U,2,flag);	// 温度个位
        LCD_WE_NUM(Display_Temprature_D,3,flag);	// 温度小数位

        LCD_DisplayMARK(Mark_PT,WRITE);     //小数点
        LCD_DisplayMARK(Mark_RH, WRITE);    //水滴
        LCD_DisplayMARK(Mark_PC, WRITE);    //百分号

        LCD_DisplayMARK(Mark_T, CLEAR);     //温度计
        LCD_DisplayMARK(Mark_DC, CLEAR);    // C 摄氏度标志
    }

}


void LCD_FullScreen(void)
{
    uint8_t counter = 0;

    for (counter = 0;counter < 0x0d; counter++)
    {
        LCD->RAM[counter] =  0xff;
    }
}

void LCD_ClearScreen(void)
{
    uint8_t counter = 0;

    for (counter = 0;counter < 22; counter++)
    {
        LCD->RAM[counter] =  LCD_RAM_RESET_VALUE;
    }
}

/*! \brief
*      显示信号状态
* \param disp_sts[IN]      - 是否显示 0-关闭(DISPLAY_OFF)；1-显示(DISPLAY_ON)
* \param ant_lv  [IN]        - 信号量 0~5
*/
void LCD_disp_ant(u8 ant_lv)
{
    //LCD_DisplayMARK(Mark_S0, CLEAR);
    LCD_DisplayMARK(Mark_S1, CLEAR);
    LCD_DisplayMARK(Mark_S2, CLEAR);
    LCD_DisplayMARK(Mark_S3, CLEAR);
    LCD_DisplayMARK(Mark_S4, CLEAR);

    // 显示信号
    LCD_DisplayMARK(Mark_S0, WRITE);
    switch(ant_lv)
    {
    case 4:
        LCD_DisplayMARK(Mark_S4, WRITE);
    case 3:
        LCD_DisplayMARK(Mark_S3, WRITE);
    case 2:
        LCD_DisplayMARK(Mark_S2, WRITE);
    case 1:
        LCD_DisplayMARK(Mark_S1, WRITE);
    default:
        break;
    }
    if(ant_lv>=5)
    {
        LCD_DisplayMARK(Mark_S1, WRITE);
        LCD_DisplayMARK(Mark_S2, WRITE);
        LCD_DisplayMARK(Mark_S3, WRITE);
        LCD_DisplayMARK(Mark_S4, WRITE);
    }
}

void LCD_poll_disp(void)
{
    s8 i = 0;

//    LCD_DisplayMARK(Mark_S0, WRITE);
//    delay_ms(500);
//    LCD_DisplayMARK(Mark_S1, WRITE);
//    delay_ms(500);
//    LCD_DisplayMARK(Mark_S2, WRITE);
//    delay_ms(500);
//    LCD_DisplayMARK(Mark_S3, WRITE);
//    delay_ms(500);
//    LCD_DisplayMARK(Mark_S4, WRITE);
//    delay_ms(500);

    for(i=0;i<5;i++)
    {
       LCD_DisplayMARK(i, WRITE);
       delay_ms(300);
    }


    LCD_DisplayMARK(Mark_T, WRITE);     //温度计
    delay_ms(240);
    LCD_DisplayMARK(Mark_RH, WRITE);    //水滴
    delay_ms(240);

//    LCD->RAM[LCD_RAMRegister_10] |=  SetBit4;		        // COM3--SEG
//    delay_ms(DISP_DELAY);
//    LCD->RAM[LCD_RAMRegister_10] |=  SetBit5;
//    delay_ms(DISP_DELAY);
//    LCD->RAM[LCD_RAMRegister_3]  |=  SetBit5;
//    delay_ms(DISP_DELAY);
//    LCD->RAM[LCD_RAMRegister_0]  |=  SetBit0;      		    // COM0--SEG
//    delay_ms(DISP_DELAY);
//    LCD->RAM[LCD_RAMRegister_3]  |=  SetBit4;                // COM1--SEG
//    delay_ms(DISP_DELAY);
//    LCD->RAM[LCD_RAMRegister_7]  |=  SetBit0;                // COM2--SEG
//    delay_ms(DISP_DELAY);
//    LCD->RAM[LCD_RAMRegister_7]  |=  SetBit1;
//    delay_ms(DISP_DELAY);


//    LCD->RAM[LCD_RAMRegister_10] |=  SetBit6;		       // COM3--SEG
//    delay_ms(DISP_DELAY);
//    LCD->RAM[LCD_RAMRegister_10] |=  SetBit7;
//    delay_ms(DISP_DELAY);
//    LCD->RAM[LCD_RAMRegister_3]  |=  SetBit7;
//    delay_ms(DISP_DELAY);
//    LCD->RAM[LCD_RAMRegister_0]  |=  SetBit2;      		   // COM0--SEG
//    delay_ms(DISP_DELAY);
//    LCD->RAM[LCD_RAMRegister_3]  |=  SetBit6;               // COM1--SEG
//    delay_ms(DISP_DELAY);
//    LCD->RAM[LCD_RAMRegister_7]  |=  SetBit2;               // COM2--SEG
//    delay_ms(DISP_DELAY);
//    LCD->RAM[LCD_RAMRegister_7]  |=  SetBit3;
//    delay_ms(DISP_DELAY);

    LCD_DisplayMARK(Mark_PT,WRITE);     //小数点
    delay_ms(300);

//    LCD->RAM[LCD_RAMRegister_11] |=  SetBit0;		       // COM3--SEG
//    delay_ms(DISP_DELAY);
//    LCD->RAM[LCD_RAMRegister_11] |=  SetBit1;
//    delay_ms(DISP_DELAY);
//    LCD->RAM[LCD_RAMRegister_4]  |=  SetBit1;
//    delay_ms(DISP_DELAY);
//    LCD->RAM[LCD_RAMRegister_0]  |=  SetBit4;      		   // COM0--SEG
//    delay_ms(DISP_DELAY);
//    LCD->RAM[LCD_RAMRegister_4]  |=  SetBit0;               // COM1--SEG
//    delay_ms(DISP_DELAY);
//    LCD->RAM[LCD_RAMRegister_7]  |=  SetBit4;               // COM2--SEG
//    delay_ms(DISP_DELAY);
//    LCD->RAM[LCD_RAMRegister_7]  |=  SetBit5;
//    delay_ms(DISP_DELAY);

    LCD_DisplayMARK(Mark_DC, WRITE);    // C 摄氏度标志
    delay_ms(240);
    LCD_DisplayMARK(Mark_PC, WRITE);    //百分号
    delay_ms(240);

    //显示数字
    for(i=9; i>=0; i--)
    {
        LCD_WE_NUM(i,1,WRITE);
        LCD_WE_NUM(i,2,WRITE);
        LCD_WE_NUM(i,3,WRITE);
        delay_ms(200);
    }
    LCD_FullScreen();//显示所有段
}

void LCD_disp_clearant(void)
{
    LCD_DisplayMARK(Mark_S0, CLEAR);
}