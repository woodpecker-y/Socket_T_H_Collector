#ifndef _TASK_SAMPLE_H
#define _TASK_SAMPLE_H

#include "stm8l15x.h"

#define MAX_IDLE_TEMP_TIMER  UNIT_SECOND*10

#define MAX_RETRY_CNT   3

typedef struct _CollectHandler{
    u8      sts;                    // 状态
    s32     refresh_timer;          // 系统参数周期采集
    s32     storage_timer;          // 存储定时器

    s32     sample_cycle;           // 采样存储周期
    s16     retry_cnt;              // 采集失败重试计数器
}CollectHandler;

typedef enum _COLLECT_STATE{
    E_COLLECT_IDLE = 0,             //空闲状态
    E_COLLECT_REDAY,                //准备状态
    E_COLLECT_ACTIVE,               //动作
    E_COLLECT_FINISH                //完成状态
}COLLECT_STATE;

/*参数的初始化*/
void task_sample_init(s32 sample_cycle);

/*采集进程*/
void task_sample_proc(void);

u8 task_sample_get_state(void);

void task_sample_triggered(void);

#endif