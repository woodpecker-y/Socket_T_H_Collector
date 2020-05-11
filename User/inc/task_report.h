#ifndef _TASK_REPORT_H
#define _TASK_REPORT_H

#include "stm8l15x.h"

#define MAX_REBOOT_COUNT    3
#define MAX_WAIT_RESPONSE_TIMEOUT 10

// ģ��������ʼ�����ʱ��
#define MAX_CONNECT_TIMEOUT_NORMAL      70
#define MAX_CONNECT_TIMEOUT_FAST        90

#define MAX_CONNECT_FAILED_CNT          2

typedef enum _WorkMode{
    WORKMODE_NORMAL = 0,
    WORKMODE_STANDBY = 1
}WorkMode;

typedef struct _ReportHandler {
    u8      sts;
    u8      workmode;               // ����ģʽ
    u8      retry_cnt;              // ���Դ���
    u8      powered_on;             // �ϵ�״̬ 0�ر�ģ���Դ��1��ģ���Դ
    u8      connected_state;        // ����״̬
    u8      cmd;                    // �·�����������
    u8      reboot_cnt;             // ��������
    u8      reboot_flag;            // ������־
    //u8      module_poweroff_flag;   // ģ��ϵ��־
    s8      rtc_update_counter;     // rtc���¼�������ÿ����12�θ���һ��ʱ��
    u8      poweron_connect_counter;// �ϵ�����ѡ������
    s16     disp_timer;             // ��ʾ��ʱ��
    u16     send_mode;              // NON���跴����CONȷ�Ϸ��ͳɹ����������ͣ�ʱ�䳤Ҫ�ȴ�20�������PSMģʽһ�㲻ʹ�á�)
    u16     upload_cnt;             // �����ϱ������ϴ����ݵĴ���
    s32     timer;                  // ʱ�������
    s32     timeout_timer;          // ��ʱ������
    s32     max_connect_timeout;    // ���������ʱʱ��
//    s16     network_search_battery_check_timer; // ��������ʱ�������ݵ�����ⶨʱ��

//    u8      retry_flag;           // �ظ�������־ 1-��ʾ��Ҫ����������0-��ʾ����Ҫ��������
//    u8      nbiot_work;           // ˵��ģ���Ƿ��ڹ���

//    u16     nbiot_work_time;      // NBģ�鹤��ʱ�������

    u8      pkg_data[64];           // ���ݰ�
    u16     pkg_size;               // ���ݰ�����

    u8      connect_failed_cnt;     // ��������ʧ�ܴ���������3�Σ������л�Ƶ�β���
//    u8      pkg_temp[100];          //��ʪ�����ݰ�
//    u16     pkg_temp_size;          //��ʪ�����ݰ�����

//    u8      rev_flag;               //�Ƿ���յ��·�����ı�־��1-���յ���0-û���յ�
//    u8      report_flag;            //�ϱ���־λ     0-��ʾ���ϱ���ֱ�ӽ���   1-��ʾ�����ϱ�(ԭ��û���ڹ�ů��)
}ReportHandler;

typedef enum _REPORT_STATE{
    E_REPORT_IDLE = 0,
    E_REPORT_READY,
    E_REPORT_NET,
    E_REPORT_GETRSSI,
    E_REPORT_DATA_PACK,
    E_REPORT_REPORT,
    E_REPORT_RECVDATA,
    E_REPORT_FEEDBACK,
    E_REPORT_RAI_REPORT,
    E_REPORT_RAI_RECVDATA,
    E_REPORT_RAI_FEEDBACK,
    E_REPORT_DEAL,
    E_REPORT_REBOOT,
    E_REPORT_LOWPOWER_CONFIRM,
    E_REPORT_FINISH
}REPORT_STATE;

void task_report_init(void);

void task_report_proc(void);


/*! \brief
*       ���nbiotָ���·�Э�鴦��
* \param command[IN]        - ���յ�������
* \param len[OUT]           - ���ݳ���

*/
int task_report_dealwith_command(u8 *command, u16 *len);
//
//void nbiot_work_time_compute(void);
//
//void nbiot_report_decide(void);

// ��ȡ�ϱ�����״̬����״̬
u8 task_report_get_state(void);

u8 task_report_get_net_state(void);

/*! \brief
*       NBģ���ϱ���������
*
*/
void task_report_triggered(u8 workmode);

void task_report_set_max_connect_timeout(s32 value);

#endif