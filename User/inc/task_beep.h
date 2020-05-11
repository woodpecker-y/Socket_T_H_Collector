#ifndef _TASK_BEEP_H
#define _TASK_BEEP_H

typedef enum _BeepState{
    E_BEEP_IDLE = 0,
    E_BEEP_ON,
    E_BEEP_ON_DELAY,
    E_BEEP_OFF,
    E_BEEP_OFF_DELAY
}BeepState;

typedef struct _BeepHandler{
    u8 sts;
    u8  cnt;    
    u16 timer;
    u16 timeout;
}BeepHandler;

void task_beep_triggered(u8 cnt, u8 timer);

void task_beep_proc(void);

#endif