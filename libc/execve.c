#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <syscall.h>


int execve(const char *filename, char *const argv[], char *const envp[])
{
        int ret;
        ret =syscall_3(SYS_execve,((long)(filename)),((long)(argv)),((long)(envp)));
        return ret;
}

