#include "error.h"
#include "sysparams.h"

//д����
void error_set(u16 code)
{
    g_run_params.st |= code;
}

//�������
void error_clr(u16 code)
{
    g_run_params.st &= ~code;
}