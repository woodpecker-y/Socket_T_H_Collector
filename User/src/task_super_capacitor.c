#include "adf.h"
#include "task_super_capacitor.h"
#include "task_sample.h"
#include "task_report.h"
#include "sysparams.h"
//#include "disp.h"
//#include "task_disp.h"
//#include "rtc.h"
#include "board.h"

typedef enum _SuperCapacitorState{
    E_SC_STATE_IDLE = 0,
    E_SC_STATE_RECHARGE,
    E_SC_STATE_FINISH,
}SuperCapacitorState;

typedef struct _SuperCapacitorHandler{
    u8  state;
    u32 recharge_counter;
    u32 sample_counter;
    u8  recharge_max_counter;
    u8  recharge_mode;
    u8  report_triggered;
    u8  recharge_switch;
    s32 exit_fast_recharge_timer;
    u8  exit_fast_recharge_flag;
}SuperCapacitorHandler;

static SuperCapacitorHandler s_super_capacitor_handler;

void task_super_capacitor_init(u8 have_lights_on)
{
    s_super_capacitor_handler.recharge_switch = 0;
    //    if (have_lights_on == 1)
    //    {
    s_super_capacitor_handler.recharge_max_counter = 2;
    //    }
    //    else
    //    {
    //        s_super_capacitor_handler.recharge_max_counter = SUPER_CAPACITOR_RECHARGE_IDLE_COUNTER;
    //    }
}

void task_super_capacitor_proc(void)
{
    // 切换状态
    if (E_SUPERCAPACITOR_MODE_NORMAL == s_super_capacitor_handler.recharge_mode)
    {
        if (s_super_capacitor_handler.recharge_switch == 1)
        {
            s_super_capacitor_handler.recharge_counter++;
            if( s_super_capacitor_handler.recharge_counter >= s_super_capacitor_handler.recharge_max_counter)
            {
                //        s_super_capacitor_handler.state = E_SC_STATE_RECHARGE;
                s_super_capacitor_handler.recharge_counter = 0;
                //            if (g_sys_params.light_sts[E_LIGHT_1] == E_LIGHT_ON || g_sys_params.light_sts[E_LIGHT_2] == E_LIGHT_ON)
                //            {
                s_super_capacitor_handler.recharge_max_counter = 2;
                //            }
                //            else
                //            {
                //                s_super_capacitor_handler.recharge_max_counter = SUPER_CAPACITOR_RECHARGE_IDLE_COUNTER;
                //            }
                //        delay_us_by_timer(10);
                //                GPIO_Init(GPIOC, GPIO_Pin_7, GPIO_Mode_Out_PP_High_Fast );
                if (task_report_get_state() == 0
                    && task_sample_get_state() == 0)
                {
                    GPIO_SetBits(GPIOE, GPIO_Pin_6);
                    //        gpio_set_high(super_capacitor_recharge_cfg.port, super_capacitor_recharge_cfg.pin);
                    
                    //delay_us(500);
                    //delay_ms(5);
                    ////        while (us--)
                    ////        {
                    ////            asm("nop");
                    ////        }
                    
                    //        gpio_set_low(super_capacitor_recharge_cfg.port, super_capacitor_recharge_cfg.pin);     
                    GPIO_ResetBits(GPIOE, GPIO_Pin_6);
                }
                //printf("recharge start\r\n");
            }    
        }
    }
    else if (E_SUPERCAPACITOR_MODE_FAST == s_super_capacitor_handler.recharge_mode)
    {
        s_super_capacitor_handler.recharge_counter++;
        if( s_super_capacitor_handler.recharge_counter >= s_super_capacitor_handler.recharge_max_counter)
        {
            s_super_capacitor_handler.recharge_counter = 0;
            
            GPIO_SetBits(GPIOE, GPIO_Pin_6);
            delay_ms(30);
            GPIO_ResetBits(GPIOE, GPIO_Pin_6);
            
            s_super_capacitor_handler.sample_counter++;
            
            if (s_super_capacitor_handler.sample_counter >= SUPER_CAPACITOR_RECHARGE_COUNTER)
            {
                s_super_capacitor_handler.sample_counter = 0;
                g_run_params.super_capacitor_voltage = task_super_capacitor_read_voltage();
//                g_run_params.super_capacitor_voltage = 36;
                if (g_run_params.super_capacitor_voltage >= 36)
                {
                    s_super_capacitor_handler.recharge_mode = E_SUPERCAPACITOR_MODE_DIRECT_CONNECTION;
                    
                    GPIO_SetBits(GPIOE, GPIO_Pin_6);
                }
            }
        }            
    }
    else if (s_super_capacitor_handler.recharge_mode == E_SUPERCAPACITOR_MODE_DIRECT_CONNECTION)
    { 
        s_super_capacitor_handler.sample_counter++;
        if (s_super_capacitor_handler.sample_counter >= SUPER_CAPACITOR_RECHARGE_COUNTER)
        {
            s_super_capacitor_handler.sample_counter = 0;
            
            g_run_params.super_capacitor_voltage = task_super_capacitor_read_voltage();
//            g_run_params.super_capacitor_voltage = 49;
            if (g_run_params.super_capacitor_voltage >= 49)
            {
                if (s_super_capacitor_handler.report_triggered == 0)
                {
                    // 快速模式下增加联网超时时间，因为供电能力够
                    task_report_set_max_connect_timeout(MAX_CONNECT_TIMEOUT_FAST);
                    
                    task_report_triggered(WORKMODE_NORMAL);
                    
                    s_super_capacitor_handler.report_triggered = 1;
                    
#if 0
                    GPIO_ResetBits(GPIOE, GPIO_Pin_6);
                    s_super_capacitor_handler.recharge_mode = E_SUPERCAPACITOR_MODE_NORMAL;
                    s_super_capacitor_handler.report_triggered = 0;
#endif
                }
                
                if (task_report_get_net_state() == 1 && task_report_get_state() == 0 && s_super_capacitor_handler.exit_fast_recharge_flag == 0)
                {
                    s_super_capacitor_handler.exit_fast_recharge_flag = 1;
                    s_super_capacitor_handler.exit_fast_recharge_timer = UNIT_SECOND*20;
                    //        #if 1
                    //                        // 联网后关闭充电
                    //                        task_super_capacitor_ctrl(0);
                    //        #endif
                }
            }  
        }
        
        // 联网后且退出快速充电模式生效，计数器倒计时开始计时
        if (s_super_capacitor_handler.exit_fast_recharge_flag == 1)
        {
            s_super_capacitor_handler.exit_fast_recharge_timer--;
            if (s_super_capacitor_handler.exit_fast_recharge_timer <= 0)
            {
                GPIO_ResetBits(GPIOE, GPIO_Pin_6);
                s_super_capacitor_handler.recharge_mode = E_SUPERCAPACITOR_MODE_NORMAL;
                
                s_super_capacitor_handler.report_triggered = 0;
                
                // 普通模式下恢复联网超时时间，因为供电能力不足
                task_report_set_max_connect_timeout(MAX_CONNECT_TIMEOUT_NORMAL);
                
                // 联网成功后如果有一个灯打开，则关闭充电
                if (g_sys_params.light_sts[E_LIGHT_1] == E_LIGHT_ON || g_sys_params.light_sts[E_LIGHT_2] == E_LIGHT_ON)
                {
                    task_super_capacitor_ctrl(0);
                }
                
                s_super_capacitor_handler.exit_fast_recharge_flag = 0;
                s_super_capacitor_handler.exit_fast_recharge_timer = 0;
            }            
        }
        
        //            if (task_report_get_net_state() == 1 && g_run_params.super_capacitor_voltage >= 49)
        //            {
        //                GPIO_ResetBits(GPIOE, GPIO_Pin_6);
        //                s_super_capacitor_handler.recharge_mode = E_SUPERCAPACITOR_MODE_NORMAL;
        //                
        //                s_super_capacitor_handler.report_triggered = 0;
        //                
        //                // 普通模式下恢复联网超时时间，因为供电能力不足
        //                task_report_set_max_connect_timeout(MAX_CONNECT_TIMEOUT_NORMAL);
        //                
        //#if 1
        //                // 联网后关闭充电
        //                task_super_capacitor_ctrl(0);
        //#endif
        //            }  
    }
    
    //    switch(s_super_capacitor_handler.state)
    //    {
    //    //等待状态
    //    case E_SC_STATE_IDLE:     
    //        break;
    //    //准备状态
    //    case E_SC_STATE_RECHARGE:
    //        // 切换电平为低，启动充电
    //        //gpio_set_low(super_capacitor_recharge_cfg.port, super_capacitor_recharge_cfg.pin);
    //        gpio_set_high(super_capacitor_recharge_cfg.port, super_capacitor_recharge_cfg.pin);
    //        delay_us(10);
    //        gpio_set_low(super_capacitor_recharge_cfg.port, super_capacitor_recharge_cfg.pin);
    //        
    ////        // 满足触发条件时，启动超级电容采样，开始初始化
    ////        s_super_capacitor_handler.sample_counter++;
    ////        if (s_super_capacitor_handler.sample_counter >= SUPER_CAPACITOR_RECHARGE_COUNTER)
    ////        {
    ////            // 初始化超级电容采样
    ////            adc_power_on(SUPER_CAPACITOR_RECHARGE);
    ////            adc_init(SUPER_CAPACITOR_RECHARGE);
    ////        }
    //    
    //        // 切换状态
    //        //s_super_capacitor_handler.state = E_SC_STATE_FINISH;
    //        s_super_capacitor_handler.state = E_SC_STATE_IDLE;
    //        break; 
    ////    case E_SC_STATE_FINISH:
    ////        // 恢复电平为高，关闭充电
    ////        //s_super_capacitor_handler.recharge_counter = 1;
    ////        //gpio_set_high(super_capacitor_recharge_cfg.port, super_capacitor_recharge_cfg.pin);
    ////        //gpio_set_low(super_capacitor_recharge_cfg.port, super_capacitor_recharge_cfg.pin);
    ////        //printf("recharge finsh\r\n");
    //////        // 满足触发条件时，启动超级电容采样，执行采样
    //////        if (s_super_capacitor_handler.sample_counter >= SUPER_CAPACITOR_RECHARGE_COUNTER)
    //////        {     
    //////            // 恢复计数器
    //////            s_super_capacitor_handler.sample_counter = 0;
    //////            
    //////            // 对超级电容采样
    //////            g_run_params.super_capacitor_voltage = adc_sample(SUPER_CAPACITOR_RECHARGE);
    //////            
    //////#ifdef HAVE_TEST
    //////            //printf("voltage[super capacitor]=%u\r\n", g_run_params.super_capacitor_voltage);
    //////#endif
    //////            // 关闭释放超级电容采样电路
    //////            adc_close(SUPER_CAPACITOR_RECHARGE);
    //////            adc_power_off(SUPER_CAPACITOR_RECHARGE);
    //////        }
    ////        
    ////        // 切换状态
    ////        s_super_capacitor_handler.state = E_SC_STATE_IDLE;
    ////        break;
    //    default:
    //        break;
    //    }
}

u8 task_super_capacitor_get_mode(void)
{
    return s_super_capacitor_handler.recharge_mode;
}

void task_super_capacitor_ctrl(u8 enable)
{
    s_super_capacitor_handler.recharge_switch = enable;
    s_super_capacitor_handler.recharge_counter = 0;
}

void task_super_capacitor_fast_triggered(void)
{
    s_super_capacitor_handler.recharge_mode = E_SUPERCAPACITOR_MODE_FAST;
}

u16 task_super_capacitor_read_voltage(void)
{
    u16 super_capacitor_voltage = 0;
    
    // 初始化超级电容采样
    adc_power_on(SUPER_CAPACITOR_RECHARGE);
    adc_init(SUPER_CAPACITOR_RECHARGE);
    
    delay_ms(5);
    
    // 对超级电容采样
    super_capacitor_voltage = adc_sample(SUPER_CAPACITOR_RECHARGE); 
//    super_capacitor_voltage = 3038;
    
    // 电压转换
    super_capacitor_voltage = super_capacitor_voltage / 62;
    
    // 关闭释放超级电容采样电路
    adc_close(SUPER_CAPACITOR_RECHARGE);
    adc_power_off(SUPER_CAPACITOR_RECHARGE);      
    
    return super_capacitor_voltage;
}