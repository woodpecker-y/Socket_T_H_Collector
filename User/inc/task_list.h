#ifndef _TASK_LIST_H
#define _TASK_LIST_H
#include "task_disp.h"
#include "task_sample.h"
#include "task_report.h"
#include "task_pwm.h"

TaskItem_t task_list[] = {
    {1, task_report_proc},
    {2, task_sample_proc},
    {3, task_disp_proc},
    {4, task_pwm_proc}
};

#endif