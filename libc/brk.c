#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <syscall.h>


int brk(void *end_data_segment)
{
        int ret;
        ret =syscall_1(SYS_brk, ((long)(end_data_segment)) );
        return ret;
}

