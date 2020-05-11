#include "adf.h"
#include "board.h"
#include "sysparams.h"
#include "awu.h"
#include "task_report.h"
#include "task_sample.h"
#include "task_disp.h"
#include "error.h"
#include "disp.h"
#include "log_data.h"
#include "LCD.h"
#include "AdcDriver.h"
#include "task_pwm.h"

void hardware_init(void)
{
    struct tm t;

    // PORT��ʼ��
    GPIO_Init(GPIOA, GPIO_Pin_All, GPIO_Mode_Out_PP_Low_Slow );
    GPIO_Init(GPIOB, GPIO_Pin_All, GPIO_Mode_Out_PP_Low_Slow );
    GPIO_Init(GPIOC, GPIO_Pin_All, GPIO_Mode_Out_PP_Low_Slow );
    GPIO_Init(GPIOD, GPIO_Pin_All, GPIO_Mode_Out_PP_Low_Slow );
    GPIO_Init(GPIOE, GPIO_Pin_All, GPIO_Mode_Out_PP_Low_Slow );
    GPIO_Init(GPIOF, GPIO_Pin_All, GPIO_Mode_Out_PP_Low_Slow );

    GPIO_Init(GPIOD, GPIO_Pin_5, GPIO_Mode_Out_PP_Low_Fast);

    // system clock init
    sysclk_init_hsi(CLK_SYSCLKDiv_1);

    // com load configuration & init
    com_load_config(com_cfg, 1);
    com_set_rx_pin_mode(GPIO_Mode_Out_PP_Low_Fast);
    com_init(COM1, 9600, USART_WordLength_8b, USART_StopBits_1, USART_Parity_No, USART_Mode_All, ENABLE, 0);
    com_recv_init(COM1);

    /* Check if the system has resumed from IWDG reset */
    if (RST_GetFlagStatus(RST_FLAG_IWDGF) != RESET)
    {
        /* Clear IWDGF Flag */
        RST_ClearFlag(RST_FLAG_IWDGF);
        printf("WatchDog Triggered!\r\n");
    }

    /* Check if the system has resumed from BORF reset */
    if (RST_GetFlagStatus(RST_FLAG_BORF) != RESET)
    {
        /* Clear IWDGF Flag */
        RST_ClearFlag(RST_FLAG_BORF);
        printf("BOR Triggered!\r\n");
    }

    /* Check if the system has resumed from ILLOPF reset */
    if (RST_GetFlagStatus(RST_FLAG_ILLOPF) != RESET)
    {
        /* Clear IWDGF Flag */
        RST_ClearFlag(RST_FLAG_ILLOPF);
        printf("ILLOPF Triggered!\r\n");
    }

    rtc_init();

    rtc_read(&t);
    //printf("time:%02d-%02d-%02d %02d:%02d:%02d\r\n", t.tm_year+1900,t.tm_mon+1,t.tm_mday,t.tm_hour,t.tm_min,t.tm_sec);
    if (t.tm_year < 117)
    {
        t.tm_year = 118;
        t.tm_mon  = 8;
        t.tm_mday = 10;
        t.tm_hour = 9;
        t.tm_min  = 0;
        t.tm_sec  = 0;
        rtc_write(t);
    }

    // ��ʼ��NBģ��������ţ������ø�
    BSP_NB_LoadConfig(&nb_config);
    BSP_NB_PowerOn();

    // ��ʪ�ȴ�������ʼ������
    BSP_SHT3x_Load_Config(&sht30_cfg);

    LCD_Config();

    return;
}

void software_init(void)
{
    // ϵͳ������ʼ��
    memset(&g_sys_params, 0, sizeof(g_sys_params));
    memset(&g_run_params, 0, sizeof(g_run_params));

    eeprom_read(0, (u8*)&g_sys_params, sizeof(g_sys_params));

    // ϵͳʱ�䴦��
    if (g_sys_params.t[0] != 0x20)
    {
        memcpy(g_sys_params.t, "\x20\x18\x10\x01\x00\x00\x00", 7);
    }

    rtc_write_bcd(g_sys_params.t, sizeof(g_sys_params.t));

    // �豸�������
    factory_check();

    // �����������ԣ��ڳ���ǰͳһ��ָ������ǻָ�����,ִ������ĳ���
    if (g_sys_params.factory_flag == 0)
    {
        //  ==========����ip��ַ���˿ڣ�apn��ϵͳ����==========

#ifdef HAVE_CT
        /* �Խ���Ϣ��
        ��ʽƽ̨��ҵ�Ż���¼��ַ�� https://device.local.ct10649.com:8843
        �豸�Խӵ�ַ��             117.60.157.137���˿ں�5683��5684��
        Ӧ�öԽӵ�ַ��             https://device.api.ct10649.com:8743 */

        g_sys_params.dev_type = 'T';//�豸���� - ����
		memset(g_sys_params.ip, 0, sizeof(g_sys_params.ip));
//        memcpy(g_sys_params.ip, "180.101.147.115", sizeof("180.101.147.115"));//���ſ�����ƽ̨�Խӵ�ַ
//        g_sys_params.port = 5683;

        memcpy(g_sys_params.ip, "117.60.157.137", sizeof("117.60.157.137"));   //������ʽƽ̨��ַ
        g_sys_params.port = 5683;       //���˿ں�5683��5684��

        memset(g_sys_params.apn, 0, sizeof(g_sys_params.apn));
        memcpy(g_sys_params.apn, "ctnb", strlen("ctnb"));

        g_sys_params.band_no = 2;
#endif

#ifdef HAVE_CU
        g_sys_params.dev_type = 'U';//�豸���� - ��ͨ
        memset(g_sys_params.ip, 0, sizeof(g_sys_params.ip));

        //memcpy(g_sys_params.ip, "58.240.96.46", sizeof("58.240.96.46"));// ����ʽƽ̨����Ϊ��ͨ�����������豸���ӵ�ַ
        //g_sys_params.port = 5683;

        memcpy(g_sys_params.ip, "117.78.42.93", sizeof("117.78.42.93"));// ��ͨ����ʽƽ̨
        g_sys_params.port = 5683;

        //memcpy(g_sys_params.ip, "49.4.85.232", sizeof("49.4.85.232"));// ��ͨ������ƽ̨
        //g_sys_params.port = 5683;

        memset(g_sys_params.apn, 0, sizeof(g_sys_params.apn));
        memcpy(g_sys_params.apn, "nbiot", strlen("nbiot"));

        g_sys_params.band_no = 1;
#endif

        //  ==========����Ĭ�ϵĹ�ů��ϵͳ����==========
#if defined( HAVE_20MIN)
        g_sys_params.upload_cycle = 60;//�ϴ�����
        g_sys_params.sample_cycle = 20;//�ɼ�����
#else
        g_sys_params.upload_cycle = 60;
        g_sys_params.sample_cycle = 30;
#endif
        g_run_params.temp         = 888;
        g_run_params.humi         = 888;
        g_run_params.rssi         = 0;
        g_sys_params.records_rd   = 0;
        g_sys_params.records_wr   = 0;

        //  ==========����ϵͳ����==========
        sysparams_write(0, (u8*)&g_sys_params, sizeof(g_sys_params));

        log_clear_data();
    }

    g_run_params.re_power_on_flag   = 1;
    //g_run_params.net_state          = 2;

    return;
}

#if 0
void system_print(void)
{
    //u8 i = 0;
    struct tm t;
    //struct tm *t1;
    //time_t t2;

    rtc_read(&t);

    printf("\f");
    printf("Manufacturer:%s\r\n", manufacturer);
    printf("ProductModel:%s\r\n", product_model);
    printf("Hardware Ver:%s\r\n", hw_ver);
    printf("Software Ver:%s\r\n\r\n", sw_ver);

    printf("Factory Mode:%d\r\n", g_sys_params.factory_flag);
    printf("Product Code:%s\r\n", g_sys_params.addr);
    printf("Upload Cycle:%u, Sample Cycle:%u\r\n", g_sys_params.upload_cycle, g_sys_params.sample_cycle);
    printf("Time:%04d-%02d-%02d %02d:%02d:%02d\r\n", t.tm_year+1900, t.tm_mon+1, t.tm_mday, t.tm_hour, t.tm_min, t.tm_sec);
    printf("Read Flag:%lu, Write Flag:%lu.\r\n", g_sys_params.records_rd, g_sys_params.records_wr);

    printf("Ip:%s, Port:%u, Apn:%s.\r\n", g_sys_params.ip, g_sys_params.port, g_sys_params.apn);
    printf("Band:%s.\r\n", g_band_list[g_sys_params.band_no]);

#if 0
    {
        int i = 0;

        printf("sizeof(g_sys_params):%d\r\n", sizeof(g_sys_params));

        for (i=0; i<72; ++i)
        {
            log_read(&g_last_log_data, i);

    #if 1
            printf("temp:%d, humi:%u, rssi:%d, st:%d\r\n", g_last_log_data.temp, g_last_log_data.humi, g_last_log_data.rssi, g_last_log_data.st);
            printf("time:%02X%02X-%02X-%02X %02X:%02X:%02X\r\n",
                       g_last_log_data.sample_time[0],
                       g_last_log_data.sample_time[1],
                       g_last_log_data.sample_time[2],
                       g_last_log_data.sample_time[3],
                       g_last_log_data.sample_time[4],
                       g_last_log_data.sample_time[5],
                       g_last_log_data.sample_time[6]);
    #endif
        }
    }
#endif

}
#endif

void main(void)
{
    hardware_init();
    software_init();
    //system_print();

    LCD_ClearScreen();                  //�����Ļ������ʾ
    LCD_poll_disp();                    //��Ļ��������ѯ��ʾ

    TIMER1_PWMInit(PULSEFREQUENCY, MinimumPulseWidth);
    ADC_Initialization();               //ADC�ɼ���ʼ��

    //PWM
    task_pwm_triggered(E_TASK_PWM_GET_ADC_VALUE);

    //�ϱ����̳�ʼ��
    task_report_init();

    //�ɼ����̳�ʼ��
    task_sample_init(g_sys_params.sample_cycle);

    BSP_NB_SetRepeatSendCounter(UNIT_SECOND*6);

    //��ʾ
    task_disp_triggered(E_TASK_DISP_ANT);

    awu_init();
    awu_enable(CYCLE_AWU);
    wdg_init(1500);

    PWR_UltraLowPowerCmd(ENABLE);
    enableInterrupts();
    com_recv_init(COM1);

    while (1)
    {
        //���ܴ�halt�͹��ģ���Ϊ����ʹ�ö�ʱ����ʱPWM�����ʹ��halt ���±��ⲻ����
        //�ϱ����������󣬽���WFIģʽ��ʹ���ڿ���������

        //if (task_report_get_state() != E_REPORT_IDLE /*|| task_sample_get_state() != 0*/)
        //{
            wfi();
        //}
        //else
        //{
        //    halt();
        //}
        //
        //wdg_reset();
    }
}

#ifdef  USE_FULL_ASSERT

/**
* @brief  Reports the name of the source file and the source line number
*   where the assert_param error has occurred.
* @param  file: pointer to the source file name
* @param  line: assert_param error line source number
* @retval None
*/
void assert_failed(uint8_t* file, uint32_t line)
{
    /* User can add his own implementation to report the file name and line number,
    ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

    printf("Wrong parameters value: file %s on line %lu\r\n", file, line);

    /* Infinite loop */
    while (1)
    {
    }
}
#endif




//        rtc_read(&t);
//        printf("%02d-%02d-%02d %02d:%02d:%02d - SYS_TIME\r\n", t.tm_year+1900,t.tm_mon+1,t.tm_mday,t.tm_hour,t.tm_min,t.tm_sec);
//        delay_ms(10000);



//    printf("Time:%02X%02X-%02X-%02X %02X:%02X:%02X\r\n",
//       g_sys_params.t[0],
//       g_sys_params.t[1],
//       g_sys_params.t[2],
//       g_sys_params.t[3],
//       g_sys_params.t[4],
//       g_sys_params.t[5],
//       g_sys_params.t[6]
//       );

