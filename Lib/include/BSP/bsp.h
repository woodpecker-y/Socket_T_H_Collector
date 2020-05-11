/* This file has been prepared for Doxygen automatic documentation generation.*/
/*! \file *********************************************************************
 *
 * \brief
 *    ∞Âø®∂®“Â
 * \author
 *    zhou_shimin(at)blackants.com.cn
 * \date
 *    2014-07-13
 * \version
 *    v1.0
 * Copyright (c) 2010-2014, Blackants Energy Saving Technology co., LTD All rights reserved.
 ******************************************************************************/
#ifndef _BSP_H
#define _BSP_H

#include <stdio.h>
#include <string.h>
#include <assert.h>

typedef enum _LevelEffType{
    E_LE_LOW = 0,
    E_LE_HIGH,
}LevelEffType;

#ifdef HAVE_STM8S
#include "stm8s.h"
#else
#include "stm8l15x.h"
#endif
#include "bsp_sysclk.h"
#include "bsp_gpio.h"
#include "delay.h"
#include "remap.h"

//////////////////// BEEP≈‰÷√ //////////////////////////////
#ifdef HAVE_BEEP
#include "bsp_beep.h"
#endif

//////////////////// RTC≈‰÷√ //////////////////////////////
#ifdef HAVE_RTC
#include "bsp_rtc.h"
#endif

//////////////////// »Ìº˛∂® ±∆˜≈‰÷√ //////////////////////////////
#ifdef HAVE_SYSTIMER
#include "bsp_systimer.h"
#endif

//////////////////// LCD≈‰÷√ //////////////////////////////
#if defined(HAVE_LCD_EXT)

#include "bu97931.h"

#elif defined(HAVE_LCD)

#include "bsp_lcd.h"

#endif

//////////////////// RF≈‰÷√ //////////////////////////////
#ifdef HAVE_RF

#include "si4432.h"

#endif

//////////////////// ∑ß√≈≈‰÷√ //////////////////////////////
#ifdef HAVE_MOTOR

#include "ba6289.h"

#endif

//////////////////// ∞¥º¸≈‰÷√ //////////////////////////////
#ifdef HAVE_KEY

#include "bsp_key.h"

#endif

//////////////////// AD≤…—˘≈‰÷√ ////////////////////////////
#ifdef HAVE_ADC

#include "bsp_adc.h"

#endif

//////////////////// UART≈‰÷√ //////////////////////////////
#ifdef HAVE_COM

#include "bsp_com.h"

//#define USART1_REMAP                    // ¥Æø⁄1÷ÿ”≥…‰

//extern const COM_Cfg_t com_cfg[1];      // ¥Æø⁄∂®“Â

#endif

//////////////////// EXTI≈‰÷√ //////////////////////////////
#ifdef HAVE_EXTI

#include "bsp_exti.h"

#endif

//////////////////// AWU≈‰÷√ //////////////////////////////
#ifdef HAVE_AWU

#include "bsp_awu.h"

#endif

//////////////////// ALARM≈‰÷√ //////////////////////////////
#ifdef HAVE_ALARM

#include "bsp_alarm.h"

#endif

//////////////////// LED≈‰÷√ //////////////////////////////
#ifdef HAVE_LED

#include "bsp_led.h"

#endif

//////////////////// EEPROM≈‰÷√ //////////////////////////////
#ifdef HAVE_EEPROM

#include "bsp_eeprom.h"

#endif

//////////////////// RFID≈‰÷√ //////////////////////////////
#ifdef HAVE_RFID

#include "rc522.h"

#endif

//////////////////// ø¥√≈π∑≈‰÷√ //////////////////////////////
#ifdef HAVE_WDG

#include "bsp_wdg.h"

#endif

//////////////////// sht2x≈‰÷√ //////////////////////////////
#ifdef HAVE_SHT2x

#include "sht2x.h"

#endif

//////////////////// sht3x≈‰÷√ //////////////////////////////
#ifdef HAVE_SHT3x

#include "sht3x.h"

#endif

//////////////////// ø¥√≈π∑≈‰÷√ //////////////////////////////
#ifdef HAVE_GPRS

#include "a6.h"

#endif

#endif
