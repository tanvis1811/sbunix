#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <syscall.h>


int open(const char *pathname, int flags)
{
        int ret;
        ret =syscall_2(SYS_open,((long)pathname),flags);
        return ret;
}

