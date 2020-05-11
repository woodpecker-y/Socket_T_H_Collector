#ifndef _TASK_PWM_H
#define _TASK_PWM_H

#include "sysparams.h"

typedef enum {
    E_TASK_PWM_IDLE = 0,     //����״̬
    E_TASK_PWM_GET_ADC_VALUE,    //ˢ����Ļ
    E_TASK_PWM_REFRESH           //ˢ���ź�ֵ
}TASK_PWM_STATE;


typedef struct _TASK_PWM_HANDLER{
    u8          sts;            //״̬
   u16         pwm_timer;     //ˢ�¶�ʱ��
}TASK_PWM_HANDLER;

/* ��ʾ�������� */
void task_pwm_proc();

void task_pwm_triggered(TASK_PWM_STATE sts);

#endif