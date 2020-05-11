#ifndef __TH_SHT2x_H
#define __TH_SHT2x_H

#ifdef HAVE_SHT2x

#include "bsp.h"
#include "sht2x.h"

void th_sht2x_load_config(SHT2xConfig *cfg);
void th_sht2x_power_on(void);
void th_sht2x_power_off(void);
void th_sht2x_init(void);
int  th_sht2x_acquisition(s16 *temp, s16 *humi);
void th_sht2x_close(void);

#endif

#endif