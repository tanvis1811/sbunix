#include <sys/defs.h>
#include <sys/utilities.h>
#include <sys/sbunix.h>
#include <sys/task.h>
#include <sys/timer.h>
extern void set_co_ordinates(int32_t x,int32_t y);
extern void get_co_ordinates(int32_t *x,int32_t *y);
void init_timer()
{
	int frequency = 100;
	uint16_t divisor = 1193180/frequency;
	outb(0x43, 0x36);
	char l = (char)(divisor & 0xFF);
	char h = (char)(divisor>>8);
	outb(0x40, l);
	outb(0x40, h);
}

void timer_handler(registers_t *r)
{
	
	   
	timer_ticks++;
		
	int x,y,ss=0,mm=0,hh=0;
	get_co_ordinates(&x,&y);
	set_co_ordinates(33,23);
	if(timer_ticks %100 == 0)
	{
		ss=timer_ticks/100;
		if(ss > 59)
		{
			mm = ss/60;
			ss = ss%60;
		}
		if(mm > 59)
		{
			hh = mm/60;
			mm = mm%60;
		}
		printf("        ");
		set_co_ordinates(68,23);
		printf("%d:%d:%d",hh,mm,ss);
	}
	set_co_ordinates(x,y);
	outb(0x20,0x20);
	if(!scheduling)
	{
		if(timer_ticks%20)
		scheduler();
	}
}

