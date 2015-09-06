#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

//char tempch[100];
//volatile int charcount=0;

int stringToHex(char *s)
{
    unsigned int result = 0;
    int c;
    while (*s)
    {
        if((*s>='0'&&*s<='9')||(*s>='A'&&*s<='F')||(*s>='a'&&*s<='f'))
        {
           result = result << 4;
           if (c=(*s-'0'),(c>=0 && c <=9))
                result+=c;
           else if (c=(*s-'A'),(c>=0 && c <=5))
                    result+=(c+10);
           else if (c=(*s-'a'),(c>=0 && c <=5))
                    result+=(c+10);
        }
        else break;
       ++s;
   }
   return result;
}

int is_digit (char c)
{
    int n;

    if (c>='0' && c<='9')
        n=1;
    else
        n=0;
	return n;

}

    int is_space( char c)
{
    int n;
    if (c==32)
    n=1;
     else
        n=0;
     return n;
}




 int atoi(char s[])
 { int i, n, sign;

       for (i = 0; is_space(s[i]); i++)  /* skip white space */           ;

            sign = (s[i] == '-') ? -1 : 1;

        if (s[i] == '+' || s[i] == '-')  /* skip sign */

            i++;

       for (n = 0; is_digit(s[i]);i++)

            n = 10 * n + (s[i] - '0');

        return sign * n;

    }
/* void scan_s(char *result_s, int *printed)
{
	while(1)
	{
		read(0,result_s, 1);
		if(*result_s=='\n'||*result_s==' ')
		{
			*result_s='\0';
			break;
		}
		result_s++;
		(*printed)++;
	}
} */


 void scan_s(char *result_s, int *printed)
{
	  //printf("%s",result_s);
	     char* buf= result_s;
		 	
		read(0,buf, 1024);
		//printf("%s",buf);
		while(*buf!='\n'&& *buf!=32&&*buf!='\0')
			   buf++;
			   *buf='\0';
		   //	printf("reachewd%s",result_s);
		      
		
		
		//printf("%s",result_s);
		//printf("insideloop");
		/* if(*result_s=='\n'||*result_s==' ')
		{
			*result_s='\0';
			break;
		}
		result_s++;
		*result_s='\0'; */
		(*printed)++;
	
}


/* 
void scan_d(int *result_k, int *printed)
{
	char temp[100];
		int size = read(0,temp,100);
		*printed=size-1;

			temp[size]='\0';
			*result_k = atoi(temp);

} */
 
void scan_d(int *result_k, int *printed)
{
	char temp[100];
		//int size = read(0,temp,100);
		read(0,temp,100);
		/* *printed=size-1;

			temp[size]='\0'; */
			*result_k = atoi(temp);

} 

void scan_x(unsigned int *result_s, int *printed)
{
	char temp[100];
	int size;

size=read(0,temp,200);
		//if(*result_s=='\n'||*result_s==' ')
		temp[size]='\0';
		*result_s=stringToHex(temp);

		//result_s++;
		(*printed)++;
	}
void scan_l( char *result_l,int *printed)

  {
	read(0,result_l, 1024);
  
  (*printed)++;	  
	  
  }


  
      
void scan_c(char *result_s, int *printed)
{// char tempch[100];
    /*   if (charcount==0)
	read(0,tempch, 100);
printf("%s",tempch[charcount]);

	*result_s=tempch[charcount];
	
	  //printf("%c",*result_s);
	  while(1);
	    charcount++;
		if (*result_s=='\n')
		charcount=0;  */
	    
	
char tempch[100];
  read(0,tempch,1024);
  *result_s=tempch[0];
  (*printed)++;
}

int scanf( const char *format, ...)
{
	va_list val;
	int printed = 0;
	unsigned int* result_x;
	 char *result_c;
	int *result_d;
	char *result_s;
	char *result_l;
	va_start(val, format);

 
	while(*format)
	{
		if(*format=='%')
		{
			format++;
			switch(*format)
			{
				case('s'):
				format++;
				result_s=va_arg(val,char *);
				//printf("\nstringvaluekk%s   ",result_s);
				scan_s(result_s,&printed);
				break;

				case('d'):
				format++;
				result_d=va_arg(val,int *);
				//printf("%d" ,*result_s);
				scan_d(result_d,&printed);

				break;

				case('x'):
				format++;
				result_x=va_arg(val,unsigned int *);
				scan_x(result_x,&printed);
				break;

				case('c'):
				format++;
				result_c=va_arg(val,char *);
				scan_c(result_c,&printed);
				break;
				case('l'):
				format++;
				result_l=va_arg(val,char *);
				scan_l(result_l,&printed);
				break;

				default:
				//printf("Error: No such format\n");
				break;
			}
		}
		break;
	}
	return printed;
}

