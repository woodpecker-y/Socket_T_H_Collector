#ifndef __TH_SHT3X_H
#define __TH_SHT3X_H

#ifdef HAVE_SHT3x

#include "bsp.h"
#include "sht3x.h"

void th_sht3x_load_config(SHT3xConfig *cfg);
void th_sht3x_power_on(void);
void th_sht3x_power_off(void);
void th_sht3x_init(void);
int  th_sht3x_acquisition(s16 *temp, s16 *humi);
void th_sht3x_close(void);

#endif

#endif