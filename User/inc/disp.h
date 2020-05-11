#ifndef _DISP_H
#define _DISP_H

#include "bsp.h"

typedef enum _DispSwitch{
    DISPLAY_OFF,
    DISPLAY_ON
}DispSwitch;

// ��������ΪCOM1�У������������е�˳��ÿ�μ�1
typedef enum _DigitIndex{
    DIGIT1 = 0,  
    DIGIT2,    
    DIGIT3,
    DIGIT4,
    DIGIT5,
    DIGIT6,
    DIGIT7
}DigitIndex;

typedef enum _LcdSeg{
    SEG_1A = 0,	    /*!< \com1 brief */
    SEG_S9,		    /*!< \com1 brief �¶�С����. �� ��*/
    SEG_2A,		    /*!< \com1 brief */
    SEG_NULL,		/*!< \com1 brief */
    SEG_3A,			/*!< \com1 brief */
    SEG_NULL_,		/*!< \com1 brief */
    SEG_4A,			/*!< \com1 brief */
    SEG_S13,		/*!< \com1 brief ʪ��RH �� %*/
    SEG_5A,			/*!< \com1 brief */
    SEG_S10,		/*!< \com1 brief �ָ���*/
    SEG_6A,			/*!< \com1 brief */
    SEG_S12,		/*!< \com1 brief ˮ��*/
    SEG_7A,			/*!< \com1 brief */
    SEG_S11,		/*!< \com1 brief ���Ǿ���*/
    SEG_S8,			/*!< \com1 brief �洢��־*/
    SEG_S1,			/*!< \com1 brief �ź���*/


    SEG_1B = 28,	/*!< \com2 brief */
    SEG_1F,		    /*!< \com2 brief */
    SEG_2B,			/*!< \com2 brief */
    SEG_2F,			/*!< \com2 brief */
    SEG_3B,			/*!< \com2 brief */
    SEG_3F,			/*!< \com2 brief */
    SEG_4B,			/*!< \com2 brief */
    SEG_4F,			/*!< \com2 brief */
    SEG_5B,			/*!< \com2 brief */
    SEG_5F,			/*!< \com2 brief */
    SEG_6B,			/*!< \com2 brief */
    SEG_6F,			/*!< \com2 brief */
    SEG_7B,			/*!< \com2 brief */
    SEG_7F,			/*!< \com2 brief */
    SEG_S7,			/*!< \com2 brief �¶ȼƱ�־*/
    SEG_S2,			/*!< \com2 brief �źŵ�һ��*/

    SEG_1C = 56,	        /*!< \com2 brief */
    SEG_1G,		    /*!< \com3 brief */
    SEG_2C,			/*!< \com3 brief */
    SEG_2G,			/*!< \com3 brief */
    SEG_3C,			/*!< \com3 brief */
    SEG_3G,			/*!< \com3 brief */
    SEG_4C,			/*!< \com3 brief */
    SEG_4G,			/*!< \com3 brief */
    SEG_5C,			/*!< \com3 brief */
    SEG_5G,			/*!< \com3 brief */
    SEG_6C,			/*!< \com3 brief */
    SEG_6G,			/*!< \com3 brief */
    SEG_7C,			/*!< \com3 brief */
    SEG_7G,			/*!< \com3 brief */
    SEG_S6,			/*!< \com3 brief �źŵ����*/
    SEG_S3,			/*!< \com3 brief �źŵڶ���*/

    SEG_1D = 84,	        /*!< \com4 brief 10F*/
    SEG_1E,		        /*!< \com4 brief */
    SEG_2D,			/*!< \com4 brief */
    SEG_2E,			/*!< \com4 brief */
    SEG_3D,			/*!< \com4 brief */
    SEG_3E,			/*!< \com4 brief */
    SEG_4D,			/*!< \com4 brief */
    SEG_4E,			/*!< \com4 brief */
    SEG_5D,			/*!< \com4 brief */
    SEG_5E,			/*!< \com4 brief */
    SEG_6D,			/*!< \com4 brief */
    SEG_6E,			/*!< \com4 brief */
    SEG_7D,			/*!< \com4 brief */
    SEG_7E,			/*!< \com4 brief */
    SEG_S5,			/*!< \com4 brief �źŵ��ĸ�*/
    SEG_S4			/*!< \com4 brief �źŵ�����*/
}LcdSeg;

/*! \brief Һ����ʼ��*/
void disp_init(void);

void disp_verify(void);

/*! \brief Һ���ر� */
void disp_close(void);

/*! \brief ˢ����ʾҺ�� */
void disp_update(void);

void disp_zero(void);

/*! \brief Һ������ */
void disp_debug(void);

/*! \brief Һ����ʾȫ������ */
void disp_all(void);

/*! \brief ����Һ�� */
void disp_clr(void);

/*! \brief ��ʾ�ָ���*/
void disp_line(u8 disp_sts);

/*! \brief
*      ��ʾ�ź�״̬
* \param disp_sts[IN]      - �Ƿ���ʾ 0-�رգ�1-��ʾ
* \param ant_lv[IN]           - �ź��� 0~5
*/
void disp_ant(u8 disp_sts, u8 ant_lv);
void disp_ant_basestation(u8 disp_sts);
/*! \brief
*      �¶ȼƱ�־
* \param disp_sts[IN]                 - �Ƿ���ʾ�ַ� 0-�ر�(DISPLAY_OFF)��1-��ʾ(DISPLAY_ON)
*/
void disp_thermometer(u8 disp_sts);

/*! \brief
*      ˮ�α�־
* \param disp_sts[IN]                 - �Ƿ���ʾ�ַ� 0-�ر�(DISPLAY_OFF)��1-��ʾ(DISPLAY_ON)
*/
void disp_water_drop(u8 disp_sts);

/*! \brief
*      �洢��־
* \param disp_sts[IN]                 - �Ƿ���ʾ�ַ� 0-�ر�(DISPLAY_OFF)��1-��ʾ(DISPLAY_ON)
*/
void disp_disk(u8 disp_sts);

/*! \brief
*      ���Ǿ����־
* \param disp_sts[IN]                 - �Ƿ���ʾ�ַ� 0-�ر�(DISPLAY_OFF)��1-��ʾ(DISPLAY_ON)
*/
void disp_warning(u8 disp_sts);

/*! \brief
*       display a digit
* \param pos[IN]    - segment digit display index
*       DIGIT1      - the left digist position
*       DIGIT2      - the second digit position
*       DIGIT3      - the third digit position
*       DIGIT4      - the fourth digit position
*       DIGIT5      - the fifth digit position
* \param val[IN]    - display value index:0~9,A~F,S,T,-,P,r,' '
* \param sts[IN]    - display state
*       DISPLAY_ON  - display on
*       DISPLAY_OFF - display off
*/
void disp_digit(u8 pos, u8 val, u8 sts);

/*! \brief
*       display a digit
* \param pos[IN]    - segment char display index
*       DIGIT1      - the left digist position
*       DIGIT2      - the second digit position
*       DIGIT3      - the third digit position
*       DIGIT4      - the fourth digit position
*       DIGIT5      - the fifth digit position
* \param ch[IN]     - display character
*/
void disp_char(u8 start, u8 ch);

/*! \brief
*       display a number
* \param start[IN]      - start position:0~12
* \param end[IN]        - end position:0~12
* \param val[IN]        - value
*/
void disp_number(u8 start, u8 end, u32 val);

/*! \brief
*       display a string
* \param start[IN]      - start position:0~4
* \param end[IN]        - end position:0~4
* \param str[IN]        - value
*/
void disp_str(u8 start, u8 end, const char *str);

/*! \brief
*      ��ʾ�¶�ֵ
* \param temp[IN]         - �¶�ֵ
* \param unit_F_C[IN]     - �¶ȵĵ�λ��ʾ (TEMP_F)Ϊ�����϶ȣ�TEMP_C��Ϊ���϶�
* \note
*
*/
void disp_temp(s16 temp);

/*! \brief
*      ��ʾʪ��ֵ
* \param temp[IN]     - ʪ��ֵ
* \note
*
*/
void disp_humi(u16 humi);

void disp_close_temp_humi(void);

/*! \brief
*      ��ʾ��ص���
* \param value[IN]         - ����ֵ
*/
void disp_battery_level(u16 value);

#endif