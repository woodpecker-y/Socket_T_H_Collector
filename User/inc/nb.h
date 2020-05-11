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

/*! \brief NBģ�鷵��ֵ */
typedef enum _NBRet{
    E_NB_OK = 0,                      /*!< \brief �ɹ� */
    E_NB_PROCESSING,           /*!< \brief ������ */

    E_NB_ERR_SOFTOFF,                 /*!< \brief �ػ�ָ������� */
    E_NB_ERR_IPCONFIG,                /*!< \brief ����IP�������� */
    E_NB_ERR_CONFIG_PDP,              /*!< \brief ����PDP����ʧ��(APN) */
    E_NB_ERR_SOFTON,                  /*!< \brief ����ָ������� */
    E_NB_ERR_AUTOCONNECT,             /*!< \brief �Զ�������������ָ������� */
    E_NB_ERR_SETBAND,                 /*!< \brief ����Ƶ��ָ������� */
    E_NB_ERR_REQUESTIMEI,             /*!< \brief ��ѯIMEI��ָ������� */
    E_NB_ERR_REQUESTICCID,            /*!< \brief ��ѯ��ƬICCID��ָ������� */
    E_NB_ERR_REQUESTIMSI,             /*!< \brief ��ѯIMSI��ָ������� */

    E_NB_ERR_CGATT,                   /*!< \brief ע�����緢������ */
    E_NB_ERR_CGATT_QUE,               /*!< \brief ��ѯע�����緢������ */
    E_NB_ERR_MLWEVTIND,               /*!< \brief ע��NBIoT������ƽ̨���� */
    E_NB_ERR_CEREG,                   /*!< \brief ʹ��EPS����ע�ᷢ������ */
    E_NB_ERR_CEREG_QUE,               /*!< \brief ��ѯʹ��EPS����ע�ᷢ������ */
    E_NB_ERR_NNMI,                    /*!< \brief ��������֪ͨ�������� */

    E_NB_ERR_SIGNAL,                  /*!< \brief ����ź�ʱΪ99���� */
    E_NB_ERR_NUESTATS,                /*!< \brief ����ź�״̬���� */
    E_NB_ERR_RECVACK,                   /*!< \brief ����ACKָ������� */
    E_NB_ERR_TIME,                    /*!< \brief ���ʱ�����*/

    E_NB_ERR_NO_DATA,                 /*!< \brief �������� */
    E_NB_ERR_RECVING,                 /*!< \brief ������ */
    E_NB_ERR_MODULE_RESPONSE_ERROR    /*!< \brief ģ�鷴������ */
}NBRet;

//����CMD
typedef enum _ATCommand{
    E_AT_NRB=0,
    //ready
    E_ATE0,                     /*!< \brief �رջ��� */
    E_AT_CMEE,                  /*!< \brief ����������ʾ */
    E_AT_CFUN_OFF,              /*!< \brief �ػ� */
    E_AT_NCDP,                  /*!< \brief ����IP�˿� */
    E_AT_CGDCONT,               /*!< \brief ����APN */
    E_AT_CFUN,                  /*!< \brief ���ػ� */
    E_AT_NCONFIG,               /*!< \brief �Զ������������� */
    E_AT_NBAND,                 /*!< \brief ����Ƶ�� */
    E_AT_CELLRESELECTION,       /*!< \brief ����С����ѡ */
    E_AT_NCSEARFCN,             /*!< \brief �������Ƶ�� */
    E_AT_NPSMR,                 /*!< \brief ����PSMģʽ״̬���� */
    E_AT_CGSN,                  /*!< \brief ��ѯIMEI�� */
    E_AT_NCCID,                 /*!< \brief ��ѯ��ƬICCID�� */
    E_AT_CIMI,                  /*!< \brief ��ѯ��ƬIMSI�� */
    E_AT_FIRMWARE,              /*!< \brief ��ѯNBģ��Ĺ̼��汾�� */
    //����
    E_AT_CGATT,                 /*!< \brief �������� */
    E_AT_CGATT_QUE,             /*!< \brief ��ѯ�Ƿ񼤻����� */
    E_AT_MLWSREGIND,            /*!< \brief ����ע������� */
    E_AT_CEREG,                 /*!< \brief ʹ��EPS����ע�� */
    E_AT_CEREG_QUE,             /*!< \brief ��ѯʹ��EPS����ע�� */
    E_AT_NNMI,                  /*!< \brief ��������֪ͨ */
    E_AT_NMGR,                  /*!< \brief ��ȡ�������������� */
    //��ѯ�źš�ʱ��
    E_AT_CSQ,
    E_AT_NUESTATS,
    E_AT_CCLK,
    //��������
    E_AT_SEND_ACK,
    E_AT_SEND_NO_ACK,
    E_AT_IPRECV               /*!< \brief �������� */


}ATCommand;

//״̬STS
typedef enum _Sts{
    E_SEND_CMD = 0,
    E_IS_OK,
    E_OTHER
}sts;

//״̬STS
typedef enum _data_mode{
    E_SEND_NORMAL = 0,
    E_SEND_RAI_NON    = 0x0001,//����ظ��ϴ��ɹ�ȷ��
    E_SEND_RAI_CON    = 0x0101//��Ҫ�ɹ�����յ�QLWULDATASTATUS:4��ʾ�ɹ�����
}data_mode;

// ģ��Ψһʶ����
#define LENGTH_IMEI     15
// USIM��Ψһʶ����
#define LENGTH_ICCID    20
// ��վID
#define LENGTH_CELL_ID  8

typedef enum _NetworkOperator{
    E_NETWORK_OPERATOR_CT = 0,  // ����
    E_NETWORK_OPERATOR_CU       // ��ͨ
}NetworkOperator;

// ����Ƶ��BAND���ȼ���
#define BAND_LIST_CT    "5,8,3"
// ��ͨƵ��BAND���ȼ���
#define BAND_LIST_CU    "3,8,5"
//#define BAND_LIST_CU    "8"

//typedef struct _NBIoTModuleState{
//    u8     imei[LENGTH_IMEI+1];
//    u8     iccid[LENGTH_ICCID+1];
//    u8     rssi;//��ʵrssiΪ��ֵ����ָ��ָ��CSQ�������ֵ��Ϊ��ֵ
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
    u8     rssi;                //��ʵrssiΪ��ֵ����ָ��ָ��CSQ�������ֵ��Ϊ��ֵ
    u8     time[7];             // ϵͳʱ��
    u8     cmd_dat[48];
    u8     cmd_size;
//    u8*    recv_ack;
//    u16    len_ack;
    u16    max_repeat_send_timer;//�ظ����Ͷ�ʱ��
    u16    repeat_send_timer;
//    NBIoTModuleState state;
}NBHandler;    //static NBHandler sg_nb_handler;

/*! \brief
*       ����ģ������
* \param handler[IN]        - NB config handler
*/
void BSP_NB_LoadConfig(NBConfig *handler);

/*! \brief
*       ��ģ���ϵ�
*/
void BSP_NB_PowerOn(void);

/*! \brief
*       ��ģ��ϵ�
*/
void BSP_NB_PowerOff(void);

/*! \brief
*       ģ�����ó�ʼ��
* \param baudrate[IN]       - ģ�鴮�ڲ�����
* \param wordlength[IN]     - ģ�鴮������λ
* \param stopbits[IN]       - ģ�鴮��ֹͣλ
* \param parity[IN]         - ģ�鴮��У��λ
*
* \return
*       E_NB_OK     - �ɹ�
*       Others          - ʧ��
*/
u8 BSP_NB_Init(u32 baudrate, u8 wordlength, u8 stopbits, u8 parity);

/*! \brief
*       �ͷ�ģ��,��Ƭ���رմ���
*/
void BSP_NB_Close(void);

/*! \brief
*       ���������ڿ���ʱ�����ʱ����
*/
u8 NB_SoftReboot(void);

/*! \brief
*       ������֧���Զ��ط�������趨�ط���ʱ���ڣ�����ִ��ʧ�ܺ�������ط�����ҪӦ�ò����
*/
void BSP_NB_SetRepeatSendCounter(u16 counter);

/*! \brief
* ���NBģ���Ƿ���׼��������,���Ҷ����豸����
*
* \param first_connect[IN]         - ��Ӫ������
* \param type[IN]         - ��Ӫ������
* \param apn[IN]          - APN
* \param ip[IN]           - IP
* \param ip_port[IN]      - IP�˿�
* \param imei[OUT]        - ģ��IMEI��
* \param card_id[OUT]     - SIM����ICCID
* \return
*       E_NB_OK               - �ɹ�
*       E_NB_PROCESSING       - ������
*       E_NB_ERR_NO_SIM       - ��SIM��
*       E_NB_ERR_CONFIG_PDP   - ����PDP����ʧ��
*       E_NB_ERR_ACTIVE_PDP   - ����PDP����ʧ��
*       E_NB_ERR_ATTACHED_NETWORK - ��������ʧ��
*/
//u8 BSP_NB_CheckIsReady(char* apn, char* ip, u16* ip_port, char* imei, char* iccid, char* firmware)
u8 BSP_NB_CheckIsReady(u8 first_connect, const char* band, char* apn, char* ip, u16* ip_port, char* imei);

/*! \brief
*       ���ģ��ķ����Ƿ�ΪOK
* \param msg[IN]                - ��ģ����յ�����
*
* \return
*       E_NB_OK             - �ɹ�
*       E_NB_ERR_OTHERS       - ʧ��
*/
static u8 NB_CheckRespIsRecvComplete(char *msg);

/*! \brief
* ����
* \param rssi[OUT]           - �ź�ֵ
* \return
*       E_NB_OK               - �ɹ�
*       E_NB_PROCESSING       - ������
*       E_NB_ERR_NO_SIM       - ��SIM��
*       E_NB_ERR_CONFIG_PDP   - ����PDP����ʧ��
*       E_NB_ERR_ACTIVE_PDP   - ����PDP����ʧ��
*       E_NB_ERR_ATTACHED_NETWORK - ��������ʧ��
*/
u8 BSP_NB_CheckNet(u8 *rssi);

/*! \brief
*   ������������
* \return
*       E_NB_OK               - �ɹ�
*       E_NB_PROCESSING       - ������
*       E_NB_ERR_NO_SIM       - ��SIM��
*       E_NB_ERR_CONFIG_PDP   - ����PDP����ʧ��
*       E_NB_ERR_ACTIVE_PDP   - ����PDP����ʧ��
*       E_NB_ERR_ATTACHED_NETWORK - ��������ʧ��
*/
u8 BSP_NB_ReadData(u8 *cmd, u16* cmd_len);

///*! \brief
//*       ��ȡ�ź�ֵ
//* \param rssi[OUT]           - �ź�ֵ
//* \param signalPower[OUT]           -
//* \param TxPower[OUT]           - �ź�ֵ
//* \param CellID[OUT]           - �ź�ֵ
//* \param ECL[OUT]           - �ź�ֵ
//* \param SNR[OUT]           - �ź�ֵ
//* \param PCI[OUT]           - �ź�ֵ
//* \param RSRQ[OUT]           - �ź�ֵ
//* \return
//*       E_NB_OK           - �ɹ�
//*       E_NB_PROCESSING   - ������
//*       E_NB_ERR_OTHERS   - ����
//*/
//u8 BSP_NB_GetRSSI(u8 *rssi);

/*! \brief
*       ��ȡ�ź�ֵ
* \param rssi[OUT]           - �ź�ֵ
* \param time[OUT]           - ϵͳʱ��
* \return
*       E_NB_OK           - �ɹ�
*       E_NB_PROCESSING   - ������
*       E_NB_ERR_OTHERS   - ����
*/
u8 BSP_NB_GetRSSIAndTime(u8 *rssi, u8* time);

/*! \brief
*       �������ݵ�������<ÿ�η������Ϊ100���ֽ�>
* \param dat[IN]           - ����
* \param len[IN]           - ���ݳ���
* \param mode[IN]          - ���ݷ���ģʽ
* \return
*       E_NB_OK           - �ɹ�
*       E_NB_PROCESSING   - ������
*       E_NB_ERR_OTHERS   - ����
*/
u8 BSP_NB_IPSendData(u8* dat, u16 len, u16 mode);

/*! \brief
*       ���մӷ������·�������
* \param msg[OUT]           - ��Ϣָ��
* \param size[OUT]          - ���ݳ���
*
* \return
*       E_NB_OK           - �ɹ�
*       E_NB_ERR_NO_DATA  - ��������
*       E_NB_ERR_OTHERS   - ����
* \notes
*       msgΪ����ָ�룬����ҪΪ�������ڴ滺�壬��ֱ��ָ�򴮿ڽ��ջ��������ڵ���NB_recv_init()��NB_send_data()����֮ǰ��һֱ���á�
*/
u8 BSP_NB_IPRecvData(u8* msg, u16 *size);

/*! \brief
*       NBģ�����ݽ��ճ�ʼ��
*/
void BSP_NB_RecvInit(void);

u8 NB_SendData(const u8 *dat, u16 len);

/*! \brief
*       ��ȡPSMģʽ״̬
*
* \return
*       E_NB_OK           - �ɹ�
*       E_NB_ERR_RECVING  - ��������
*/
u8 BSP_NB_GetPSMState(void);


#endif

#endif
