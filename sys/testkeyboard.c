#include <sys/defs.h>
#include <sys/utilities.h>
#include <sys/sbunix.h>
#include <sys/task.h>
#include <sys/paging.h>
#define VADDR 0xFFFFFFFF80000000
unsigned char kbdus[256];
unsigned char shift_key=0;
unsigned char control_key=0;
volatile uint64_t count=0;
unsigned char c='\0';
char buf[1024];
volatile int flag=0;
extern void get_co_ordinates(int *x,int *y);
extern void set_co_ordinates(int x,int y);
int counter=0;
int l=0,m=0;
int limit=0;

int kscanf(char *add, int limiter)
{    
	count=0;
	limit=limiter;
	flag=1;

	__asm__ __volatile__("sti"); 
	while(flag);
	memcpy(add,buf,count);
	counter=count;
	count=0;
	set_co_ordinates(l,m);

	return count;
}	


void keyboard_install()
{
	
	int i=0;
	unsigned char temp[256]=
	{
		0,  27, '1', '2', '3', '4', '5', '6', '7', '8', /* 9 */
		'9', '0', '-', '=', '\b', /* Backspace */
		'\t',         /* Tab */
		'q', 'w', 'e', 'r',   /* 19 */
		't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n', /* Enter key */
		0,          /* 29   - Control */
		'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', /* 39 */
		'\'', '`',   0,        /* Left shift */
		'\\', 'z', 'x', 'c', 'v', 'b', 'n',            /* 49 */
		'm', ',', '.', '/',   0,              /* Right shift */
		'*',
		0,  /* Alt */
		' ',  /* Space bar */
		0,  /* Caps lock */
		0,  /* 59 - F1 key ... > */
		0,   0,   0,   0,   0,   0,   0,   0,
		0,  /* < ... F10 */
		0,  /* 69 - Num lock*/
		0,  /* Scroll Lock */
		0,  /* Home key */
		0,  /* Up Arrow */
		0,  /* Page Up */
		'-',
		0,  /* Left Arrow */
		0,
		0,  /* Right Arrow */
		'+',
		0,  /* 79 - End key*/
		0,  /* Down Arrow */
		0,  /* Page Down */
		0,  /* Insert Key */
		0,  /* Delete Key */
		0,   0,   0,
		0,  /* 87 - F11 Key */
		0,  /* 88 - F12 Key */
		0,  /* All other keys are undefined */
		0,  /*90*/
		0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, /* 105*/
		0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, /* 120*/
		0,0,0,0,0,0,0,      /*128*/
		//*****************keymap for shift + ********


		0,  27, '!', '@', '#', '$', '%', '^', '&', '*', /* 9 */
		'(', ')', '_', '+', '\b', /* Backspace */
		'\t',         /* Tab */
		'Q', 'W', 'E', 'R',   /* 19 */
		'T', 'Y', 'U', 'I', 'O', 'P', '{', '}', '\n', /* Enter key */
		0,          /* 29   - Control */
		'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', ':', /* 39 */
		'\"', '~',   0,        /* Left shift */
		'|', 'Z', 'X', 'C', 'V', 'B', 'N',            /* 49 */
		'M', '<', '>', '?',   0,              /* Right shift */
		'*',
		0,  /* Alt */
		' ',  /* Space bar */
		0,  /* Caps lock */
		0,  /* 59 - F1 key ... > */
		0,   0,   0,   0,   0,   0,   0,   0,
		0,  /* < ... F10 */
		0,  /* 69 - Num lock*/
		0,  /* Scroll Lock */
		0,  /* Home key */
		0,  /* Up Arrow */
		0,  /* Page Up */
		'-',
		0,  /* Left Arrow */
		0,
		0,  /* Right Arrow */
		'+',
		0,  /* 79 - End key*/
		0,  /* Down Arrow */
		0,  /* Page Down */
		0,  /* Insert Key */
		0,  /* Delete Key */
		0,   0,   0,
		0,  /* F11 Key */
		0,  /* F12 Key */
		0,  /* All other keys are undefined */
		
	};
	for(i=0;i<256;i++)
	{
		kbdus[i] = temp[i];
	}
}

	

void keyboard_handler(registers_t *r)
{
	unsigned char scancode;
	
	int x=0,y=0;
	
	get_co_ordinates(&x,&y);

	scancode = inb(0x60);

	if (scancode & 0x80)
	{  
		if(scancode == 42)	//shift alt control
		{
			shift_key=0;
		}
	}
	else
	{   
		switch(scancode)
		{
		    
		    case 42:
		    case 54:
		        shift_key = 1;
		    break;

		    case 29:
		        control_key = 1;
		    break;

		    default:
		           
		        if(shift_key==1)
			{
				c = kbdus[scancode+128];
				set_co_ordinates(75,23);
				printf(" ");
				printf("%c",c);
				shift_key=0;
		        }
		        else
			{
				c = kbdus[scancode];
				if(c == '\b')				//backspace
				{
					//get_co_ordinates(&x,&y);
					if(x!=0) 
					{
						set_co_ordinates(75,23);
						printf("\\");
						printf("b");
					}
					if( (x == 0) && (y>=1))
					{
						set_co_ordinates(75,23);
						printf("\b");									
					}
				}
				else if(c == '\t')
				{	       	
					set_co_ordinates(75,23);
					printf("\\");
					printf("t");
				}
				else if(c == '\n')
				{	       		
					set_co_ordinates(75,23);
					printf("\\");
					printf("n");
				}
				else
				{	
					set_co_ordinates(75,23);
					printf(" ");
					printf("%c",c);
				}
		        }
				buf[count++]=c;
				
				 if (c=='\b'&&count>1)
				 {
					 count=count-2;
					buf[count]=32;
					count=count+1;
				}
				if(flag)
				{     
					buf[count]='\0';
					set_co_ordinates(x,y);
					printf("%s",buf);
					if(c=='\b')
					count--;
					get_co_ordinates(&l,&m);
				}
				
				if((c=='\n'||count==limit)&& c!='\b')
				
				 {

					flag=0;
					if (count!=limit)
					count--;
					 
				}
				set_co_ordinates(x,y);			
		}
	}
	
	outb(0x20,0x20);
}

void page_fault_handler()
{
	//while(1)
		
		uint64_t a;
		int pages;
		__asm__ __volatile__ ("movq %%cr2, %0;" :"=r"(a):);

		if (a > 0xFFFFFFFF80020000)
		{
			printf("Cannot access kernel space!\n");
			while(1);
			return;
		}
		if(a)
		{
			struct vma *my_vma = running->mm->vma_list;
			uint64_t *vir = (uint64_t*)(VADDR + running->mm->pml4);
			uint64_t start, end;
			while (my_vma != NULL) 
			{
				start = my_vma->vm_start; 
				end = my_vma->vm_end;
				pages = (end-start) / 4096;
				if( (end-start) % 4096)
					pages = pages + 1;
				
				{
					printf("Mapping pages of vma\n");
					//__asm__ __volatile__ ("movq %0, %%cr3;" :: "r"(running->mm->pml4));
					proc_paging(start,pages,vir);
					//__asm__ __volatile__ ("movq %0, %%cr3;" :: "r"(pml4));
					return;
				}
				my_vma = my_vma->next;
			}
			
		}
		else
		{
			printf("Segmentation fault\n");
			exit();
		}
}
