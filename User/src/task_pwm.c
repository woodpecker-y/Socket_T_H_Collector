//#include "disp.h"
#include "task_pwm.h"
#include "adf.h"
#include "sysparams.h"
#include "board.h"
#include "delay.h"
#include "error.h"
#include "AdcDriver.h"

static TASK_PWM_HANDLER s_task_pwm_handler;


void task_pwm_proc(void)
{
    s_task_pwm_handler.pwm_timer++;
    if(s_task_pwm_handler.pwm_timer>=UNIT_SECOND)
    {
        s_task_pwm_handler.pwm_timer = 0;
        task_pwm_triggered(E_TASK_PWM_GET_ADC_VALUE);
    }

    switch(s_task_pwm_handler.sts)
    {
    case E_TASK_PWM_IDLE:
        break;
    case E_TASK_PWM_GET_ADC_VALUE:
        GetilluminanceValue();           //获取亮度值
        s_task_pwm_handler.sts = E_TASK_PWM_REFRESH;
        break;
    case E_TASK_PWM_REFRESH:
        BacklightBrightnessAdjustment();
        s_task_pwm_handler.sts = E_TASK_PWM_IDLE;
        break;
    default:
        s_task_pwm_handler.sts = E_TASK_PWM_GET_ADC_VALUE;
        break;
    }
}

//显示触发
void task_pwm_triggered(TASK_PWM_STATE sts)
{
    s_task_pwm_handler.sts = sts;

    return;
}



