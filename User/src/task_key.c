/* This file has been prepared for Doxygen automatic documentation generation.*/
/*! \file *********************************************************************
*
* \brief
*    application layer--key process
* \author
*    zhou_sm(at)blackants.com.cn
* \date
*    2014-07-16
* \version
*    v1.0
* Copyright (c) 2010-2014, Blackants Energy Saving Technology co., LTD All rights reserved.
******************************************************************************/
#include "adf.h"
#include "sysparams.h"
#include "board.h"
#include "task_key.h"
#include "task_disp.h"
#include "key.h"
//#include "light.h"
#include "task_super_capacitor.h"
#include "task_report.h"

u8 task_key_dealwith(void);

/*! \brief key status */
static KeyHandler s_key_handler;

/*! \brief
*       key process
*/
void task_key_proc(void)
{
    switch(s_key_handler.sts)
    {
    case KEY_STS_IDLE:
        break;
    case KEY_STS_PUSH:
        // �ж��Ƿ�Ϊ�Ϸ�����
        if (TRUE == key_check_valid(s_key_handler.key_val))
        {
            s_key_handler.sts = KEY_STS_VALID;
        }
        else
        {
            s_key_handler.sts = KEY_STS_IDLE;
        }
        break;
    case KEY_STS_VALID:
        // ��ⰴ���ͷ�
        if (TRUE == key_release(s_key_handler.key_val))
        {
            s_key_handler.sts = KEY_STS_DEAL;
        }
        // ����ע�ͣ����򰴼��޷�ʹ��
//        else
//        {
//            s_key_handler.sts = KEY_STS_IDLE;
//        }
        break;
    case KEY_STS_DEAL:
#ifdef LIGHT_DEBUG
        printf("dealwith\r\n");
#endif
        task_key_dealwith();
//        if (1 == task_key_dealwith())
//        {
            s_key_handler.feedback_timer = 0;
            s_key_handler.sts = KEY_STS_FEEDBACK;
//        }
//        else
//        {
//            s_key_handler.sts = KEY_STS_IDLE;
//        }
        delay_ms(25);
#if 0
        //MYLOG_DEBUG("task reader triggered!\r\n");
#endif
        break;
    case KEY_STS_FEEDBACK:
        s_key_handler.feedback_timer++;
        if (s_key_handler.feedback_timer >= MAX_FEEDBACK_TIMER)
        {
#ifdef LIGHT_DEBUG
            printf("counter:%d \r\n", s_key_handler.pluse_counter_light);
#endif
            //if (g_run_params.light_sts[E_LIGHT_1] == E_LIGHT_ON || g_run_params.light_sts[E_LIGHT_2] == E_LIGHT_ON)
            {
                exti_close_by_index(E_LIGHT_1);

//                if (g_sys_params.light_sts[E_LIGHT_1] == E_LIGHT_ON || g_sys_params.light_sts[E_LIGHT_2] == E_LIGHT_ON)
//                {
                    // ���ڹصƵ�����¿������
//                    task_super_capacitor_ctrl(0);
//                }
//                else
//                {
//                    // ���ڹصƵ�����¿������
//                    task_super_capacitor_ctrl(1);
//                }
                // L1��L2���޵Ƹ��ش���
                if (s_key_handler.pluse_counter_light < MAX_VAILD_PLUSE_COUNT)
                {
                    // �ر�L1��L2
                    light_off(E_LIGHT_1);
                    light_off(E_LIGHT_2);
                    g_sys_params.light_sts[E_LIGHT_1] = E_LIGHT_OFF;
                    g_sys_params.light_sts[E_LIGHT_2] = E_LIGHT_OFF;

                    // ���ڹصƵ�����¿������
                    task_super_capacitor_ctrl(1);
#ifdef HAVE_TEST
                    gpio_set_low(PORTE, PIN3);
                    gpio_set_low(PORTE, PIN4);
#endif
#ifdef LIGHT_DEBUG
                    printf("light1,light2 off\r\n");
#endif
                }
                else
                {
                    // ���ڿ��Ƶ�����¹رճ��
                    task_super_capacitor_ctrl(0);
                }

                // ����Ƶ�״̬
                sysparams_write(E_ADDR_OFFSET_LIGHT1_STATE, g_sys_params.light_sts, 2);
#ifdef HAVE_TEST
//                else
//                {
//                    gpio_set_high(PORTE, PIN3);
//                }
#endif

                //s_key_handler.pluse_counter_light = 0;
            }

//            if (g_run_params.light_sts[E_LIGHT_1] == E_LIGHT_OFF || g_run_params.light_sts[E_LIGHT_2] == E_LIGHT_OFF)
//            {
//                exti_close_by_index(E_LIGHT_2);
//
//                // L1��L2���޵Ƹ��ش���
//                if (s_key_handler.pluse_counter_light < MAX_VAILD_PLUSE_COUNT)
//                {
//                    light_off(E_LIGHT_2);
//                    g_run_params.light_sts[E_LIGHT_2] = E_LIGHT_OFF;
//#ifdef HAVE_TEST
//                    gpio_set_low(PORTE, PIN4);
//                    //printf("light2 off\r\n");
//#endif
//                }
//#ifdef HAVE_TEST
//                else
//                {
//                    gpio_set_high(PORTE, PIN4);
//                }
//#endif
//
//                //s_key_handler.pluse_counter_light = 0;
//            }

            s_key_handler.sts = KEY_STS_IDLE;
        }
        break;
    default:
        break;
    }
}

/*! \brief
*       key task init
*/
void task_key_init(void)
{
    memset(&s_key_handler, 0, sizeof(s_key_handler));
}

/*! \brief
*       key triggered, start key process
* \param key_code[IN]       - key value
*/
void task_key_triggered(u8 key_code)
{
    s_key_handler.key_val = key_code;
    s_key_handler.sts = KEY_STS_PUSH;
}

/*! \brief
*       key triggered, start key process
* \param key_code[IN]       - key value
*/
void task_key_triggered_manual(u8 light1_state, u8 light2_state)
{
    if (light1_state == 1)
    {
        light_on(E_LIGHT_1);
        g_sys_params.light_sts[E_LIGHT_1] = E_LIGHT_ON;
    }

    if (light2_state == 1)
    {
        light_on(E_LIGHT_2);
        g_sys_params.light_sts[E_LIGHT_2] = E_LIGHT_ON;
    }

    //if (light2_state == 1 || light1_state == 1)
    {
        delay_ms(8);
        exti_init_by_index(E_LIGHT_1);
        s_key_handler.pluse_counter_light = 0;

        s_key_handler.feedback_timer = 0;
        s_key_handler.sts = KEY_STS_FEEDBACK;
    }
}

/*! \brief
*       get key task run state
* \return
*       key task run state
*/
u8 task_key_get_state(void)
{
    return s_key_handler.sts;
}

u8 task_key_dealwith(void)
{

    switch(s_key_handler.key_val)
    {
    case KEY_1:
        if (g_sys_params.light_sts[E_LIGHT_1] == E_LIGHT_OFF)
        {
            //printf("light1 on\r\n");
            light_on(E_LIGHT_1);
            g_sys_params.light_sts[E_LIGHT_1] = E_LIGHT_ON;

#ifdef LIGHT_DEBUG
            printf("light1 on\r\n");
#endif
#ifdef HAVE_TEST
            // ��������;���ж��Ƿ����ѿ���
            gpio_set_high(PORTE, PIN3);
#endif
//            return 1;
//            s_key_handler.feedback_timer = 0;
//            s_key_handler.sts = KEY_STS_FEEDBACK;
        }
        else
        {
            //printf("light1 off\r\n");
            light_off(E_LIGHT_1);
            g_sys_params.light_sts[E_LIGHT_1] = E_LIGHT_OFF;
#ifdef LIGHT_DEBUG
            printf("light1 off\r\n");
#endif

#ifdef HAVE_TEST
            // ��������;���ж��Ƿ����ѿ���
            gpio_set_low(PORTE, PIN3);
#endif
//            s_key_handler.sts = KEY_STS_IDLE;

//            return 1;
        }

        delay_ms(8);
        exti_init_by_index(E_LIGHT_1);
        s_key_handler.pluse_counter_light = 0;
        break;
    case KEY_2:
        if (g_sys_params.light_sts[E_LIGHT_2] == E_LIGHT_OFF)
        {
            //printf("light2 on\r\n");
            light_on(E_LIGHT_2);
            g_sys_params.light_sts[E_LIGHT_2] = E_LIGHT_ON;

#ifdef LIGHT_DEBUG
            printf("light2 on\r\n");
#endif
#ifdef HAVE_TEST
            // ��������;���ж��Ƿ����ѿ���
            gpio_set_high(PORTE, PIN4);
#endif
//            s_key_handler.feedback_timer = 0;
//            s_key_handler.sts = KEY_STS_FEEDBACK;

//            return 1;
        }
        else
        {
            //printf("light2 off\r\n");
            light_off(E_LIGHT_2);
            g_sys_params.light_sts[E_LIGHT_2] = E_LIGHT_OFF;
#ifdef LIGHT_DEBUG
            printf("light2 off\r\n");
#endif

#ifdef HAVE_TEST
            // ��������;���ж��Ƿ����ѿ���
            gpio_set_low(PORTE, PIN4);
#endif
//            s_key_handler.sts = KEY_STS_IDLE;
//            return 1;
        }

        delay_ms(8);
        exti_init_by_index(E_LIGHT_1);
        s_key_handler.pluse_counter_light = 0;
        break;
    case KEY_COMB_1:
        // ������ʾ����
        task_disp_triggered(E_TASK_DISP_ID_MENU);

        // ����δ������״̬�´������
        if (task_report_get_net_state() == 0)
        {
            // ���ģʽ���������
            task_super_capacitor_ctrl(1);
            // �������
            task_super_capacitor_fast_triggered();
        }

        break;
    default:
#ifdef LIGHT_DEBUG
            printf("comb key\r\n");
#endif
//        // �ر�L1��L2
//        light_off(E_LIGHT_1);
//        light_off(E_LIGHT_2);
//        g_sys_params.light_sts[E_LIGHT_1] = E_LIGHT_OFF;
//        g_sys_params.light_sts[E_LIGHT_2] = E_LIGHT_OFF;
        return 1;
        break;
    }

    return 1;
}

void task_key_set_pluse_counter(u8 idx)
{
    s_key_handler.pluse_counter_light++;
}