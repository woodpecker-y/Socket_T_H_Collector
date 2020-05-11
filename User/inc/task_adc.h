#ifndef _TASK_ADC_H
#define _TASK_ADC_H

typedef enum _E_ADC_STATE{
    E_ADC_IDLE = 0,
    E_ADC_READY,
    E_ADC_BATT,
    E_ADC_INLET,
    E_ADC_RETURN,
    E_ADC_FINISH
}E_ADC_STATE;

typedef enum _E_ADC_EXEC_STATE{
    E_ADC_EXEC_INIT = 0,
    E_ADC_EXEC_FINISH = 1
}E_ADC_EXEC_STATE;

//#define MAX_SAMPLE_COUNT 8

void task_adc_proc(void);

//void task_adc_triggered(void);
//
//u8  task_adc_get_state(void);
//
//u8  task_adc_get_exec_state(void);
//
u32 task_read_adc_value(u8 idx);
//
//void temp_calibration(void);

#endif