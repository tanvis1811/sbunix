#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include<syscall.h>


struct spec 
{
       uint64_t tv_sec; 
       long   tv_nsec;  
};

int nanosleep(const struct spec *req, struct spec *rem)
{
        int ret;
        ret =syscall_2(SYS_nanosleep, ((long)(req)),((long)(rem)) );
        return ret;
}

unsigned int sleep(unsigned int seconds)
{


       struct spec *req;
        req=(struct spec*)malloc(sizeof ( struct spec));
        req->tv_sec=seconds;
	req->tv_nsec=0;
        struct spec *rem=(struct spec*)malloc(sizeof (struct spec));

	int t=nanosleep(req,rem);
	return t;
}