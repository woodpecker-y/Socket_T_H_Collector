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
*      �����ⲿ�ж�ģʽ
*\param mode[IN]        ���ŵĹ���ģʽ
*       GPIO_Mode_In_FL_IT    �����жϵĸ�������(Ĭ��)
*       GPIO_Mode_In_PU_IT    �����жϵ���������
*/
void exti_set_pin_mode(u8 mode);

/*! \brief
*      exti initilization
*\notes
*      ����ģʽĬ��Ϊ�����жϵĸ������룬����ģʽ�����exti_set_pin_mode����
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
