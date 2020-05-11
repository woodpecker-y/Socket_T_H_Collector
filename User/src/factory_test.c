#include "adf.h"
#include "motor.h"
#include "factory_test.h"
#include "board.h"
#include "utili.h"

// 时间测试
void factory_test_time(void);

void factory_test(void)
{
    // 时间测试
    factory_test_time();
    wdg_reset();    
    
    // 阀门ADC测试
    //factory_test_valve_adc();
    wdg_reset();        
}

void factory_test_time(void)
{
    int i = 0;
    u32 time_start = 0, time_end = 0;
    
    struct tm cur_time_start, cur_time_end;
    
    rtc_read(&cur_time_start);
    wdg_reset();

    for(i=0; i<4; i++)
    {
        delay_ms(500);
        wdg_reset();
    }

    //disableInterrupts();

    // 时间判断
    rtc_read(&cur_time_end);
    wdg_reset();
    time_start = mktime_user(cur_time_start.tm_year+1900,cur_time_start.tm_mon+1,cur_time_start.tm_mday, cur_time_start.tm_hour, cur_time_start.tm_min, cur_time_start.tm_sec);
    wdg_reset();
    time_end = mktime_user(cur_time_end.tm_year+1900,cur_time_end.tm_mon+1,cur_time_end.tm_mday, cur_time_end.tm_hour, cur_time_end.tm_min, cur_time_end.tm_sec);
    wdg_reset();
    if((time_end-time_end <= 3) && (time_end-time_start >= 1))
    {
        printf("Time Ok\r\n");
    }
    else
    {
        printf("Time Error\r\n");
    }    
}

void factory_test_light(void)
{
    int cnt = 0;
    
    do{
        led_off(E_LIGHT_1);
        led_off(E_LIGHT_2);
        printf("light 1/2 on\r\n");
        delay_ms(5000);
        
        led_on(E_LIGHT_1);
        led_on(E_LIGHT_2);
        printf("light 1/2 off\r\n");
        delay_ms(5000);
        
        cnt++;
    }while (cnt < 10);
}

void factory_test_super_capacitor(void)
{
    u16 adc = 0;
            
    adc_power_on_opendrain(SUPER_CAPACITOR_RECHARGE);
    adc_init(SUPER_CAPACITOR_RECHARGE);
    
    gpio_init(super_capacitor_recharge_cfg.port, super_capacitor_recharge_cfg.pin, GPIO_Mode_Out_OD_Low_Fast);
  
    while (1)
    {
        adc = adc_sample(SUPER_CAPACITOR_RECHARGE); 
        
        printf("adc=%u\r\n", adc);
        
        gpio_set_low(super_capacitor_recharge_cfg.port, super_capacitor_recharge_cfg.pin);
        delay_ms(50);
        
        gpio_set_high(super_capacitor_recharge_cfg.port, super_capacitor_recharge_cfg.pin);
        //gpio_init(super_capacitor_recharge_cfg.port, super_capacitor_recharge_cfg.pin, GPIO_Mode_Out_PP_High_Fast);
        delay_ms(950);
    }
    
    adc_close(SUPER_CAPACITOR_RECHARGE);
}

void factory_test_temp_humi_sample(void)
{
//    int rv = 0;
//    int count = 0;
//    s16 temp = 0;
//    s16 humi = 0;    
//    
//    th_sht3x_power_on();
//    th_sht3x_init();
//    
//    do{
//        rv = th_sht3x_acquisition(&temp, &humi);    
//
//        printf("rv:%d, temp=%d, humi:%d\r\n", rv, temp, humi);
//    
//        count++;
//        delay_ms(1000);
//    }while (count < 10);
//    
//    //关闭SHT20
//    th_sht3x_close();
//    th_sht3x_power_off();
}
