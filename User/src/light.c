#include "adf.h"
#include "light.h"
//#include "led.h"

void light_load_config(PortTypeDef *light_config, int cnt)
{
    // Light Init,�ߵ�ƽ��Ч����Ҫ����
    led_load_config(light_config, cnt);    
}

void light_init(void)
{
    led_init();
}

void light_off(u8 idx)
{
    led_on(idx);
}

void light_on(u8 idx)
{
    led_off(idx);
}

