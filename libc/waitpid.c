#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <syscall.h>


pid_t waitpid(pid_t pid, int *status, int options)
{
        pid_t ret;
        ret =syscall_3(SYS_wait4,((long)(pid)),((long)(status)),((long)(options)));
        return ret;
}

