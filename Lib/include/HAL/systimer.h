#ifndef _SYSTIMER_H
#define _SYSTIMER_H

/*! \brief
*       定时器初始化
*/
u8  systimer_init(void);

/*! \brief
*       配置定时器初始值
* \param[IN]id				- 定时器索引号
* \param[IN]val				- 定时器设定参考值
*/
void systimer_config(unsigned char id, unsigned long val);

/*! \brief
*       获取定时器状态，当状态为E_TIMER_OVF表明定时器溢出
* \param[IN]id				- 定时器索引号
* \param[IN]val				- 定时器设定参考值
*/
u8  systimer_get_over_flag(unsigned char id);

/*! \brief
*       清除定时器溢出标志
* \param[IN]id				- 定时器索引号
*/
void  systimer_clr_over_flag(unsigned char id);

/*! \brief
*       中断服务函数（定时器1）
* \param[IN]id				- 定时器索引号
*/
void  systimer_isr(void);

#endif
