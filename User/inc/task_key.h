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
#ifndef _TASK_KEY_H
#define _TASK_KEY_H

// ��ʽ����
#ifndef LIGHT_DEBUG
#define MAX_FEEDBACK_TIMER      (UNIT_SECOND*2)
#else
// ���Ի���
#define MAX_FEEDBACK_TIMER      (UNIT_SECOND*2)
#endif

#define MAX_VAILD_PLUSE_COUNT   20

/*! \brief key status definition */
typedef enum _KeySts{
    KEY_STS_IDLE = 0,               // ����״̬
    KEY_STS_PUSH,                   // ��������
    KEY_STS_VALID,                  // �����Ϸ�
    KEY_STS_DEAL,                   // ��������
    KEY_STS_CHECKSTART,             // ��ʼ���
    KEY_STS_FEEDBACK                // ����ȷ��
}KeySts;

/*! \brief ������� */
typedef struct _KeyHandler{
    u8  sts;                    // ����״̬
    u8  key_val;                // ������ֵ
    u16 pluse_counter_light; // ���������
    u32 feedback_timer;         // ����������
}KeyHandler;

/*! \brief
*       key process
*/
void task_key_proc(void);

/*! \brief
*       key task init
*/
void task_key_init(void);

/*! \brief
*       key triggered, start key process
* \param key_code[IN]       - key value
*/
void task_key_triggered(u8 key_code);

void task_key_triggered_manual(u8 light1_state, u8 light2_state);

/*! \brief
*       get key task run state
* \return
*       key task run state
*/
u8 task_key_get_state(void);

// ���������
void task_key_set_pluse_counter(u8 idx);

#endif