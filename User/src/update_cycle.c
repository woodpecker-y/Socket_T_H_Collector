#include "update_cycle.h"

/*! \brief
*       根据周期计算下一个时间点
* \param cycle[IN]    - 周期，单位为分钟
* \param offset[IN]   - 时间偏移，单位为秒
* \return
*       返回值是秒单位
*/
//s_report_handler.timer = update_timer(g_sys_params.upload_cycle, 120 + factor%(g_sys_params.upload_cycle/2*60))*UNIT_SECOND;
s32 update_timer(s32 cycle, s32 offset)
{
    s32     timer = 0;
    s32     cycle_remainder = 0;//倒计时
    u32     total_second    = 0;//总秒数
    struct tm t;

    rtc_read(&t);

    //现在时刻的总秒数，从00：00：00开始计算
    total_second = ((u32)t.tm_hour*(u32)60*(u32)60)+((u32)t.tm_min*60)+(u32)t.tm_sec;

    //取余
    cycle_remainder = (s32)cycle*(s32)60 + offset - total_second % ((s32)cycle*(s32)60) ;
    timer = cycle_remainder;

//#if 0
//    printf("offset:%ld\r\n", offset);
//    printf("\r\n[update]Time:%04d-%02d-%02d %02d:%02d:%02d\r\n", t.tm_year+1900, t.tm_mon+1, t.tm_mday, t.tm_hour, t.tm_min, t.tm_sec);
//    printf("cycle=%ld min, timer=%ld s \r\n",cycle, timer);
//#endif

    return timer;
}