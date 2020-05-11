#ifndef _SYS_PARAMS_H
#define _SYS_PARAMS_H

#include "adf.h"
#include "board.h"

#define TEMPERATURE_COMPENSATION       0  //温度补偿开关    0关闭   1打开

#define CYCLE_AWU                      101UL
#define UNIT_SECOND	                   20UL

//#define CYCLE_AWU                     256UL
//#define UNIT_SECOND	                8UL
#define RTC_FRE                         (CYCLE_AWU/2)

#define UNIT_MIN		                (60*UNIT_SECOND)
#define UNIT_HOUR		                (24*UNIT_MIN)

#define BAND_CNT                        3

//#define UNIT_KWH                        0x05    /*!< \brief KWH */
//#define UNIT_GJ                         0x11    /*!< \brief GJ */

//#define AWU_TIMER    15                 // ~50ms
//
//#define RTC_UPDATE_CYCLE                (UNIT_SECOND*60*60UL)

//typedef enum _DevType{
//    E_DEV_TYPE_ELEC_VALVE = 0x01,
//    E_DEV_TYPE_V5,
//    E_DEV_TYPE_V1,
//}DevType;

//typedef enum _eValveCtrl{
//    E_VALVE_MODE_OPEN  = 0x55,
//    E_VALVE_MODE_CLOSE = 0x99,
//    E_VALVE_MODE_AUTO  = 0x59,
//}eValveCtrl;
//
//typedef enum _ValveState{
//    E_VALVE_STATE_OPENED = 0x00,
//    E_VALVE_STATE_CLOSED = 0x01,
//    E_VALVE_STATE_ABNORMAL = 0x03,
//}ValveState;

//typedef enum _HPState{
//    E_HP_IN = 1,
//    E_HP_OUT = 0
//}HPState;
//
//typedef enum _LightState{
//    E_LIGHT_ON = 1,
//    E_LIGHT_OFF = 0
//}LightState;

//typedef struct _RechargeInfoHP{
//    u16     recharge_sn;
//    u8      hp_st[4];
//    u8      hp_et[4];
//}RechargeInfoHP;
//
//typedef union _RechargeInfo{
//    RechargeInfoHP  hp_info;
//}RechargeInfo;

#define LENGTH_ADDR 16

typedef enum _AddrOffset{
    E_ADDR_OFFSET_FACTORY = 0,          //出厂标识
    E_ADDR_OFFSET_DEV_TYPE = 1,         //设备类型
    E_ADDR_OFFSET_DEV_ADDR = 2,         //设备ID_IMEI-16
    E_ADDR_OFFSET_IP = 18,              //IP-16
    E_ADDR_OFFSET_PORT = 34,            //port-2
    E_ADDR_OFFSET_APN = 36,             //APN-20
    E_ADDR_OFFSET_FIX_TEMP = 56,
    E_ADDR_OFFSET_FIX_HUMI = 57,
    //58    59
    E_ADDR_OFFSET_UPLOAD_CYCLE = 60,    //上传周期
    E_ADDR_OFFSET_SAMPLE_CYCLE  = 62,   //采集周期
    E_ADDR_OFFSET_TIME = 64,            //时间
    E_ADDR_OFFSET_RECORDS_READ = 71,    //读标志
    E_ADDR_OFFSET_RECORDS_WRITE = 75,   //写标志
//    E_ADDR_OFFSET_LIGHT1_STATE = 79,    //
//    E_ADDR_OFFSET_LIGHT2_STATE = 80,    //
    E_ADDR_OFFSET_BAND_NO = 81,         //BAND
//    E_ADDR_OFFSET_BAND_RSSI = 82,
//    E_ADDR_OFFSET_HAVE_CONNECTED = 82
}AddrOffset;

// 系统参数，需要存储与EEPROM中,系统参数区必须控制在85Bytes以内，否则会覆盖历史记录区
typedef struct _SysParams{
    u8              factory_flag;			                    // 出厂标识 - 0
    u8              dev_type;                                   // 设备类型代码 - 1 : 'T'- 电信版程序代码;   'U'- 联通版程序
    u8              addr[LENGTH_ADDR];                          // 设备地址 - 2

    char            ip[16];                                     // IP地址 -18
    u16             port;                                       // 端口号 - 34
    char            apn[20];                                    // APN  - 36
     s8             fix_temp;                                   //温度补偿值
     s8             fix_humi;                                   //湿度补偿值

    u16             _num;
    u16             upload_cycle;                               // 上报周期  - 60  单位为分钟
    u16             sample_cycle;                               // 采集周期  - 62
    u8              t[7];                                       // 系统时间  - 64

    u32             records_rd;                                 // 读标记 - 71
    u32             records_wr;                                 // 写标记 - 75

    u16             empty_;
    u8              band_no;                                    // 当前使用的频段编码

//    u8              band_rssi[3];                               // 支持的三种频段对应的信号值
//    u8              have_connected;                             // 当前频段成功连接过

}SysParams;

// 运行参数，仅存在于内存中
typedef struct _RunParams{
    u8          net_state;                                      // 网络状态:0-搜网中;1-通讯中;2-待机中
    u8          rssi;                                           // 信号强度
    u8          battery_level;                                  // 电池电量，固定为0
    u8          re_power_on_flag;                               // 重新上电标志
    s16         temp;                                           // 温度
    s16         humi;                                           // 湿度
    u16         st;                                             // 状态ST
    u8          rtc_updata;

    u8          backlight_v;                                    //亮度等级
}RunParams;

extern SysParams g_sys_params;
extern RunParams g_run_params;

// 全局的频段表
extern const char *g_band_list[BAND_CNT];

//void set_valve_fault_code(u8 state);
u8    factory_check(void);

void sysparams_update_records_write_counter(u32 records_wr);

/*! \brief
*       更新系统参数
* \param addr[IN]           - 接收到的数据
* \param dat[IN]            - 数据内容
* \param len[IN]            - 数据长度
*/
void sysparams_write(u8 addr, u8 *dat, u16 len);

/*! \brief
*       解包nbiot指令下发协议处理
* \param command[IN]        - 接收到的数据
* \param len[OUT]           - 数据长度

*/
void sysparams_read(u8 addr, u8 *dat, u16 len);

#endif