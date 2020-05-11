/* This file has been prepared for Doxygen automatic documentation generation.*/
/*! \file *********************************************************************
*
* \brief
*    board support package-- key driver
* \author
*    zhou_sm(at)blackants.com.cn
* \date
*    2013-01-19
* \version
*    v1.0
* Copyright (c) 2010-2014, Blackants Energy Saving Technology co., LTD All rights reserved.
******************************************************************************/
#ifndef _BSP_KEY_H
#define _BSP_KEY_H

#ifdef HAVE_KEY

/*! \brief Key definition */
typedef enum _KeyTypeDef{
    KEY_1 = '1',            /*!< \brief key code 1 */
    KEY_2 = '2',            /*!< \brief key code 2 */
    KEY_3 = '3',            /*!< \brief key code 3 */
    KEY_4 = '4',            /*!< \brief key code 4 */
    KEY_5 = '5',            /*!< \brief key code 5 */
    KEY_6 = '6',            /*!< \brief key code 6 */
    KEY_COMB_1 = 'a',       /*!< \brief key combination code 1 */
    KEY_COMB_2 = 'b',       /*!< \brief key combination code 2 */
    KEY_COMB_3 = 'c',       /*!< \brief key combination code 3 */
    KEY_COMB_4 = 'd',       /*!< \brief key combination code 4 */
    KEY_COMB_5 = 'e',       /*!< \brief key combination code 5 */
    KEY_IDLE = 0,           /*!< \brief no key code */
}KeyTypeDef;

/*! \brief
*      Load Key Port Configure
* \param handler[IN]        - board configure handler
* \note
*      1.key code is one-to-one correspondence with key configure array index,the first key code is KEY_1.
*      2.Key port confirure array must be ended with "NULL".
*      ex.
*           __far PortTypeDef key_handler[4] = {
*               {PORTA, PIN0},      // Key 1
*               {PORTA, PIN1},      // Key 2
*               {PORTA, PIN2},      // Key 3
*            };
*/
void BSP_Key_LoadConfig(const PortTypeDef *handler, u8 num);

/*! \brief
*      Configure Key Pin Work Mode
* \param mode[IN]        - board configure handler
*      GPIO_Mode_In_PU_IT   - Default
*      GPIO_Mode_In_FL_IT   - 
*       
* \note
*/
void BSP_Key_SetMode(u8 mode);

/*! \brief
*      key initilization
*/
u8 BSP_Key_Init(void);

/*! \brief
*      Get key code by exti pin number
* \param exti_pin[IN]        - exti pin number
* \return key code
*/
u8 BSP_Key_Get(u16 exti_pin);

/*! \brief
*      Verify key code is valid
* \param key[IN]        - key code
* \return
*       TRUE    - valid
*       FALSE   - invalid
*/
u8 BSP_Key_CheckValid(u8 key);

/*! \brief
*      Check whether the key is pressed.
* \param key[IN]        - key code
* \return
*       TRUE    - valid
*       FALSE   - invalid
*/
u8 BSP_Key_Release(u8 key);

#endif

#endif