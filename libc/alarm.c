#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <syscall.h>


unsigned int alarm(unsigned int seconds)
{
        unsigned int ret;
        ret =syscall_1(SYS_alarm,((long)seconds));
        return ret;
}

