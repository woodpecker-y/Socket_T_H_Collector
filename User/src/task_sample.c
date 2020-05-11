#include "task_sample.h"
#include "task_disp.h"
#include "sysparams.h"
#include "log_data.h"
#include "sht3x.h"
#include "error.h"
#include "task_report.h"
#include "update_cycle.h"

CollectHandler s_collect_handler;

void task_sample_storage(void);

void task_sample_init(s32 sample_cycle)
{
    //��ֵ��ʼ�Ĳɼ�ʱ��
    s_collect_handler.refresh_timer   = UNIT_SECOND/2;
    s_collect_handler.storage_timer = (update_timer(sample_cycle, 0))*UNIT_SECOND;
    //s_collect_handler.storage_timer = sample_cycle*UNIT_MIN;
    s_collect_handler.sample_cycle  = sample_cycle;
    task_sample_triggered();
}

void task_sample_proc(void)
{
    s16     temp = 0;                                           // �¶�
    s16     humi = 0;

    // �洢����������ʱ
    s_collect_handler.storage_timer--;

    // ˢ�¼���������ʱ
    s_collect_handler.refresh_timer--;
    if(s_collect_handler.refresh_timer <= 0)
    {
        s_collect_handler.sts = E_COLLECT_REDAY;
        s_collect_handler.refresh_timer = MAX_IDLE_TEMP_TIMER;//10���Ӳɼ�һ����ʪ��
    }

    switch(s_collect_handler.sts)
    {
    case E_COLLECT_IDLE:

        break;
    case E_COLLECT_REDAY:       //׼��״̬
        BSP_SHT3x_PowerOn();
        BSP_SHT3x_Init();
        s_collect_handler.retry_cnt = 0;
        s_collect_handler.sts = E_COLLECT_ACTIVE;
        break;
    case E_COLLECT_ACTIVE:      //�ɼ�״̬
        {
            u8 rv = 0;
            rv = BSP_SHT3x_Acquisition(&temp, &humi);
            if(rv != 0)
            {
                error_set(FAULT_TEMP);
                error_set(FAULT_HUMI);
            }
            //printf("rv = %d\r\n", rv);
#if TEMPERATURE_COMPENSATION
            u8    value_fix = 0;

            switch(g_run_params.backlight_v)
            {
            case 1:
                value_fix = 0;
                break;
            case 2:
                value_fix = 2;
                break;
            case 3:
                value_fix = 5;
                break;
            case 4:
                value_fix = 8;
                break;
            case 5:
                value_fix = 10;
                break;
            default:
                value_fix = 10;
                break;
            }
            temp -= (20 + value_fix);
#endif

            //��������ֵ��ʾ
            //printf("value_fix = %d\r\n", value_fix);
            temp += g_sys_params.fix_temp;
            humi += g_sys_params.fix_humi;

//            printf("[temp]=%d\r\n\r\n", temp);
//            printf("[humi]=%d\r\n\r\n", humi);

            // �����쳣ֵ�жϣ������쳣ֵ��������һ�ε�ֵ
            if( (temp>=-200) && (temp<1500) )
            {
                g_run_params.temp = temp;
                error_clr(FAULT_TEMP);
            }

            if( (humi>0) && (humi<1000) )
            {
                g_run_params.humi = humi;
                error_clr(FAULT_HUMI);
            }

            if (((temp>=-200)&&(temp<1500)) && humi > 0)
            {
                s_collect_handler.retry_cnt = 0;
            }
            else
            {
                s_collect_handler.retry_cnt++;
            }

            // �������Ի���:����3�βɼ�ʧ�ܺ�Ż���ʾ�����־��һ���ָ������־����0
            if (s_collect_handler.retry_cnt >= MAX_RETRY_CNT)
            {
                if ( (temp<-200) || (temp>1500) )
                    error_set(FAULT_TEMP);

                if ( (humi==0) || (humi>1000) )
                    error_set(FAULT_HUMI);
            }

//            printf("[G_temp]=%d\r\n\r\n", g_run_params.temp);
//            printf("[G_humi]=%d\r\n\r\n", g_run_params.humi);

            s_collect_handler.sts = E_COLLECT_FINISH ;
        }
        break;
    case E_COLLECT_FINISH:      //�ɼ�����״̬
        BSP_SHT3x_Close();
        BSP_SHT3x_PowerOff();

        if (s_collect_handler.storage_timer <= 0)
        {
            // ���¼�����
            //s_collect_handler.storage_timer = s_collect_handler.sample_cycle*UNIT_MIN;
            s_collect_handler.storage_timer = (update_timer(s_collect_handler.sample_cycle, 0))*UNIT_SECOND;

            // ��ֹʱ����º󣬼�¼�ظ�
            if (s_collect_handler.storage_timer/UNIT_SECOND >= s_collect_handler.sample_cycle*60*1/3)
            {
                if(g_run_params.rtc_updata == 1)//ֻ�е�����ʱ��֮����ܴ�������
                {
                    task_sample_storage();                // �洢����
                }
            }
        }
        s_collect_handler.sts = E_COLLECT_IDLE;
        break;
    default:
        break;
    }
}

void task_sample_triggered(void)
{
    s_collect_handler.sts = E_COLLECT_REDAY;
}

u8 task_sample_get_state(void)
{
    return s_collect_handler.sts;
}

void task_sample_storage(void)
{
    u16 hour = 0, min = 0;
    u8 time[7] = {0};

    // �������ݵ���������
    g_last_log_data.temp    = g_run_params.temp;
    g_last_log_data.humi    = g_run_params.humi/10;
    g_last_log_data.rssi    = g_run_params.rssi;
    g_last_log_data.st      = g_run_params.st;

    // ��ȡ��ǰʱ��
    rtc_read_bcd(g_sys_params.t, 7);
#if 0
    printf("\r\nSYS_TIME:%02X%02X-%02X-%02X %02X:%02X:%02X\r\n",
               g_sys_params.t[0],
               g_sys_params.t[1],
               g_sys_params.t[2],
               g_sys_params.t[3],
               g_sys_params.t[4],
               g_sys_params.t[5],
               g_sys_params.t[6]);
#endif
    // ʱ�������Ҫ������洢 //20190530T154000Z
    memcpy(time, g_sys_params.t, 7);
    hour = (time[4]>>4&0x0F)*10 + (time[4]&0x0F);//BCD ת10����
    min  = (time[5]>>4&0x0F)*10 + (time[5]&0x0F);//BCD ת10����
    time[6] = 0;

    min = (( hour*60 + min ) / s_collect_handler.sample_cycle ) * s_collect_handler.sample_cycle % 60;

    time[5] = ( (min/10) << 4 ) | ( min % 10 );

//    if (task_report_get_state() != E_REPORT_IDLE)
//    {
//        printf("[2]Time:%02X%02X-%02X-%02X %02X:%02X:%02X\r\n",
//               time[0],
//               time[1],
//               time[2],
//               time[3],
//               time[4],
//               time[5],
//               time[6]
//                   );
//    }

    // ����ʱ�䵽��������
    memcpy(g_last_log_data.sample_time, time, sizeof(g_last_log_data.sample_time));

#if 0
    printf("[MEMORY] temp:%d, humi:%d, rssi:%d, st:%02X\r\n", g_last_log_data.temp, g_last_log_data.humi, g_last_log_data.rssi, g_last_log_data.st);
    printf("time:%02X%02X-%02X-%02X %02X:%02X:%02X\r\n",
               g_last_log_data.sample_time[0],
               g_last_log_data.sample_time[1],
               g_last_log_data.sample_time[2],
               g_last_log_data.sample_time[3],
               g_last_log_data.sample_time[4],
               g_last_log_data.sample_time[5],
               g_last_log_data.sample_time[6]);
#endif

#if 0
    printf("records_wr:%lu\r\n", g_sys_params.records_wr);
#endif

    // �洢д�����־
    log_write(&g_last_log_data, g_sys_params.records_wr);

    // д��¼�ż�1
    g_sys_params.records_wr++;
    if (g_sys_params.records_wr >= LOG_MAX_RECORDS_COUNT)
    {
        g_sys_params.records_wr = 0;
    }

    // ���������д��¼�� �Ѿ����߽�
    if (g_sys_params.records_wr == g_sys_params.records_rd)
    {
        g_sys_params.records_rd++;

        // �������������¼�� �Ѿ����߽�
        if (g_sys_params.records_rd >= LOG_MAX_RECORDS_COUNT)
        {
            g_sys_params.records_rd = 0;
        }
    }

#if 0
    printf("[1]records_rd:%lu, records_wr:%lu\r\n", g_sys_params.records_rd, g_sys_params.records_wr);
#endif

    // ���´洢��־
    sysparams_write(E_ADDR_OFFSET_RECORDS_READ, (u8*)&g_sys_params.records_rd, sizeof(g_sys_params.records_rd));
    sysparams_write(E_ADDR_OFFSET_RECORDS_WRITE, (u8*)&g_sys_params.records_wr, sizeof(g_sys_params.records_wr));

    return;
}