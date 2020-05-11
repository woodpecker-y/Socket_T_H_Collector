#ifndef _TASK_DISP_H
#define _TASK_DISP_H

#include "sysparams.h"

typedef enum {
    E_TASK_DISP_IDLE = 0,     //����״̬
    E_TASK_DISP_MAIN_MENU,    //ˢ����Ļ
    E_TASK_DISP_ANT,           //ˢ���ź�ֵ

    E_TASK_DISP_ANT_IDLE,
    E_TASK_DISP_ANT_TWINKLE
}TASK_DISP_STATE;


typedef struct _TASK_DISP_HANDLER{
    u8          sts;            //״̬
    u16         disp_timer;     //ˢ�¶�ʱ��
    bool       temp_humi_flag; //ѭ��ˢ�±�־

    u8          sts_ant;
    u16         disp_ant_timer;
    bool        ant_flag;
    u16         twinkle_num;
}TASK_DISP_HANDLER;

/* ��ʾ�������� */
void task_disp_proc();

void task_disp_triggered(TASK_DISP_STATE sts);

u8 task_disp_calc_ant_level(u8 rssi);

#endif