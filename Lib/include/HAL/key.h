#ifndef _KEY_H
#define _KEY_H

#ifdef HAVE_KEY

#include "bsp.h"

/*! \brief
*      Load Key Port Configure
* \param handler[IN]        - board configure handler
* \param num[IN]            - key count
*/
void key_load_config(PortTypeDef *handler, u8 num);

/*! \brief
*      key initilization
*/
u8 key_init(void);

/*! \brief
*      Get key code by pin number
* \param key_idx[IN]        - key index
* \return key code
*/
u8 key_get(u16 exti_pin);

/*! \brief
*      Verify key code is valid
* \param key[IN]        - key code
* \return
*       TRUE    - valid
*       FALSE   - invalid
*/
u8 key_check_valid(u8 key);

/*! \brief
*      Check whether the key is pressed.
* \param key[IN]        - key code
* \return
*       TRUE    - valid
*       FALSE   - invalid
*/
u8 key_release(u8 key);

#endif

#endif
