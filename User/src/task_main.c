#include "task_main.h"
#include "task_list.h"
#include "wdg.h"
#include "adf.h"

// ����������
void task_main(void)
{
    u8 i = 0;

    // ��ʱ˳��ִ��
    for (i=0; i<sizeof(task_list)/sizeof(task_list[0]); ++i)
    {
        task_list[i].func();
        wdg_reset();
    }
}