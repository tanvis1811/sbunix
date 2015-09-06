#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <syscall.h>


int pipe(int filedes[2])
{
        int ret;
        ret =syscall_1(SYS_pipe,((long)filedes));
        return ret;
}

