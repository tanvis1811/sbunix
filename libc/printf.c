#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#define MAXLINE 100
int print_s(char *input)
{
	
	int ps=0;
	while(*input)
	{
		write(1,input,1);
		input++;
		ps++;
	}
	return ps;
}

void print_d(int result,int *printed)
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
	op[b]=('\0');
	char *fin=op;
	while(*fin)
	{
		write(1,fin,1);
		fin++;
		printed++;
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

void print_x(int result,int *printed)
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
	char *fin=op;
	while(*fin)
	{
		write(1,fin,1);
		fin++;
		printed++;
	}
}

void print_c(uint64_t result,int *printed)
{
	
	printed++;			
	char s[5];
		//char *s;
	s[0]=(char)result;
	s[1]='\0';
	//s=(void *)result;
	//char *fin=s;
	*printed=print_s((char*)s);
}

int printf(const char *format, ...)
{    
  
	va_list ap;

	int printed=0;
	char *result_s,s[2];
	int result_d=0,result_c=0,result_x=0;
	va_start(ap, format);
	char neg[2],*fin;
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
					printed=print_s(result_s);
					
					break;

					case('d'):
					result_d=va_arg(ap,int);
					if(result_d<0)
					{
						neg[0]='-';
						fin=neg;
						
					    
						write(1,fin,1);
						printed++;
						result_d=result_d*(-1);
					}
					print_d(result_d,&printed);
					break;

					case('c'):
					result_c=va_arg(ap, int );
					//fin=result_c;
					//write(1,fin,1);
					print_c(result_c,&printed);
					break;

					case('x'):
					result_x=va_arg(ap,int);
					print_x(result_x,&printed);
					break;

					default:
					s[0]='%';
					s[1]=*format;
					s[2]='\0';
					printed=print_s(s);
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
		write(1,format,1);
		format++;
		if(*format!=0)
		{
			printed++;
		}
		else
		{
			break;
		}
	}
	va_end(ap);
	
	return printed;
}
