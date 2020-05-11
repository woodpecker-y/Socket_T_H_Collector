//#include "disp.h"
#include "task_disp.h"
#include "adf.h"
#include "sysparams.h"
#include "board.h"
#include "delay.h"
#include "error.h"
#include "LCD.h"
#include "task_report.h"

static TASK_DISP_HANDLER s_task_disp_handler;

//��ʾ�˵�
void task_disp_main_menu(void);
//�źŵȼ�
u8 task_disp_calc_ant_level(u8 rssi);
//��صȼ�
//u8 task_disp_calc_battery_percent(u16 value);
void task_disp_ant_menu(void);

//bool fldffd;

void task_disp_proc(void)
{
    u8 ant_level = 0;

    s_task_disp_handler.disp_timer++;
    if(s_task_disp_handler.disp_timer >= 10*UNIT_SECOND)
    {
        task_disp_triggered(E_TASK_DISP_MAIN_MENU);//ѭ��ˢ����ʪ��
        s_task_disp_handler.disp_timer = 0;
    }

    switch(s_task_disp_handler.sts)
    {
    case E_TASK_DISP_IDLE:      // ����״̬

        break;
    case E_TASK_DISP_MAIN_MENU:   //ˢ��
        task_disp_main_menu();
        s_task_disp_handler.sts = E_TASK_DISP_IDLE;
        break;
    case E_TASK_DISP_ANT:
        ant_level = task_disp_calc_ant_level(g_run_params.rssi);
        LCD_disp_ant(ant_level);
        break;
    default:
        break;
    }


    if( task_report_get_state() >= E_REPORT_READY )
    {
        s_task_disp_handler.disp_ant_timer++;
        if(s_task_disp_handler.disp_ant_timer >= UNIT_SECOND/2)
        {
            s_task_disp_handler.disp_ant_timer = 0;
            s_task_disp_handler.sts_ant = E_TASK_DISP_ANT_TWINKLE;
        }

        switch(s_task_disp_handler.sts_ant)
        {
        case E_TASK_DISP_ANT_IDLE://����

            break;
        case E_TASK_DISP_ANT_TWINKLE://��˸
            task_disp_ant_menu();
            s_task_disp_handler.sts_ant = E_TASK_DISP_ANT_IDLE;
            break;
        default:
            break;
        }
    }
    else
    {
        s_task_disp_handler.twinkle_num++;
        if(s_task_disp_handler.twinkle_num >= UNIT_SECOND)
        {
            LCD_DisplayMARK(Mark_S0, WRITE);
            s_task_disp_handler.twinkle_num = 0;
        }
    }

}

//��ʾ����
void task_disp_triggered(TASK_DISP_STATE sts)
{
    s_task_disp_handler.sts = sts;

    s_task_disp_handler.disp_timer = 9*UNIT_SECOND;
    return;
}

//ˢ����Ļ
void task_disp_main_menu(void)
{
    //��ʱ�л�ˢ���¶����ݺ�ʪ������
    if(s_task_disp_handler.temp_humi_flag == TRUE)
    {
        s_task_disp_handler.temp_humi_flag = FALSE;
        LCD_Display_H(g_run_params.humi, WRITE);//��ʾʪ������
    }
    else
    {
        s_task_disp_handler.temp_humi_flag = TRUE;
        LCD_Display_T(g_run_params.temp, WRITE); //��ʾ�¶�����

#if 0   //ʵʱ��ӡϵͳʱ��
        rtc_read_bcd(g_sys_params.t, 7);
        printf("\r\n%02X%02X-%02X-%02X %02X:%02X:%02X - disp\r\n",g_sys_params.t[0],g_sys_params.t[1],g_sys_params.t[2],g_sys_params.t[3],g_sys_params.t[4],g_sys_params.t[5],g_sys_params.t[6]);
#endif
    }
}

//��˸�ź�
void task_disp_ant_menu(void)
{
    u8 ant_level = 0;

    if(s_task_disp_handler.ant_flag == TRUE)
    {
        s_task_disp_handler.ant_flag = FALSE;
        ant_level = task_disp_calc_ant_level(g_run_params.rssi);
        LCD_disp_ant(ant_level);
    }
    else
    {
        s_task_disp_handler.ant_flag = TRUE;
        LCD_disp_clearant();
    }
}

//�źŵȼ��ж�     һ   <15      ��  15--20      ��   21--23     ��   >=24
u8 task_disp_calc_ant_level(u8 rssi)
{
    if (rssi >= 15)
    {
        return (rssi - 15)/3 + 1;
    }
    else if (rssi > 0)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

////��ذٷְ��ж�
//u8 task_disp_calc_battery_percent(u16 value)
//{
//    if (value >= 50)
//    {
//        return 99;
//    }
//    else
//    {
//        return value*2;
//    }
//}



