#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <syscall.h>

struct timespec
{
        unsigned int secs;
        long nanosecs;

};

int nanosleep(const struct timespec *req, struct timespec *rem)

{
        uint64_t ret;
        ret =syscall_2(SYS_nanosleep, ((long)(req)),((long)(rem)) );
        return ret;
}

