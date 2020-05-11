#ifndef _EXTI_H
#define _EXTI_H

#ifdef HAVE_EXTI

#include "bsp.h"
#include "exti.h"

/*! \brief
*      Load Key Port Configure
* \param handler[IN]        - board configure handler
* \param cnt[IN]            - extern interrupt count
* \note
*      ex.
*           PortTypeDef exti_handler[4] = {
*               {PORTA, PIN0},      // EXTI 1
*               {PORTA, PIN1},      // EXTI 2
*               {PORTA, PIN2},      // EXTI 3
*            };
*/
void exti_load_config(EXTIConfig *handler, u8 cnt);

/*! \brief
*      设置外部中断模式
*\param mode[IN]        引脚的工作模式
*       GPIO_Mode_In_FL_IT    带有中断的浮空输入(默认)
*       GPIO_Mode_In_PU_IT    带有中断的上拉输入
*/
void exti_set_pin_mode(u8 mode);

/*! \brief
*      exti initilization
*\notes
*      引脚模式默认为带有中断的浮空输入，其它模式请调用exti_set_pin_mode配置
*/
void exti_init(void);

void exti_init_by_index(u8 idx);

void exti_close_by_index(u8 idx);

/*! \brief
*      exti initilization
*/
void exti_init_single(u8 port, u8 pin, uint8_t mode);

/*! \brief
*      extern interrupt pin state
* \param idx[IN]        extern interrupt pin array index
* \return
*      0    - low level
*      1    - high level
*/
u8 exti_read_state(u8 idx);

u8 exti_read_state_single(u8 port, u8 pin);


#endif

#endif
