#ifndef _TASK_PWM_H
#define _TASK_PWM_H

#include "sysparams.h"

typedef enum {
    E_TASK_PWM_IDLE = 0,     //空闲状态
    E_TASK_PWM_GET_ADC_VALUE,    //刷新屏幕
    E_TASK_PWM_REFRESH           //刷新信号值
}TASK_PWM_STATE;


typedef struct _TASK_PWM_HANDLER{
    u8          sts;            //状态
   u16         pwm_timer;     //刷新定时器
}TASK_PWM_HANDLER;

/* 显示的主进程 */
void task_pwm_proc();

void task_pwm_triggered(TASK_PWM_STATE sts);

#endif