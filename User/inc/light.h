#ifndef _LIGHT_H
#define _LIGHT_H

void light_load_config(PortTypeDef *light_config, int cnt);

void light_init(void);

void light_off(u8 idx);

void light_on(u8 idx);

#endif