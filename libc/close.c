#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <syscall.h>


int close(int fd)
{
        int ret;
        ret =syscall_1(SYS_close,fd);
        return ret;
}

