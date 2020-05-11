#include "error.h"
#include "sysparams.h"

//Ğ´¹ÊÕÏ
void error_set(u16 code)
{
    g_run_params.st |= code;
}

//Çå³ı¹ÊÕÏ
void error_clr(u16 code)
{
    g_run_params.st &= ~code;
}