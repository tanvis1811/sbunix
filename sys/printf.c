#include <sys/sbunix.h>
#include <sys/gdt.h>
#include <sys/tarfs.h>
#include <stdarg.h>
#include <sys/defs.h>
#include <sys/utilities.h>

#define START_VADDR 0xFFFFFFFF800B8000UL

unsigned short *textmemptr=(unsigned short *)START_VADDR;

uint64_t vaddr=START_VADDR;
void clear_screen();
uint64_t get_curr_loc()
{
	return vaddr;	
}	

void set_loc(uint64_t addr)
{
	vaddr=addr;
}

void get_co_ordinates(int32_t *x,int32_t *y)
{
	uint64_t curpos = (get_curr_loc() - START_VADDR) / 2;
	*x = curpos % 80;
	*y = curpos / 80;
}

void set_co_ordinates(int32_t x,int32_t y)
{
	vaddr=START_VADDR+ ((y*80) +x) *2;
}

void goto_newline()
{
	int32_t x,y;
	get_co_ordinates(&x,&y);
	y=y+1;
	x=0;
	set_co_ordinates(x,y);
}

void goto_c_return()
{
	int32_t x,y;
	get_co_ordinates(&x,&y);
	y=y;
	x=0;
	set_co_ordinates(x,y);
}

void clear_screen()
{
	int i = 0;
	set_co_ordinates(0,0);
	for(i=0;i < 4000;i++)
	{
		printf(" ");	
	}
	set_co_ordinates(0,0);
}
void scroll(void)
{    
	int32_t x,y;
	get_co_ordinates(&x,&y);
	unsigned blank ,temp;
	int attrib = 0x0F;

	blank = 0x20 | (attrib << 8);

	if(y >= 25)
	{
		temp = y - 25 + 1;
		memcpy (textmemptr, textmemptr + temp * 80, (25 - temp) * 80 * 2);
		memset(textmemptr + (25 - temp) * 80,blank, 80);
		y = 25 - 1;
		set_co_ordinates(x,y);
	}
}
void write_c(const char *string )
{    scroll();
	uint64_t pos;
	int32_t x,y;
	char *my_string;

	pos=get_curr_loc();
	get_co_ordinates(&x,&y);
	if(y>50)
	{
		set_co_ordinates(0,0);		//go to 1st address
	}
	if(*string=='\n')
	{
		goto_newline();
		return;
		
		
	}
	if(*string=='\r')
	{
		goto_c_return();
		return;
	}
	
	my_string=(char*)pos;
	
	*my_string++ = *string;
	*my_string++ = 8;
	set_loc((uint64_t)my_string);
	return;
}

void print_d(int result)
{
	char ip[20],op[20];
	int a,i=0,x,b=0;
	while(result>=10)
	{
		a=result%10;
		ip[i]=(char)(((int)'0')+a);
		i++;
		result=result/10;
	}
	ip[i]=(char)(((int)'0')+result);
	for(x=i;x>=0;x--)
	{
		op[b]=ip[x];
		b++;
	}
	op[b]='\0';
	i=0;
	while(op[i]!=0)
	{	
		write_c(&op[i]);
		i++;
	}
}

char get_hex_value(int a)
{
	char value='0';
	if(a<10)
	{
		value=(char)(((int)'0')+a);
	}
	if(a==10)
	{
		value='a';
	}
	else if(a==11)
	{
		value='b';
	}
	else if(a==12)
	{
		value='c';
	}
	else if(a==13)
	{
		value='d';
	}
	else if(a==14)
	{
		value='e';
	}
	else if(a==15)
	{
		value='f';
	}
	return value;
}

void print_x(int result)
{
	char ip[20],op[20];
	int a,i=0,x,b=2;
	if(result<16)
	{
		op[2]='0';
		b++;
	}
	while(result>=16)
	{
		a=result%16;
		ip[i]=get_hex_value(a);
		i++;
		result=result/16;
	}
	ip[i]=get_hex_value(result);

	for(x=i;x>=0;x--)
	{
		op[0]='0';
		op[1]='x';
		op[b]=ip[x];
		b++;
	}
	op[b]='\0';
	char *fin=op;
	while(*fin!=0)
	{
		write_c(fin);
		fin++;
	}
}

void print_p(uint64_t result)
{
	char ip[20],op[20];
	int a,i=0,x,b=2;
	if(result<16)
	{
		op[2]='0';
		b++;
	}
	while(result>=16)
	{
		a=result%16;
		ip[i]=get_hex_value(a);
		i++;
		result=result/16;
	}
	ip[i]=get_hex_value(result);

	for(x=i;x>=0;x--)
	{
		op[0]='0';
		op[1]='x';
		op[b]=ip[x];
		b++;
	}
	op[b]='\0';
	char *fin=op;
	while(*fin!=0)
	{
		write_c(fin);
		fin++;
	}
}

void print_c(int result)
{
	char s[5];
	s[0]=(char)result;
	//s[1]='\0';
	char *fin=s;
	write_c(fin);
}

void printf(const char *format, ...)
{
	va_list ap;
	char *result_s,s[2];
	int result_d,result_c,result_x;
	char neg[2];
	uint64_t result_p;
	va_start(ap, format);
	while (*format != 0)
	{
			while(1)
		{
			if(*format=='%')
			{
				format++;
				switch(*format)
				{
					case('s'):
					result_s=va_arg(ap,char *);
					while(*result_s!=0)
					{
						write_c(result_s);
						result_s++;
					}					
					break;

					case('d'):
					result_d=va_arg(ap,int);
					if(result_d<0)
					{
						neg[0]='-';
						write_c(neg);
						result_d=result_d*(-1);
					}
					print_d(result_d);
					break;

					case('c'):
					result_c=va_arg(ap, int );
					print_c(result_c);
					break;

					case('x'):
					result_x=va_arg(ap,int);
					print_x(result_x);
					break;

					case('p'):
					result_p=va_arg(ap,uint64_t);
					print_p(result_p);
					break;
					
					default:
					s[0]='%';
					s[1]=*format;
					s[2]='\0';
					write_c(s);
					break;
				}
				format++;
			}
			if(*format==0)
			{
				break;
			}
			else 
			{
				break;
			}
		}
		if(*format==0)
		{
			break;
		}
		write_c(format);
		format++;
		if(*format!=0)
		{
			continue;
		}
		else
		{
			break;
		}
	}
	va_end(ap);
	
	return ;
}

