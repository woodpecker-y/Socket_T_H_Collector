#ifndef _SYS_PARAMS_H
#define _SYS_PARAMS_H

#include "adf.h"
#include "board.h"

#define TEMPERATURE_COMPENSATION       0  //�¶Ȳ�������    0�ر�   1��

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
    E_ADDR_OFFSET_FACTORY = 0,          //������ʶ
    E_ADDR_OFFSET_DEV_TYPE = 1,         //�豸����
    E_ADDR_OFFSET_DEV_ADDR = 2,         //�豸ID_IMEI-16
    E_ADDR_OFFSET_IP = 18,              //IP-16
    E_ADDR_OFFSET_PORT = 34,            //port-2
    E_ADDR_OFFSET_APN = 36,             //APN-20
    E_ADDR_OFFSET_FIX_TEMP = 56,
    E_ADDR_OFFSET_FIX_HUMI = 57,
    //58    59
    E_ADDR_OFFSET_UPLOAD_CYCLE = 60,    //�ϴ�����
    E_ADDR_OFFSET_SAMPLE_CYCLE  = 62,   //�ɼ�����
    E_ADDR_OFFSET_TIME = 64,            //ʱ��
    E_ADDR_OFFSET_RECORDS_READ = 71,    //����־
    E_ADDR_OFFSET_RECORDS_WRITE = 75,   //д��־
//    E_ADDR_OFFSET_LIGHT1_STATE = 79,    //
//    E_ADDR_OFFSET_LIGHT2_STATE = 80,    //
    E_ADDR_OFFSET_BAND_NO = 81,         //BAND
//    E_ADDR_OFFSET_BAND_RSSI = 82,
//    E_ADDR_OFFSET_HAVE_CONNECTED = 82
}AddrOffset;

// ϵͳ��������Ҫ�洢��EEPROM��,ϵͳ���������������85Bytes���ڣ�����Ḳ����ʷ��¼��
typedef struct _SysParams{
    u8              factory_flag;			                    // ������ʶ - 0
    u8              dev_type;                                   // �豸���ʹ��� - 1 : 'T'- ���Ű�������;   'U'- ��ͨ�����
    u8              addr[LENGTH_ADDR];                          // �豸��ַ - 2

    char            ip[16];                                     // IP��ַ -18
    u16             port;                                       // �˿ں� - 34
    char            apn[20];                                    // APN  - 36
     s8             fix_temp;                                   //�¶Ȳ���ֵ
     s8             fix_humi;                                   //ʪ�Ȳ���ֵ

    u16             _num;
    u16             upload_cycle;                               // �ϱ�����  - 60  ��λΪ����
    u16             sample_cycle;                               // �ɼ�����  - 62
    u8              t[7];                                       // ϵͳʱ��  - 64

    u32             records_rd;                                 // ����� - 71
    u32             records_wr;                                 // д��� - 75

    u16             empty_;
    u8              band_no;                                    // ��ǰʹ�õ�Ƶ�α���

//    u8              band_rssi[3];                               // ֧�ֵ�����Ƶ�ζ�Ӧ���ź�ֵ
//    u8              have_connected;                             // ��ǰƵ�γɹ����ӹ�

}SysParams;

// ���в��������������ڴ���
typedef struct _RunParams{
    u8          net_state;                                      // ����״̬:0-������;1-ͨѶ��;2-������
    u8          rssi;                                           // �ź�ǿ��
    u8          battery_level;                                  // ��ص������̶�Ϊ0
    u8          re_power_on_flag;                               // �����ϵ��־
    s16         temp;                                           // �¶�
    s16         humi;                                           // ʪ��
    u16         st;                                             // ״̬ST
    u8          rtc_updata;

    u8          backlight_v;                                    //���ȵȼ�
}RunParams;

extern SysParams g_sys_params;
extern RunParams g_run_params;

// ȫ�ֵ�Ƶ�α�
extern const char *g_band_list[BAND_CNT];

//void set_valve_fault_code(u8 state);
u8    factory_check(void);

void sysparams_update_records_write_counter(u32 records_wr);

/*! \brief
*       ����ϵͳ����
* \param addr[IN]           - ���յ�������
* \param dat[IN]            - ��������
* \param len[IN]            - ���ݳ���
*/
void sysparams_write(u8 addr, u8 *dat, u16 len);

/*! \brief
*       ���nbiotָ���·�Э�鴦��
* \param command[IN]        - ���յ�������
* \param len[OUT]           - ���ݳ���

*/
void sysparams_read(u8 addr, u8 *dat, u16 len);

#endif