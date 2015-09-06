#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <syscall.h>


int dup2(int oldfd, int newfd)
{
        int ret;
        ret =syscall_2(SYS_dup2,oldfd,newfd);
        return ret;
}

