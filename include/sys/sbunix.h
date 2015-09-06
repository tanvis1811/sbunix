#ifndef __SBUNIX_H
#define __SBUNIX_H

#include <sys/defs.h>
#include  <sys/idt.h>


//void irq_install_handler(int irq, void (*handler)( regs *r));
void irq_install();

 

void printf(const char *fmt, ...);


 //void cls();
 //void putch(unsigned char c);
 //void puts(unsigned char *str);
 //void settextcolor(unsigned char forecolor, unsigned char backcolor);
 //void init_video();

#endif
