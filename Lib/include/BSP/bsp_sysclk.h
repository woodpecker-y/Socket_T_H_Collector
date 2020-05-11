#ifndef _BSP_SYSCLK_H
#define _BSP_SYSCLK_H

/*! \brief
*       system clock initilization
* \note
*       default system clock is HSI, and its frequency is 8MHz.
*/
void BSP_SYSCLK_Init(void);

/*! \brief
*       system clock initilization(HSE)
* \param div[IN]                - Frequency division coefficient
*           CLK_SYSCLKDiv_1     - 1 points frequency
*           CLK_SYSCLKDiv_2     - 2 points frequency
*           CLK_SYSCLKDiv_4     - 4 points frequency
*           CLK_SYSCLKDiv_8     - 8 points frequency
*           CLK_SYSCLKDiv_16    - 16 points frequency
*           CLK_SYSCLKDiv_64    - 64 points frequency
*           CLK_SYSCLKDiv_128   - 128 points frequency
*/
void BSP_SYSCLK_Init_HSE(u8 div);

/*! \brief
*       system clock initilization(LSI)
* \param div[IN]                - Frequency division coefficient
*           CLK_SYSCLKDiv_1     - 1 points frequency
*           CLK_SYSCLKDiv_2     - 2 points frequency
*           CLK_SYSCLKDiv_4     - 4 points frequency
*           CLK_SYSCLKDiv_8     - 8 points frequency
*           CLK_SYSCLKDiv_16    - 16 points frequency
*           CLK_SYSCLKDiv_64    - 64 points frequency
*           CLK_SYSCLKDiv_128   - 128 points frequency
*/
void BSP_SYSCLK_Init_LSI(u8 div);

/*! \brief
*       system clock initilization(HSI)
* \param div[IN]                - Frequency division coefficient
*           CLK_SYSCLKDiv_1     - 1 points frequency
*           CLK_SYSCLKDiv_2     - 2 points frequency
*           CLK_SYSCLKDiv_4     - 4 points frequency
*           CLK_SYSCLKDiv_8     - 8 points frequency
*           CLK_SYSCLKDiv_16    - 16 points frequency
*           CLK_SYSCLKDiv_64    - 64 points frequency
*           CLK_SYSCLKDiv_128   - 128 points frequency
*/
void BSP_SYSCLK_Init_HSI(u8 div);

#endif