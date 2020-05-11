#define  __INTADC1_DRIVER_C__

#include "AdcDriver.h"
#include "delay.h"
#include "sysparams.h"

uint16_t BackLightPlusWidth = PULSEFREQUENCY;   // �������Ƶ��
uint16_t IlluminanceValue[10] = {0};            // ���ն�ADֵ����


void Timer2_Init(void)
{
  //ʱ������
  CLK_PeripheralClockConfig(CLK_Peripheral_TIM2,ENABLE);

  //1��Ƶ�����ϼ�����ÿ13us��ʱ�ж�һ��
  TIM2_DeInit();
  //TIM2_TimeBaseInit(TIM2_Prescaler_1, TIM2_CounterMode_Up, 1000*40*2);
  TIM2_TimeBaseInit(TIM2_Prescaler_1, TIM2_CounterMode_Up, 60000);
  TIM2_ARRPreloadConfig(ENABLE);
  TIM2_ITConfig(TIM2_IT_Update , ENABLE);
  TIM2_Cmd(ENABLE);
}


/*******************************************************************************
* ��    �ƣ�void TIMER1_PWMInit(uint16_t PlusFreq,uint16_t PlusWidth)
* ��    �ܣ�stm8��ʱ��1��ʼ�����ڲ���PWM���� ����LED��������
* ��ڲ�����PlusFreq:����Ƶ��
*           PluseDutyCycle:����ռ�ձ� ����ֵ��ʾ�͵�ƽ��ֵ
* ���ڲ�������
* ˵    ����ͨ��1���pwm��
* ����˵������
*******************************************************************************/
void TIMER1_PWMInit(uint16_t PluseFreq,uint16_t PluseDutyCycle)
{
    //PB0ͨ��IO����Ϊ���
    GPIO_Init(GPIOD, GPIO_Pin_5, GPIO_Mode_Out_PP_Low_Fast);

    CLK_PeripheralClockConfig(CLK_Peripheral_TIM1,ENABLE);//����TIM1ʱ��
    TIM1_DeInit();
    //��Ƶ1600�����ϼ�����TIM1_ARR=20000,�ظ�������Ϊ0
    TIM1_TimeBaseInit(15,TIM1_CounterMode_Up,2000,0);

    TIM1_OC3Init(TIM1_OCMode_PWM1,
                 TIM1_OutputState_Enable,
                 TIM1_OutputNState_Disable,
                 PluseDutyCycle,                     // ����ռ�ձ�
                 TIM1_OCPolarity_Low,
                 TIM1_OCNPolarity_Low,
                 TIM1_OCIdleState_Set,
                 TIM1_OCNIdleState_Set);

   TIM1_OC3PreloadConfig(ENABLE);
   TIM1_CtrlPWMOutputs(ENABLE); //PWM���ʹ��
   TIM1_Cmd(ENABLE); //��ʱ����ʼ����
}

/*******************************************************************************
* Function Name:BubbleSort
* Purpose:      AD��ʼ��
* Params:  	NONE
* Return:  	NONE
* Limitation:
*******************************************************************************/
void BubbleSort(uint16_t *SortDataArray)
{
    uint8_t i,j;

    for(i=0; i<9; i++)
    {
        for(j=0; j<9-i; j++)
        {
            uint16_t DataTemp;
            if(*SortDataArray+j > *SortDataArray+j+1)
            {
                DataTemp = *(SortDataArray+j);

                *(SortDataArray+j) = *(SortDataArray+j+1);

                *(SortDataArray+j+1) = DataTemp;
            }
        }
    }
}

/*******************************************************************************
* Function Name:ADC_Initialization
* Purpose:      AD��ʼ��
* Params:  	NONE
* Return:  	NONE
* Limitation:
*******************************************************************************/
void ADC_Initialization(void)
{
    // ʹ��ADC_IN3����
    GPIO_Init(ADC_IlluminationIntersityPort, ADC_IlluminationIntersityPin, GPIO_Mode_In_FL_No_IT);

    // Enable ADC1 clock
    CLK_PeripheralClockConfig(CLK_Peripheral_ADC1, ENABLE);

    // Initialise and configure ADC1
    ADC_Init(ADC1, ADC_ConversionMode_Single, ADC_Resolution_12Bit, ADC_Prescaler_2);

    ADC_SamplingTimeConfig(ADC1, ADC_Group_SlowChannels, ADC_SamplingTime_384Cycles);

    // Enable ADC1
    ADC_Cmd(ADC1, ENABLE);

    // Enable End of conversion ADC1 Interrupt
    ADC_ITConfig(ADC1, ADC_IT_EOC, DISABLE);

    // Start ADC1 Conversion using Software trigger
    ADC_SoftwareStartConv(ADC1);
}

/*******************************************************************************
* Function Name:ADC_DeInit
* Purpose:      ADC�Ĵ����ظ���ʼֵ
* Params:  		NONE
* Return:  		NONE
* Limitation:
*******************************************************************************/
void  ADC_DeInitialization(void)
{
    ADC_Cmd(ADC1,DISABLE);
    CLK_PeripheralClockConfig(CLK_Peripheral_ADC1, DISABLE);
}

/*******************************************************************************
* Function Name:GetilluminanceValue
* Purpose:      ��������������ն�ֵ
* Params:  	NONE
* Return:  	NONE
* Limitation:
*******************************************************************************/
void GetilluminanceValue(void)
{
    uint32_t  ADC_REF_INT = 0;
    uint8_t   i = 10;
    uint32_t  Loops = 0x0FFF;

    // Enable ADC1 Channel ADC_Channel_Vrefint
    ADC_VrefintCmd(ENABLE);

    delay_us(5);
    ADC_ChannelCmd(ADC1, ADC_Channel_Vrefint, ENABLE);
    ADC_SoftwareStartConv(ADC1);

    // Wait until End-Of-Convertion
    while( 0 == ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC))
    {
        if(0 == Loops--) break;
    }

    // Get ADC convertion value
    ADC_REF_INT= ADC_GetConversionValue(ADC1);

    // TSON��λ
    ADC_VrefintCmd(DISABLE);
    ADC_ChannelCmd(ADC1, ADC_Channel_Vrefint,DISABLE);//�ر�ͨ��

    // ���������ѹֵ
    ADC_ChannelCmd(ADC1, ADC_Channel_8, ENABLE);

    while(i--)
    {
        // Enable ADC1 Channel 21
        ADC_SoftwareStartConv(ADC1);

        // Wait until End-Of-Convertion
        Loops = 0x0FFF;
        while(RESET ==ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC))
        {
            if(0 == Loops--) break;
        }
        // Get ADC convertion value
        IlluminanceValue[i] = ADC_GetConversionValue(ADC1);
    }

    // ��10�β����Ĺ��ն�ֵ����ð������
    BubbleSort(IlluminanceValue);

    IlluminanceValue[0] = 0;

    for(i = 0; i < 9; i++)//��ƽ��ֵ
        IlluminanceValue[0] += IlluminanceValue[i];

    IlluminanceValue[1] = (uint16_t)(IlluminanceValue[0]/6);

    // �ر�ͨ��
    ADC_ChannelCmd(ADC1, ADC_Channel_24, DISABLE);

    // ���������ѹֵ
    IlluminanceValue[9] = (uint16_t)((VoltageRefInt*IlluminanceValue[1])/(uint32_t)ADC_REF_INT);
    //IlluminanceValue[9] = 3770;
    
//    printf("ADC_value=%d\r\n", IlluminanceValue[9]);//��ӡ�ɼ���ADֵ
}

uint8_t H,L;
/*******************************************************************************
* ��    �ƣ�void BacklightBrightnessAdjustment(void)
* ��    �ܣ�����PWM������ռ��
* ��ڲ�����BacklightValue ������ռ��ֵ
* ���ڲ�������
* ˵    ������
* ����˵������
*******************************************************************************/
float Ill_Proportion;
void BacklightBrightnessAdjustment(void)
{
    float minvaule = 200.0;

    // ��������ȼ���ѹֵ�������� ǿ��ת��Ϊ����ֵ
    if(IlluminanceValue[9] >= Ill_UpLimit)
        IlluminanceValue[9] = Ill_UpLimit;

    // ��������ȼ���ѹֵ�������� ǿ��ת��Ϊ����ֵ
    if(IlluminanceValue[9] <= Ill_DnLimit)
        IlluminanceValue[9] = Ill_DnLimit;

    // �����նȱ��������������ռ�ձ�
    //Ill_Proportion = 2000 - ( (3700.0-(float)IlluminanceValue[9])*2000.0/1000.0 );//1280
    Ill_Proportion = 2000 - ( (3700.0-(float)IlluminanceValue[9])*2 );

    if(Ill_Proportion < minvaule)
      Ill_Proportion = minvaule;
    
    BackLightPlusWidth = (uint16_t)Ill_Proportion;
    
//    printf("Ill_Proportion=%f\r\n", Ill_Proportion);//������ֵ
    
    //BackLightPlusWidth = 1800;
#if 1
    if(Ill_Proportion <= 200)
    {
        g_run_params.backlight_v = 1;
        BackLightPlusWidth = 200;
    }
    else if(Ill_Proportion>200 && Ill_Proportion<=700)
    {
        g_run_params.backlight_v = 2;
        BackLightPlusWidth = 450;
    }
    else if(Ill_Proportion>700 && Ill_Proportion<=1200)
    {
        g_run_params.backlight_v = 3;
        BackLightPlusWidth = 950;
    }
    else if(Ill_Proportion>1200 && Ill_Proportion<=1700)
    {
        g_run_params.backlight_v = 4;
        BackLightPlusWidth = 1450;
    }
    else if(Ill_Proportion>1700 && Ill_Proportion<=2200)
    {
        g_run_params.backlight_v = 5;
        BackLightPlusWidth = 2000;
    }
    else//�����ȼ���
    {
        g_run_params.backlight_v = 6;
        BackLightPlusWidth = 2000;
    }
#endif
    
    //printf("BackLight_Width=%d\r\n\r\n\r\n", BackLightPlusWidth);//������ֵ
    
    // ��������ռ�ձ�ֵ
    TIM1->CCR3H = (uint8_t)(BackLightPlusWidth >> 8);
    TIM1->CCR3L = (uint8_t)(BackLightPlusWidth);

//    TIM1->CCR3H = 0x00;
//    TIM1->CCR3L = 0x00;
}
/*---------------------------------------------Document end----------------------------------------*/