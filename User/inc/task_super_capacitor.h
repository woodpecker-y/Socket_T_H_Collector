#ifndef _TASK_SUPER_CAPACITOR_H
#define _TASK_SUPER_CAPACITOR_H

#define SUPER_CAPACITOR_RECHARGE_COUNTER  10
//#define SUPER_CAPACITOR_RECHARGE_IDLE_COUNTER   (100/RTC_FRE)

typedef enum _SuperCapacitorMode{
    E_SUPERCAPACITOR_MODE_NORMAL = 0,
    E_SUPERCAPACITOR_MODE_FAST,
    E_SUPERCAPACITOR_MODE_DIRECT_CONNECTION
}SuperCapacitorMode;

void task_super_capacitor_init(u8 have_lights_on);

void task_super_capacitor_proc(void);

u16 task_super_capacitor_read_voltage(void);

void task_super_capacitor_fast_triggered(void);

void task_super_capacitor_ctrl(u8 enable);

u8 task_super_capacitor_get_mode(void);

#endif