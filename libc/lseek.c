#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <syscall.h>


off_t lseek(int fildes, off_t offset, int whence)
{
        off_t ret;
        ret =syscall_3(SYS_lseek,fildes,offset,whence);
        return ret;
}

