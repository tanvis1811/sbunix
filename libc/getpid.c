#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <syscall.h>


pid_t getpid(void)
{
        pid_t ret;
        ret =syscall_0(SYS_getpid);
        return ret;
}

