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

//���⿪��
//FlagStatus BackLight_Status            SET     //����Ĭ�Ͽ�

//-------------------------------------�ڲ�����---------------------------------
// �궨��
#define VoltageRefInt   	    1224L  		    // �ڲ�ϵͳ��ѹֵ

#define Ill_UpLimit             4000            // ���ն�ADֵ���� ֵԽ��Խ��
#define Ill_DnLimit             1000            // ���ն�ADֵ���� ֵԽСԽ��

#define PULSEFREQUENCY          1000            // ����Ƶ��1KHz

#define MinimumPulseWidth       0x0001          // ����ռ�ձ����� ��ֵ����͵�ƽ��ֵ

#define MaximumPulseWidth       0x0FA0          // ����ռ�ձ����� ��ֵ����͵�ƽ��ֵ


extern uint16_t BackLightPlusWidth;             // ���������
extern uint16_t IlluminanceValue[10];           // ���ն�ADֵ����

void Timer2_Init(void);

void TIMER1_PWMInit(uint16_t PluseFreq,uint16_t PluseDutyCycle); // ��ʱ��1��ʼ�� PWM����
void BubbleSort(uint16_t *SortDataArray);       // ð������
void ADC_Initialization(void);
void ADC_DeInitialization(void);
void GetilluminanceValue(void);
void BacklightBrightnessAdjustment(void);

#endif   //  _AD1_DRIVER_H_
