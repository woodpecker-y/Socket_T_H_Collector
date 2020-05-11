#ifndef _TASK_SAMPLE_H
#define _TASK_SAMPLE_H

#include "stm8l15x.h"

#define MAX_IDLE_TEMP_TIMER  UNIT_SECOND*10

#define MAX_RETRY_CNT   3

typedef struct _CollectHandler{
    u8      sts;                    // ״̬
    s32     refresh_timer;          // ϵͳ�������ڲɼ�
    s32     storage_timer;          // �洢��ʱ��

    s32     sample_cycle;           // �����洢����
    s16     retry_cnt;              // �ɼ�ʧ�����Լ�����
}CollectHandler;

typedef enum _COLLECT_STATE{
    E_COLLECT_IDLE = 0,             //����״̬
    E_COLLECT_REDAY,                //׼��״̬
    E_COLLECT_ACTIVE,               //����
    E_COLLECT_FINISH                //���״̬
}COLLECT_STATE;

/*�����ĳ�ʼ��*/
void task_sample_init(s32 sample_cycle);

/*�ɼ�����*/
void task_sample_proc(void);

u8 task_sample_get_state(void);

void task_sample_triggered(void);

#endif