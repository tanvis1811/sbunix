#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <syscall.h>


int dup(int oldfd)
{
        int ret;
        ret =syscall_1(SYS_dup,oldfd);
        return ret;
}

