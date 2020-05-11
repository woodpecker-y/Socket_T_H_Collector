#ifndef _TASK_COMM_H
#define _TASK_COMM_H

#include "adf.h"
#include "cj188.h"

typedef struct _CommHandler{
    u8          sts;
    u8*         rcv_ptr;
    u16         rcv_len;
    CJ188Pkg    pkg;
}CommHandler;

typedef enum _COMM_STATE{
    E_COMM_IDLE = 0,
    E_COMM_PARSER,
    E_COMM_DEAL,
    E_COMM_FINISH
}COMM_STATE;

void task_comm_proc(void);

#endif