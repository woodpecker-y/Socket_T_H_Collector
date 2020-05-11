#ifndef _AWU_H
#define _AWU_H

#ifdef HAVE_AWU

/*! \brief
*   awu initilization
*/
void awu_init(void);

/*! \brief
*       enable awu
* \param init[IN]	- wakeup cycle
*
* \notes
*       唤醒周期计算值与CPU频率有关，计算公式为：
*/
void awu_enable(u16 cycle);

/*! \brief
*       disable awu
*/
void awu_disable(void);

#endif

#endif