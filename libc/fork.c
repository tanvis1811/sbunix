#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <syscall.h>


pid_t fork(void)
{
        pid_t ret;
        ret =syscall_0(SYS_fork);
        return ret;
}

