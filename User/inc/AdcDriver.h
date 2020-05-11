#ifndef __ADCDRIVER_H__
#define __ADCDRIVER_H__

#include "stm8l15x.h"
#include "stm8l15x_tim1.h"
#include "stm8l15x_adc.h"
//#include "main.h"
#include "stm8l15x_tim2.h"

#define ADC_IlluminationIntersityPort	GPIOD
#define ADC_IlluminationIntersityPin	GPIO_Pin_6
#define ADC_IlluminationIntersityMode   GPIO_Mode_Out_PP_Low_Fast

//背光开关
//FlagStatus BackLight_Status            SET     //背光默认开

//-------------------------------------内部参数---------------------------------
// 宏定义
#define VoltageRefInt   	    1224L  		    // 内部系统电压值

#define Ill_UpLimit             4000            // 光照度AD值上限 值越大越亮
#define Ill_DnLimit             1000            // 光照度AD值下限 值越小越暗

#define PULSEFREQUENCY          1000            // 脉冲频率1KHz

#define MinimumPulseWidth       0x0001          // 脉冲占空比下限 该值代表低电平数值

#define MaximumPulseWidth       0x0FA0          // 脉冲占空比上限 该值代表低电平数值


extern uint16_t BackLightPlusWidth;             // 最低脉冲宽度
extern uint16_t IlluminanceValue[10];           // 光照度AD值数组

void Timer2_Init(void);

void TIMER1_PWMInit(uint16_t PluseFreq,uint16_t PluseDutyCycle); // 定时器1初始化 PWM设置
void BubbleSort(uint16_t *SortDataArray);       // 冒泡排序法
void ADC_Initialization(void);
void ADC_DeInitialization(void);
void GetilluminanceValue(void);
void BacklightBrightnessAdjustment(void);

#endif   //  _AD1_DRIVER_H_
