#include "lcd.h"
#include "disp.h"
#include "dbg.h"
#include "utili.h"
#include <string.h>
#include <stdio.h>

static unsigned char disp_buf[14] = {0};

void disp_hex_dump(u8 *dat, u16 len)
{
    u8 i = 0;
    
    for (i=0; i<len; ++i)
    {
        printf("%02X ", dat[i]);
    }
    
    printf("\r\n");
}

void disp_init(void)
{
    lcd_init();
    memset(disp_buf, 0x00, sizeof(disp_buf));
    lcd_update_screen(disp_buf, sizeof(disp_buf));
}

void disp_verify(void)
{
    int i = 0;
    
    for(i=0; i<102; i++)
    {
        if((i<17 && i>=0) || (i<45 && i>=28) || (i<73 && i>=56) || (i<101 && i>=84))
        {
            lcd_write_seg(disp_buf, i, 1);
            delay_ms(100);
            disp_update();            //brief ������ʾҺ��
        }
    }
    return;
}

void disp_close(void)
{
    lcd_close();
}

void disp_update(void)
{
    lcd_update_screen(disp_buf, sizeof(disp_buf));
}

void disp_debug(void)
{
    //MYLOG_DEBUG_HEXDUMP("Disp:", disp_buf, sizeof(disp_buf));
}

void disp_all(void)
{
    memset(disp_buf, 0xFF, sizeof(disp_buf));
    //memset(disp_buf+7, 0x80, 1);
    lcd_update_screen(disp_buf, sizeof(disp_buf));
}

void disp_clr(void)
{
    memset(disp_buf, 0x00, sizeof(disp_buf));
    lcd_update_screen(disp_buf, sizeof(disp_buf));
}

/*! \brief
*      ��ʾ�ָ���
* \param disp_sts[IN]      - �Ƿ���ʾ 0-�ر�(DISPLAY_OFF)��1-��ʾ(DISPLAY_ON)
*/
void disp_line(u8 disp_sts)
{
    assert_param(disp_sts==DISPLAY_ON || disp_sts==DISPLAY_OFF);
    
    lcd_write_seg(disp_buf, SEG_S10, DISPLAY_OFF);
    if (disp_sts == DISPLAY_ON)
    {
        lcd_write_seg(disp_buf, SEG_S10, DISPLAY_ON);
    }
    
    //lcd_update_screen(disp_buf, sizeof(disp_buf));
}

/*! \brief
*      ��ʾ�źŻ�վ��־
* \param disp_sts[IN]      - �Ƿ���ʾ 0-�ر�(DISPLAY_OFF)��1-��ʾ(DISPLAY_ON)
* \param ant_lv  [IN]        - �ź��� 0~5
*/
void disp_ant_basestation(u8 disp_sts)
{
    assert_param(disp_sts==DISPLAY_ON || disp_sts==DISPLAY_OFF);
    //assert_param(ant_lv == 0 || ant_lv == 1 || ant_lv == 2 || ant_lv == 3 || ant_lv == 4 || ant_lv == 5);

    lcd_write_seg(disp_buf, SEG_S1, DISPLAY_OFF); //�ź���
    lcd_write_seg(disp_buf, SEG_S2, DISPLAY_OFF);//�ź�1��
    lcd_write_seg(disp_buf, SEG_S3, DISPLAY_OFF);//�ź�2��
    lcd_write_seg(disp_buf, SEG_S4, DISPLAY_OFF);//�ź�3��
    lcd_write_seg(disp_buf, SEG_S5, DISPLAY_OFF);//�ź�4��
    lcd_write_seg(disp_buf, SEG_S6, DISPLAY_OFF);//�ź�5��

    // ��ʾ�ź�
    lcd_write_seg(disp_buf, SEG_S1, disp_sts);  //�ź���
    
    //lcd_update_screen(disp_buf, sizeof(disp_buf));
}

/*! \brief
*      ��ʾ�ź�״̬
* \param disp_sts[IN]      - �Ƿ���ʾ 0-�ر�(DISPLAY_OFF)��1-��ʾ(DISPLAY_ON)
* \param ant_lv  [IN]        - �ź��� 0~5
*/
void disp_ant(u8 disp_sts, u8 ant_lv)
{
    assert_param(disp_sts==DISPLAY_ON || disp_sts==DISPLAY_OFF);
    assert_param(ant_lv == 0 || ant_lv == 1 || ant_lv == 2 || ant_lv == 3 || ant_lv == 4 || ant_lv == 5);

    lcd_write_seg(disp_buf, SEG_S1, DISPLAY_OFF); //�ź���
    lcd_write_seg(disp_buf, SEG_S2, DISPLAY_OFF);//�ź�1��
    lcd_write_seg(disp_buf, SEG_S3, DISPLAY_OFF);//�ź�2��
    lcd_write_seg(disp_buf, SEG_S4, DISPLAY_OFF);//�ź�3��
    lcd_write_seg(disp_buf, SEG_S5, DISPLAY_OFF);//�ź�4��
    lcd_write_seg(disp_buf, SEG_S6, DISPLAY_OFF);//�ź�5��

    // ��ʾ�ź�
    if (disp_sts == DISPLAY_ON)
    {
        lcd_write_seg(disp_buf, SEG_S1, DISPLAY_ON); //�ź���
            
    	switch(ant_lv)
    	{
        case 5:
            lcd_write_seg(disp_buf, SEG_S6, DISPLAY_ON);//�ź�5��            
        case 4:
            lcd_write_seg(disp_buf, SEG_S5, DISPLAY_ON);//�ź�4��            
        case 3:
            lcd_write_seg(disp_buf, SEG_S4, DISPLAY_ON);//�ź�3��          
        case 2:
            lcd_write_seg(disp_buf, SEG_S3, DISPLAY_ON);//�ź�2��             
        case 1:
            lcd_write_seg(disp_buf, SEG_S2, DISPLAY_ON);//�ź�1��
        case 0:
            break;            
        default:
            break;
    	}
    }
    
    //lcd_update_screen(disp_buf, sizeof(disp_buf));
}

/*! \brief
*      �¶ȼƱ�־
* \param disp_sts[IN]                 - �Ƿ���ʾ�ַ� 0-�ر�(DISPLAY_OFF)��1-��ʾ(DISPLAY_ON)
*/
void disp_thermometer(u8 disp_sts)
{
    assert_param(disp_sts==DISPLAY_ON || disp_sts==DISPLAY_OFF);
        
    if(disp_sts == DISPLAY_ON)
    {
        lcd_write_seg(disp_buf, SEG_S7, DISPLAY_ON);
    }
    else
    {
    	lcd_write_seg(disp_buf, SEG_S7, DISPLAY_OFF);
    }
    
    //lcd_update_screen(disp_buf, sizeof(disp_buf));
}

/*! \brief
*      ˮ�α�־
* \param disp_sts[IN]                 - �Ƿ���ʾ�ַ� 0-�ر�(DISPLAY_OFF)��1-��ʾ(DISPLAY_ON)
*/
void disp_water_drop(u8 disp_sts)
{
    assert_param(disp_sts==DISPLAY_ON || disp_sts==DISPLAY_OFF);
    
    if(disp_sts == DISPLAY_ON)
    {
        lcd_write_seg(disp_buf, SEG_S12, DISPLAY_ON);
    }
    else
    {
    	lcd_write_seg(disp_buf, SEG_S12, DISPLAY_OFF);
    }
    
    //lcd_update_screen(disp_buf, sizeof(disp_buf));
}

/*! \brief
*      �洢��־
* \param disp_sts[IN]                 - �Ƿ���ʾ�ַ� 0-�ر�(DISPLAY_OFF)��1-��ʾ(DISPLAY_ON)
*/
void disp_disk(u8 disp_sts)
{
    assert_param(disp_sts==DISPLAY_ON || disp_sts==DISPLAY_OFF);
    
    if(disp_sts == DISPLAY_ON)
    {
        lcd_write_seg(disp_buf, SEG_S8, DISPLAY_ON);
    }
    else
    {
    	lcd_write_seg(disp_buf, SEG_S8, DISPLAY_OFF);
    }
    
    //lcd_update_screen(disp_buf, sizeof(disp_buf));
}

/*! \brief
*      ���Ǿ����־
* \param disp_sts[IN]                 - �Ƿ���ʾ�ַ� 0-�ر�(DISPLAY_OFF)��1-��ʾ(DISPLAY_ON)
*/
void disp_warning(u8 disp_sts)
{
    assert_param(disp_sts==DISPLAY_ON || disp_sts==DISPLAY_OFF);
    
    if(disp_sts == DISPLAY_ON)
    {
        lcd_write_seg(disp_buf, SEG_S11, DISPLAY_ON);
    }
    else
    {
    	lcd_write_seg(disp_buf, SEG_S11, DISPLAY_OFF);
    }
    
    //lcd_update_screen(disp_buf, sizeof(disp_buf));
}

void disp_zero(void)
{
    lcd_write_seg(disp_buf, SEG_3A, DISPLAY_ON);
    lcd_write_seg(disp_buf, SEG_3B, DISPLAY_ON);
    lcd_write_seg(disp_buf, SEG_3C, DISPLAY_ON);
    lcd_write_seg(disp_buf, SEG_3D, DISPLAY_ON);
    lcd_write_seg(disp_buf, SEG_3E, DISPLAY_ON);
    lcd_write_seg(disp_buf, SEG_3F, DISPLAY_ON);
    lcd_write_seg(disp_buf, SEG_3G, DISPLAY_ON);
    
#ifdef HAVE_TEST_DISP
    printf("0:\r\n");
    disp_hex_dump(disp_buf, sizeof(disp_buf));
#endif
    
    //lcd_update_screen(disp_buf, sizeof(disp_buf));
} 

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
* \note
*       1)stm8l152��ʾ��ʽ�Ǻ�����ʾ�ģ����뽫��ʾ���������Ų��ſ���������ʾ��ͼ��������ʾ��
*               SEG1    SEG2    SEG3    SEG4
*       COM1    *       *       *       *
*       COM2    -       -       -       -
*       COM3    -       -       -       -
*       COM4    -       -       -       -
*           *       �D�D������ɨ��
*           -       �D�D������ɨ��
*       stm8l152ɨ�跽ʽ����ɨ����COM1��Ȼ��ɨ��COM2\COM3\COM4����������ƽ�����õ�LCD����о
*   Ƭ��������Ϊ����λ�������Ҳ������һ��оƬ�����Ǹ���SEG��������STM8L152�Ǹ���COM��������
*       2)����ֵ����ԭ��������ʾλ��ȷ������bitλ��ţ�Ȼ����ݶ������λ���
*   ȷ��������ʾ�ڴ����ڵ��ֽڼ��ڸ��ֽڵ�λ�ã�Ȼ���λ���и�ֵ����
*
*/
void disp_digit(u8 pos, u8 val, u8 sts)
{
    u8 pos_byte_1=0, pos_byte_2=0, pos_bit_1=0, pos_bit_2=0;
    /* digit segmented table: 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, A, b, c, d, E, F, S, T, -, P, ''*/
    //const static u8 table_digit[] = {0xFA, 0x60, 0xDC, 0xF4, 0x66, 0xB6, 0xBE, 0xE0, 0xFE, 0xF6, 0xEE, 0x3E, 0x1C, 0x7C, 0x9E, 0x8E, 0xB6, 0x8A, 0x04, 0xCE, 0x00};
    const static u8 table_digit[] = {/*0*/0xAF, /*1*/0x06, /*2*/0xCB, /*3*/0x4F, /*4*/0x66, /*5*/0x6D, /*6*/0xED, /*7*/0x07, /*8*/0xEF, /*9*/0x6F, 
    /*A*/0xE7, /*b*/0xEC, /*C*/0xA9, /*d*/0xCE, /*E*/0xE9, /*F*/0xE1, /*S*/0x6D, /*T*/0xA1, /*-*/0x40, /*P*/0xE3, /*''*/0x00};
    const static u8 table_position[] = {DIGIT3, DIGIT2, DIGIT1, DIGIT5, DIGIT4, DIGIT7, DIGIT6};

    assert_param(pos==DIGIT1 || pos==DIGIT2 || pos==DIGIT3 || pos==DIGIT4 || pos==DIGIT5 || pos==DIGIT6 || pos==DIGIT7);
    assert_param(val <= sizeof(table_digit));

    //printf("[1]pos:%d\r\n", pos);
    /* position remapping */
    pos = table_position[pos];
    //printf("[2]pos:%d\r\n", pos);

    //MYLOG_DEBUG_HEXDUMP("disp:", disp_buf, sizeof(disp_buf));

    /* update screen buffer */
    if (sts == DISPLAY_ON)
    {
        /* COM1��pos��Ҫ*2��ԭ��Ϊÿ����������ռ2�У�*2֮����Զ�λ������COM1�е�POSλ��*/
        //printf("pos:%d/%d\r\n", (56+3+pos*2)/8, (56+3+pos*2)%8);
        pos_byte_1 = (0+pos*2)/8;
        pos_bit_1  = (0+pos*2)%8;
#ifdef HAVE_TEST_DISP
        printf("p:%d/%d,%d/%d\r\n", pos_byte_1, pos_bit_1, pos_byte_2, pos_bit_2);
#endif
        disp_buf[pos_byte_1]  = disp_buf[pos_byte_1] & (~(1<<pos_bit_1));
        disp_buf[pos_byte_1] |= ((table_digit[val]&0x01)>>0)<<pos_bit_1;

        /* COM2 */
        //printf("pos:%d/%d\r\n", (56+3+pos*2)/8, (56+3+pos*2)%8);
        pos_byte_1 = (28+pos*2)/8;
        pos_bit_1  = (28+pos*2)%8;
        pos_byte_2 = (28+pos*2+1)/8;
        pos_bit_2  = (28+pos*2+1)%8;
        //printf("p:%d/%d,%d/%d\r\n", pos_byte_1, pos_bit_1, pos_byte_2, pos_bit_2);
        disp_buf[pos_byte_1]  = disp_buf[pos_byte_1] & (~(1<<pos_bit_1));
        disp_buf[pos_byte_2]  = disp_buf[pos_byte_2] & (~(1<<pos_bit_2));
        disp_buf[pos_byte_1] |= ((table_digit[val]&0x02)>>1)<<pos_bit_1;
        disp_buf[pos_byte_2] |= ((table_digit[val]&0x20)>>5)<<pos_bit_2;

        /* COM3 */
        //printf("pos:%d/%d\r\n", (56+3+pos*2)/8, (56+3+pos*2)%8);
        pos_byte_1 = (56+pos*2)/8;
        pos_bit_1  = (56+pos*2)%8;
        pos_byte_2 = (56+pos*2+1)/8;
        pos_bit_2  = (56+pos*2+1)%8;
        //printf("p:%d/%d,%d/%d\r\n", pos_byte_1, pos_bit_1, pos_byte_2, pos_bit_2);
        disp_buf[pos_byte_1]  = disp_buf[pos_byte_1] & (~(1<<pos_bit_1));
        disp_buf[pos_byte_2]  = disp_buf[pos_byte_2] & (~(1<<pos_bit_2));
        disp_buf[pos_byte_1] |= ((table_digit[val]&0x04)>>2)<<pos_bit_1;
        disp_buf[pos_byte_2] |= ((table_digit[val]&0x40)>>6)<<pos_bit_2;

        /* COM4 */
        //printf("pos:%d/%d\r\n", (56+3+pos*2)/8, (56+3+pos*2)%8);
        pos_byte_1 = (84+pos*2)/8;
        pos_bit_1  = (84+pos*2)%8;
        pos_byte_2 = (84+pos*2+1)/8;
        pos_bit_2  = (84+pos*2+1)%8;
        //printf("p:%d/%d,%d/%d\r\n", pos_byte_1, pos_bit_1, pos_byte_2, pos_bit_2);
        disp_buf[pos_byte_1]  = disp_buf[pos_byte_1] & (~(1<<pos_bit_1));
        disp_buf[pos_byte_2]  = disp_buf[pos_byte_2] & (~(1<<pos_bit_2));
        disp_buf[pos_byte_1] |= ((table_digit[val]&0x08)>>3)<<pos_bit_1;
        disp_buf[pos_byte_2] |= ((table_digit[val]&0x80)>>7)<<pos_bit_2;

        //MYLOG_DEBUG_HEXDUMP("disp:", disp_buf, sizeof(disp_buf));
    }
    else
    {
        /* COM1 */
        disp_buf[(0+pos*2)/8]   = disp_buf[(0+pos*2)/8] & (~(1<<((0+pos*2)%8)));
        /* COM2 */
        disp_buf[(28+pos*2)/8]  = disp_buf[(28+pos*2)/8] & (~(1<<((28+pos*2)%8) | 1<<((28+pos*2+1)%8)));
        /* COM3 */
        disp_buf[(56+pos*2)/8]  = disp_buf[(56+pos*2)/8] & (~(1<<((56+pos*2)%8) | 1<<((56+pos*2+1)%8)));
        /* COM4 */
        disp_buf[(84+pos*2)/8]  = disp_buf[(84+pos*2)/8] & (~(1<<((84+pos*2)%8) | 1<<((84+pos*2+1)%8)));
    }
#ifdef HAVE_TEST_DISP
    printf("%d:\r\n", val);
    disp_hex_dump(disp_buf, sizeof(disp_buf));
#endif    
}

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
void disp_char(u8 start, u8 ch)
{
    u8 val = 0;
    //    unsigned char pos[9] = {IDX_DIGIT_3, IDX_DIGIT_2, IDX_DIGIT_1, IDX_DIGIT_9, IDX_DIGIT_8, IDX_DIGIT_7, IDX_DIGIT_6, IDX_DIGIT_5, IDX_DIGIT_4};
    assert_param(start <= 9);

    switch(ch)
    {
    case '0':
        val = 0;
        break;
    case '1':
        val = 1;
        break;
    case '2':
        val = 2;
        break;
    case '3':
        val = 3;
        break;
    case '4':
        val = 4;
        break;
    case '5':
        val = 5;
        break;
    case '6':
        val = 6;
        break;
    case '7':
        val = 7;
        break;
    case '8':
        val = 8;
        break;
    case '9':
        val = 9;
        break;
    case 'a':
        val = 10;
        break;
    case 'A':
        val = 10;
        break;
    case 'b':
        val = 11;
        break;
    case 'B':
        val = 11;
        break;
    case 'c':
        val = 12;
        break;
    case 'C':
        val = 12;
        break;
    case 'd':
        val = 13;
        break;
    case 'D':
        val = 13;
        break;
    case 'e':
        val = 14;
        break;
    case 'E':
        val = 14;
        break;
    case 'f':
        val = 15;
        break;
    case 'F':
        val = 15;
        break;
    case 's':
        val = 16;
        break;
    case 'S':
        val = 16;
        break;
    case 't':
        val = 17;
        break;
    case 'T':
        val = 17;
        break;
    case '-':
        val = 18;
        break;
    case 'P':
        val = 19;
        break;
    case 'p':
        val = 19;
        break;
    case ' ':
        val = 20;
        break;
    default:
        val = 20;
        break;
    }

    disp_digit(start, val, DISPLAY_ON);
}

/*! \brief
*       display a number
* \param start[IN]      - start position:0~7
* \param end[IN]        - end position:0~7
* \param val[IN]        - value
*/
void disp_number(u8 start, u8 end, u32 val)
{
    //u8 pos[] = {DIGIT1, DIGIT2, DIGIT3, DIGIT4, DIGIT5, DIGIT6, DIGIT7, DIGIT8, DIGIT9, DIGIT10, DIGIT11, DIGIT12};
    u8 i=0, j=0, len=0;
    u8 num[9] = {0};
    u32 fator = 1;

    assert_param( end<=9 && start<=end);

    // MYLOG_DEBUG_HEXDUMP("[0]Display Buffer:", disp_buf, sizeof(disp_buf));

//    for (i=0; i<end-start+1; ++i)
//    {
//        disp_digit(start+i, 0, DISPLAY_OFF);
//    }

    for (i=0; i<(end-start+1); ++i)
    {
        fator = 1;
        for (j=0; j<end-start-i; ++j)
        {
            fator *= 10;
        }

        num[i] = val / fator;
        val %= fator;
    }

    for (i=0; i<end-start+1; ++i)
    {
        if (num[i] != 0)
            break;
    }

    len = end-start+1-i;

    for (i=0; i<len; ++i)
    {
        disp_digit(end-i, num[end-start-i], DISPLAY_ON);
    }

    //MYLOG_DEBUG_HEXDUMP("[1]Display Buffer:", disp_buf, sizeof(disp_buf));
}

/*! \brief
*       display a string
* \param start[IN]      - start position:0~4
* \param end[IN]        - end position:0~4
* \param str[IN]        - value
*/
void disp_str(u8 start, u8 end, const char *str)
{
    u8 i=0;

    //assert_param( end<=12 && start<=end);

    for (i=0; i<end-start+1; ++i)
    {
//        if (str[i]>='0' && str[i]<='9')
//        {
//            disp_digit(start+i, str[i]-'0', DISPLAY_ON);
//        }
//        else
//        {
            disp_char(start+i, str[i]);
//        }
    }
}

/*! \brief
*      ��ʾ�¶�ֵ
* \param temp[IN]         - �¶�ֵ
*/
void disp_temp(s16 temp)
{
    lcd_write_seg(disp_buf, SEG_S9, DISPLAY_ON);//��ʾ ���϶� �� �¶ȵ�С����

    if(temp >= 0)
    {
        if(temp >= 100)
        {
            if(temp <= 999)
            {
                disp_number(0, 2, temp);
            }
            else
            {
                temp = 999;
                disp_number(0, 2, temp);
            }
        }
        else if(temp==0 || temp < 100)
        {
            if(temp == 0)
            {
                disp_digit(0, 0, DISPLAY_OFF);
                disp_digit(1, 0, DISPLAY_ON);
                disp_digit(2, 0, DISPLAY_ON);
            }
            else
            {
                disp_digit(0, 0, DISPLAY_OFF);
                disp_number(1, 2, temp);
            }
        }
    }
    if(temp < 0)
    {

        if(temp*(-1) >= 100)
        {
            temp = 99;
            disp_char(0,'-');
            disp_number(1, 2, temp);
        }
        else if(temp*(-1) < 100)
        {
            disp_char(0,'-');
            disp_number(1, 2, temp*(-1));
        }
    }
}

/*! \brief
*      ��ʾʪ��ֵ
* \param temp[IN]     - ʪ��ֵ
* \note
*
*/
void disp_humi(u16 humi)
{
    //assert_param(humi >= 0);

    lcd_write_seg(disp_buf, SEG_S13, DISPLAY_ON);//��ʾ �ٷֺ� �� ʪ�ȵ�С����

    if(humi >= 10)
    {
        if(humi <= 99)
        {
            disp_number(3, 4, humi);
        }
        else
        {
            humi = 99;
            disp_number(3, 4, humi);
        }
    }
    else if(100 > humi)
    {
        disp_digit(3, 0, DISPLAY_OFF);
        disp_number(4, 4, humi);
    }
    
    if(humi == 0)
    {
        disp_digit(3, 0, DISPLAY_OFF);
        disp_digit(4, 0, DISPLAY_ON);
    }
}

void disp_close_temp_humi(void)
{
    lcd_write_seg(disp_buf, SEG_S9, DISPLAY_OFF);//��ʾ ���϶� �� �¶ȵ�С����   
    lcd_write_seg(disp_buf, SEG_S13, DISPLAY_OFF);//��ʾ �ٷֺ� �� ʪ�ȵ�С����
}

/*! \brief
*      ��ʾ��ص���
* \param value[IN]         - ����ֵ
*/
void disp_battery_level(u16 value)
{
    if(value == 0)
    {
        disp_digit(5, 0, DISPLAY_ON);
        disp_digit(6, 0, DISPLAY_ON);
    }
    else
    {
        disp_number(5, 6, value);
    }
}
