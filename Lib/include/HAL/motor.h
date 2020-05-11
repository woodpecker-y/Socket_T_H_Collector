/* This file has been prepared for Doxygen automatic documentation generation.*/
/*! \file *********************************************************************
*
* \brief
*    hardware abstract layer--motor driver
* \author
*    zhou_sm(at)blackants.com.cn
* \date
*    2014-07-16
* \version
*    v1.0
* Copyright (c) 2010-2014, Blackants Energy Saving Technology co., LTD All rights reserved.
******************************************************************************/

#ifndef _MOTOR_H
#define _MOTOR_H

#ifdef HAVE_MOTOR

#include "ba6289.h"

/*! \brief
*      Load Motor Port Configure
* \param handler[IN]        - board motor port configure handler
*/
void motor_load_config(const MotorConfig *handler);

/*! \brief
*       motor power on
*/
void motor_power_on(void);


/*! \brief
*       motor power off
*/
void motor_power_off(void);


/*! \brief
*       motor initilization
*/
void motor_init(void);

/*! \brief
 *       motor initilization
 */
void motor_init_ipu(void);

/*! \brief
*       motor close
*/
void motor_close(void);

/*! \brief
*       motor run by direction
* \param dir[IN]            - Motor run direction
*        MOTOR_DIR_FORWORD  - Run forword, the state of motor should be closed.
*        MOTOR_DIR_REVERSE  - Run Reverse, the state of motor should be opened.
*/
void motor_run(u8 dir);

/*! \brief
*       motor stop
* \note
*       motor should be stoped when the state of motor is closed or opened max.
*/
void motor_stop(void);

/*! \brief
*       get motor run state
* \return
*       MOTOR_STATE_CLOSED      - motor is closed
*       MOTOR_STATE_OPEND       - motor is opened max
*       MOTOR_STATE_OTHERS      - motor is in other state
*/
u8   motor_get_state(void);

/*! \brief
*       motor ADC POWER ON
* \return
*       
* \note
*       阀门开度ADC采集电源控制，一般与ADC采集公用控制引脚，使用时需要做排他处理
*/
void motor_adc_power_on(void);

/*! \brief
*       motor adc初始化
* \return
*       
* \note
*      初始化阀门ADC.
*/
void motor_adc_init(void);

/*! \brief
*       motor adc释放
* \return
*       
* \note
*      释放阀门ADC.
*/
void  motor_adc_close(void);

/*! \brief
*       motor get position
* \return
*       valve position adc value
* \note
*      获取阀门开度ADC值.
*/
u32 motor_get_position(void);

/*! \brief
*       motor ADC POWER OFF
* \return
*       
* \note
*       阀门开度ADC采集电源控制，一般与ADC采集公用控制引脚，使用时需要做排他处理
*/
void motor_adc_power_off(void);

/*! \brief
*       motor reverse run flag
* \flag[IN]     - run reverse flag
*                       - 0 : no reverse running
*                       - 1 : reverse running
* \note
*       motor should be run reverse if it set to 1.
*/
u8 motor_set_reverse(u8 flag);

#endif

#endif
