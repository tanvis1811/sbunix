#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <syscall.h>


int chdir(const char *buf)
{
        int ret;
        ret =syscall_1(SYS_chdir,((uint64_t)(buf)));
        return ret;
}

