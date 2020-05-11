#ifndef _TASK_REPORT_H
#define _TASK_REPORT_H

#include "stm8l15x.h"

#define MAX_REBOOT_COUNT    3
#define MAX_WAIT_RESPONSE_TIMEOUT 10

// 模组入网初始化最大时间
#define MAX_CONNECT_TIMEOUT_NORMAL      70
#define MAX_CONNECT_TIMEOUT_FAST        90

#define MAX_CONNECT_FAILED_CNT          2

typedef enum _WorkMode{
    WORKMODE_NORMAL = 0,
    WORKMODE_STANDBY = 1
}WorkMode;

typedef struct _ReportHandler {
    u8      sts;
    u8      workmode;               // 工作模式
    u8      retry_cnt;              // 重试次数
    u8      powered_on;             // 上电状态 0关闭模块电源，1打开模块电源
    u8      connected_state;        // 联网状态
    u8      cmd;                    // 下发的命令类型
    u8      reboot_cnt;             // 重启次数
    u8      reboot_flag;            // 重启标志
    //u8      module_poweroff_flag;   // 模组断电标志
    s8      rtc_update_counter;     // rtc更新计数器，每联网12次更新一次时间
    u8      poweron_connect_counter;// 上电后初次选网次数
    s16     disp_timer;             // 显示定时器
    u16     send_mode;              // NON无需反馈，CON确认发送成功，正常发送（时间长要等待20多秒进入PSM模式一般不使用。)
    u16     upload_cnt;             // 本轮上报触发上传数据的次数
    s32     timer;                  // 时间计数器
    s32     timeout_timer;          // 超时计数器
    s32     max_connect_timeout;    // 最大联网超时时间
//    s16     network_search_battery_check_timer; // 网络搜索时超级电容电量检测定时器

//    u8      retry_flag;           // 重复入网标志 1-表示需要重新入网，0-表示不需要重新入网
//    u8      nbiot_work;           // 说明模块是否在工作

//    u16     nbiot_work_time;      // NB模块工作时间计数器

    u8      pkg_data[64];           // 数据包
    u16     pkg_size;               // 数据包长度

    u8      connect_failed_cnt;     // 联网连续失败次数，超过3次，重新切换频段查网
//    u8      pkg_temp[100];          //温湿度数据包
//    u16     pkg_temp_size;          //温湿度数据包长度

//    u8      rev_flag;               //是否接收到下发命令的标志，1-接收到，0-没接收到
//    u8      report_flag;            //上报标志位     0-表示不上报，直接结束   1-表示可以上报(原因没有在供暖期)
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
*       解包nbiot指令下发协议处理
* \param command[IN]        - 接收到的数据
* \param len[OUT]           - 数据长度

*/
int task_report_dealwith_command(u8 *command, u16 *len);
//
//void nbiot_work_time_compute(void);
//
//void nbiot_report_decide(void);

// 读取上报进程状态机的状态
u8 task_report_get_state(void);

u8 task_report_get_net_state(void);

/*! \brief
*       NB模块上报触发函数
*
*/
void task_report_triggered(u8 workmode);

void task_report_set_max_connect_timeout(s32 value);

#endif