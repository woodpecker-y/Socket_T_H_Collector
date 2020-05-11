#ifndef _ALARM_H
#define _ALRAM_H

#ifdef HAVE_ALARM

/*! \brief
*   awu initilization
*/
void alarm_load_config(AlarmConfig *alarm_cfg);

/*! \brief
*   awu initilization
*/
void alarm_init(void);

/*! \brief
*       update alarm setting
* \param date_weekday[IN]	- alarm date or weekday
* \param hour[IN]	        - alarm hour
* \param min[IN]	        - alarm min
* \param sec[IN]	        - alarm sec
* \param sec[IN]	        - alarm mask
*
*/
void alarm_update(u8 date_weekday, u8 hour, u8 min, u8 sec, u8 mask);

/*! \brief
*       update alarm enable of disable
* \param sts[IN]	- alarm enable state
*
*/
void alarm_ctrl(u8 sts);

#endif

#endif