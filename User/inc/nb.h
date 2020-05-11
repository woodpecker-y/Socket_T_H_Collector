#ifndef _NB_H
#define _NB_H

#ifdef HAVE_NB

#include "bsp.h"

/*! \brief NB Configure Definition */
typedef struct _NBConfig{
    COM          com;            /*!< \brief NB communication com */
    PortTypeDef  reset;          /*!< \brief reset pin */
    PortTypeDef  pwr;            /*!< \brief power pin */
    u8           pwr_le;         /*!< \brief power level effective */
}NBConfig;

/*! \brief NB模块返回值 */
typedef enum _NBRet{
    E_NB_OK = 0,                      /*!< \brief 成功 */
    E_NB_PROCESSING,           /*!< \brief 处理中 */

    E_NB_ERR_SOFTOFF,                 /*!< \brief 关机指令发生错误 */
    E_NB_ERR_IPCONFIG,                /*!< \brief 配置IP发生错误 */
    E_NB_ERR_CONFIG_PDP,              /*!< \brief 配置PDP参数失败(APN) */
    E_NB_ERR_SOFTON,                  /*!< \brief 开机指令发生错误 */
    E_NB_ERR_AUTOCONNECT,             /*!< \brief 自动附着网络连接指令发生错误 */
    E_NB_ERR_SETBAND,                 /*!< \brief 设置频段指令发生错误 */
    E_NB_ERR_REQUESTIMEI,             /*!< \brief 查询IMEI号指令发生错误 */
    E_NB_ERR_REQUESTICCID,            /*!< \brief 查询卡片ICCID号指令发生错误 */
    E_NB_ERR_REQUESTIMSI,             /*!< \brief 查询IMSI号指令发生错误 */

    E_NB_ERR_CGATT,                   /*!< \brief 注册网络发生错误 */
    E_NB_ERR_CGATT_QUE,               /*!< \brief 查询注册网络发生错误 */
    E_NB_ERR_MLWEVTIND,               /*!< \brief 注册NBIoT核心网平台出错 */
    E_NB_ERR_CEREG,                   /*!< \brief 使能EPS网络注册发生错误 */
    E_NB_ERR_CEREG_QUE,               /*!< \brief 查询使能EPS网络注册发生错误 */
    E_NB_ERR_NNMI,                    /*!< \brief 开启数据通知发生错误 */

    E_NB_ERR_SIGNAL,                  /*!< \brief 检测信号时为99错误 */
    E_NB_ERR_NUESTATS,                /*!< \brief 检测信号状态错误 */
    E_NB_ERR_RECVACK,                   /*!< \brief 接收ACK指令发生错误 */
    E_NB_ERR_TIME,                    /*!< \brief 检测时间错误*/

    E_NB_ERR_NO_DATA,                 /*!< \brief 暂无数据 */
    E_NB_ERR_RECVING,                 /*!< \brief 接收中 */
    E_NB_ERR_MODULE_RESPONSE_ERROR    /*!< \brief 模组反馈错误 */
}NBRet;

//命令CMD
typedef enum _ATCommand{
    E_AT_NRB=0,
    //ready
    E_ATE0,                     /*!< \brief 关闭回显 */
    E_AT_CMEE,                  /*!< \brief 开启错误提示 */
    E_AT_CFUN_OFF,              /*!< \brief 关机 */
    E_AT_NCDP,                  /*!< \brief 配置IP端口 */
    E_AT_CGDCONT,               /*!< \brief 配置APN */
    E_AT_CFUN,                  /*!< \brief 开关机 */
    E_AT_NCONFIG,               /*!< \brief 自动附着网络连接 */
    E_AT_NBAND,                 /*!< \brief 设置频段 */
    E_AT_CELLRESELECTION,       /*!< \brief 设置小区重选 */
    E_AT_NCSEARFCN,             /*!< \brief 清除先验频点 */
    E_AT_NPSMR,                 /*!< \brief 开启PSM模式状态报告 */
    E_AT_CGSN,                  /*!< \brief 查询IMEI号 */
    E_AT_NCCID,                 /*!< \brief 查询卡片ICCID号 */
    E_AT_CIMI,                  /*!< \brief 查询卡片IMSI号 */
    E_AT_FIRMWARE,              /*!< \brief 查询NB模块的固件版本号 */
    //入网
    E_AT_CGATT,                 /*!< \brief 激活网络 */
    E_AT_CGATT_QUE,             /*!< \brief 查询是否激活网络 */
    E_AT_MLWSREGIND,            /*!< \brief 主动注册核心网 */
    E_AT_CEREG,                 /*!< \brief 使能EPS网络注册 */
    E_AT_CEREG_QUE,             /*!< \brief 查询使能EPS网络注册 */
    E_AT_NNMI,                  /*!< \brief 开启数据通知 */
    E_AT_NMGR,                  /*!< \brief 读取核心网命令数据 */
    //查询信号、时间
    E_AT_CSQ,
    E_AT_NUESTATS,
    E_AT_CCLK,
    //发送数据
    E_AT_SEND_ACK,
    E_AT_SEND_NO_ACK,
    E_AT_IPRECV               /*!< \brief 接收数据 */


}ATCommand;

//状态STS
typedef enum _Sts{
    E_SEND_CMD = 0,
    E_IS_OK,
    E_OTHER
}sts;

//状态STS
typedef enum _data_mode{
    E_SEND_NORMAL = 0,
    E_SEND_RAI_NON    = 0x0001,//无需回复上传成功确认
    E_SEND_RAI_CON    = 0x0101//需要成功后会收到QLWULDATASTATUS:4表示成功发送
}data_mode;

// 模组唯一识别码
#define LENGTH_IMEI     15
// USIM卡唯一识别码
#define LENGTH_ICCID    20
// 基站ID
#define LENGTH_CELL_ID  8

typedef enum _NetworkOperator{
    E_NETWORK_OPERATOR_CT = 0,  // 电信
    E_NETWORK_OPERATOR_CU       // 联通
}NetworkOperator;

// 电信频率BAND优先级表
#define BAND_LIST_CT    "5,8,3"
// 联通频率BAND优先级表
#define BAND_LIST_CU    "3,8,5"
//#define BAND_LIST_CU    "8"

//typedef struct _NBIoTModuleState{
//    u8     imei[LENGTH_IMEI+1];
//    u8     iccid[LENGTH_ICCID+1];
//    u8     rssi;//其实rssi为负值这里指用指令CSQ查出来的值，为正值
//    s16    signal_power;
//    s16    total_power;
//    s16    tx_power;
//    u8     cell_id[LENGTH_CELL_ID+1];
//    u8     ecl;
//    s8     snr;
//    u8     pci;
//    s16    rsrq;
//    u8     firmware[8];
//}NBIoTModuleState;
//
/*! \brief */
typedef struct _NBHandler{
    u8     sts;
    u8     cfun_sts;
    u8     net_sts;
    u8     rssi_sts;
    u8     cmd;
    u8     imei[LENGTH_IMEI+1];
    u8     iccid[LENGTH_ICCID+1];
    u8     rssi;                //其实rssi为负值这里指用指令CSQ查出来的值，为正值
    u8     time[7];             // 系统时间
    u8     cmd_dat[48];
    u8     cmd_size;
//    u8*    recv_ack;
//    u16    len_ack;
    u16    max_repeat_send_timer;//重复发送定时器
    u16    repeat_send_timer;
//    NBIoTModuleState state;
}NBHandler;    //static NBHandler sg_nb_handler;

/*! \brief
*       加载模块配置
* \param handler[IN]        - NB config handler
*/
void BSP_NB_LoadConfig(NBConfig *handler);

/*! \brief
*       给模块上电
*/
void BSP_NB_PowerOn(void);

/*! \brief
*       给模块断电
*/
void BSP_NB_PowerOff(void);

/*! \brief
*       模块配置初始化
* \param baudrate[IN]       - 模块串口波特率
* \param wordlength[IN]     - 模块串口数据位
* \param stopbits[IN]       - 模块串口停止位
* \param parity[IN]         - 模块串口校验位
*
* \return
*       E_NB_OK     - 成功
*       Others          - 失败
*/
u8 BSP_NB_Init(u32 baudrate, u8 wordlength, u8 stopbits, u8 parity);

/*! \brief
*       释放模块,单片机关闭串口
*/
void BSP_NB_Close(void);

/*! \brief
*       软重启，在开机时或故障时重启
*/
u8 NB_SoftReboot(void);

/*! \brief
*       驱动层支持自动重发命令，可设定重发超时周期，命令执行失败后可启动重发，需要应用层配合
*/
void BSP_NB_SetRepeatSendCounter(u16 counter);

/*! \brief
* 检查NB模块是否已准备好联网,并且读出设备参数
*
* \param first_connect[IN]         - 运营商类型
* \param type[IN]         - 运营商类型
* \param apn[IN]          - APN
* \param ip[IN]           - IP
* \param ip_port[IN]      - IP端口
* \param imei[OUT]        - 模块IMEI号
* \param card_id[OUT]     - SIM卡的ICCID
* \return
*       E_NB_OK               - 成功
*       E_NB_PROCESSING       - 处理中
*       E_NB_ERR_NO_SIM       - 无SIM卡
*       E_NB_ERR_CONFIG_PDP   - 配置PDP参数失败
*       E_NB_ERR_ACTIVE_PDP   - 激活PDP参数失败
*       E_NB_ERR_ATTACHED_NETWORK - 附着网络失败
*/
//u8 BSP_NB_CheckIsReady(char* apn, char* ip, u16* ip_port, char* imei, char* iccid, char* firmware)
u8 BSP_NB_CheckIsReady(u8 first_connect, const char* band, char* apn, char* ip, u16* ip_port, char* imei);

/*! \brief
*       检查模块的反馈是否为OK
* \param msg[IN]                - 从模块接收的数据
*
* \return
*       E_NB_OK             - 成功
*       E_NB_ERR_OTHERS       - 失败
*/
static u8 NB_CheckRespIsRecvComplete(char *msg);

/*! \brief
* 入网
* \param rssi[OUT]           - 信号值
* \return
*       E_NB_OK               - 成功
*       E_NB_PROCESSING       - 处理中
*       E_NB_ERR_NO_SIM       - 无SIM卡
*       E_NB_ERR_CONFIG_PDP   - 配置PDP参数失败
*       E_NB_ERR_ACTIVE_PDP   - 激活PDP参数失败
*       E_NB_ERR_ATTACHED_NETWORK - 附着网络失败
*/
u8 BSP_NB_CheckNet(u8 *rssi);

/*! \brief
*   读服务器命令
* \return
*       E_NB_OK               - 成功
*       E_NB_PROCESSING       - 处理中
*       E_NB_ERR_NO_SIM       - 无SIM卡
*       E_NB_ERR_CONFIG_PDP   - 配置PDP参数失败
*       E_NB_ERR_ACTIVE_PDP   - 激活PDP参数失败
*       E_NB_ERR_ATTACHED_NETWORK - 附着网络失败
*/
u8 BSP_NB_ReadData(u8 *cmd, u16* cmd_len);

///*! \brief
//*       读取信号值
//* \param rssi[OUT]           - 信号值
//* \param signalPower[OUT]           -
//* \param TxPower[OUT]           - 信号值
//* \param CellID[OUT]           - 信号值
//* \param ECL[OUT]           - 信号值
//* \param SNR[OUT]           - 信号值
//* \param PCI[OUT]           - 信号值
//* \param RSRQ[OUT]           - 信号值
//* \return
//*       E_NB_OK           - 成功
//*       E_NB_PROCESSING   - 处理中
//*       E_NB_ERR_OTHERS   - 错误
//*/
//u8 BSP_NB_GetRSSI(u8 *rssi);

/*! \brief
*       读取信号值
* \param rssi[OUT]           - 信号值
* \param time[OUT]           - 系统时间
* \return
*       E_NB_OK           - 成功
*       E_NB_PROCESSING   - 处理中
*       E_NB_ERR_OTHERS   - 错误
*/
u8 BSP_NB_GetRSSIAndTime(u8 *rssi, u8* time);

/*! \brief
*       发送数据到服务器<每次发送最多为100个字节>
* \param dat[IN]           - 数据
* \param len[IN]           - 数据长度
* \param mode[IN]          - 数据发送模式
* \return
*       E_NB_OK           - 成功
*       E_NB_PROCESSING   - 处理中
*       E_NB_ERR_OTHERS   - 错误
*/
u8 BSP_NB_IPSendData(u8* dat, u16 len, u16 mode);

/*! \brief
*       接收从服务器下发的数据
* \param msg[OUT]           - 消息指针
* \param size[OUT]          - 数据长度
*
* \return
*       E_NB_OK           - 成功
*       E_NB_ERR_NO_DATA  - 暂无数据
*       E_NB_ERR_OTHERS   - 错误
* \notes
*       msg为二级指针，不需要为它分配内存缓冲，它直接指向串口接收缓冲区，在调用NB_recv_init()或NB_send_data()函数之前，一直可用。
*/
u8 BSP_NB_IPRecvData(u8* msg, u16 *size);

/*! \brief
*       NB模块数据接收初始化
*/
void BSP_NB_RecvInit(void);

u8 NB_SendData(const u8 *dat, u16 len);

/*! \brief
*       获取PSM模式状态
*
* \return
*       E_NB_OK           - 成功
*       E_NB_ERR_RECVING  - 暂无数据
*/
u8 BSP_NB_GetPSMState(void);


#endif

#endif
