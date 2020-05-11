#ifndef _TASK_MAIN_H
#define _TASK_MAIN_H

#include "adf.h"

// 任务句柄
typedef struct TaskItem{
    u8   priority;
    void (*func)(void);
}TaskItem_t;

// 任务主进程
void task_main(void);

#endif