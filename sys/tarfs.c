#include <sys/tarfs.h> 	
#include <sys/defs.h>
#include <sys/utilities.h>
#include <sys/memory.h>	
#define VADDR 0xFFFFFFFF80000000
struct files_list* flist_head=NULL;
struct files_list* curr=NULL;
//struct files_list* prevlist=NULL;
uint64_t atoi(char *str)
{
	uint64_t res = 0; // Initialize result

	// Iterate through all characters of input string and update result
	for (int i = 0; str[i] != '\0'; ++i)
	res = res*10 + str[i] - '0';

	// return result.
	return res;
}

void strcpy(char* dest,const char* src)
{
	//char *point= dest;

	//return point;

	while ((*dest++ = *src++) != '\0')          
	;
	//return point;
}

int strcmp(char *s, const char *t)
{
       int i;
       for (i = 0; s[i] == t[i]; i++)
        if (s[i] == '\0')
        return 0;
       return s[i] - t[i];
 }


uint64_t pow(int base,int power)
{
	 int i = 0; 
	uint64_t product = 1;

   	 for(i = 0; i < power; ++i) {
        	product = product * base;
   	 }

    return product;

}


uint64_t octalToDecimal(uint64_t num)
{
	int decimal=0;
	int rem;
	int i=0	;
	while(num!=0)
	{
		rem=num%10;
		num=num/10;
		decimal=decimal+(rem*pow(8,i));
		i++;
	}
	return decimal;

}


void part(char* str,char* a)
{
	//char a[20];
	int i=0;
	while(*(str)!='\0')
	{
		if(*(str)=='/')
		{
			break;
		}
		*(a+i)=*(str)++;
		i++;		
	}
	*(a+i)='\0';
	//return a;

}

int pathlook(char* filename) 
{ 
	struct files_list* f=flist_head; 
	while(f->next!=NULL) 
	{ 
		if(strcmp(filename,f->name)==0) 
			return 1; 
		f=f->next; 
	} 
	//printf("No such Directory\n"); 
	return 0; 
}

void init_tarfs()
{
	uint64_t file_size=0;int filedes=4;
	struct files_list *current = NULL;
	struct files_list *prev = current;
	struct files_list *start_dir = NULL;

	HEAD_USTAR *header = (HEAD_USTAR*) &_binary_tarfs_start;
	
	while(header<(HEAD_USTAR*)&_binary_tarfs_end  )
	{	
		file_size=(uint64_t)octalToDecimal(atoi(header->size));
		if(( strcmp(header->typeflag,"0")==0  || strcmp(header->typeflag,"5")==0))
		{
			current = flist_head;
		
			if(strlen(header->name) == 0)
				break;
			struct files_list* f=(struct files_list*)(allocate_page()+VADDR);
	
	
			memset(f->name,0,256);
			strcpy(f->name,header->name);  //d_name
		       
			char temp[20];
			memset(temp,0,20);
			strcpy(temp,"rootfs/");
			strcat(temp,f->name);
	
			strcpy(f->name,temp);
	
			f->size=file_size;
			f->fd=filedes;  //d_off
			filedes++;
			f->offset=0; 
	
			f->next=NULL;
			f->addr=(void*)(header+1);		
			strcpy(f->typeflag,header->typeflag);
			f->d_off=0;
			f->d_ino=0;

			if(flist_head==NULL)
			{
		
				flist_head = f;
				flist_head->next = NULL; 
				prev=flist_head;
				start_dir=flist_head;			
		
			}
	
		
			else
			{
		
				//while(1);
				while((current->next)!=NULL)
					current = current->next;
				current->next = f;
				prev=current;
				current=current->next;
		
				/**for file offset**/
				char o[25];char n[25];
				char* old=o;char* now=n;
				part(current->name,now);  //check if directory has changed
				part(prev->name,old);
		
				if(strcmp(now,old)==0)
				{
					current->d_off=current->addr-start_dir->addr;
				}
				else{

					start_dir=current;
				}
		
				/***end **/
				current->next = NULL;
		
				//lpoint->next=f;

			}
		}

		if(file_size>0)
		{
			header=header +(file_size / (sizeof(HEAD_USTAR) + 1)) + 2;
			
		}

		else	
		{	
			header=header+1;
		
		}	
	}
	
	struct files_list* f=(struct files_list*)(allocate_page()+VADDR);
	strcpy(f->name,"rootfs/");
	f->fd=3;  //d_off
	f->offset=0; 
	strcpy(f->typeflag,"5"); 
	f->next=flist_head;
	flist_head=f;
	//struct files_list *ptr = flist_head;
}


void* lookup(char* filename)
{
	HEAD_USTAR *header = (HEAD_USTAR*) &_binary_tarfs_start;
	int file_size=0;
	while(header<(HEAD_USTAR*)&_binary_tarfs_end)
	{
		file_size=octalToDecimal(atoi(header->size));
		if(strcmp(filename,header->name)==0)
		{
			return (void*)(header+1);
		}
		if(file_size>0)
		{
			header=header + file_size / (sizeof(HEAD_USTAR) + 1) + 2;
			
		}

		else
			header=header+1;
	
	}
	
	return (void*)0;
}


int open(char *filename)
{
	printf("%s\n",filename);	
	struct files_list* lpoint=flist_head;
	while(lpoint->next!=NULL)
	{	printf("file:%s\n",lpoint->name);
	if(strcmp(filename,lpoint->name)==0)
	{
		printf("Found! Filenameis %s\n",lpoint->name);
		return lpoint->fd;
	}
	lpoint = lpoint->next;
	}
	return -1;
	
}
//struct files_list* prlist;//=NULL;
int ind=0;int flg=0;
char temp[100];
void parser(char* str,char* a)
{
	//char a[20];
	int x=0;
	while(x<=ind)
	{
		if(*(str+x)!=*(temp+x))
		{
			*(a+0)='x';
			*(a+1)='\0';
			
			return;
		}
		x++;
	}
	
	int i=0;
	
	
	i=ind+1;int index=ind;int lastindex=ind;
	while(i<(strlen(str)))
	{	
		if(*(str+i)=='/')
		{
			//lastindex=index;
			index=i;
			break;
		}

		i++;	
	}
	if(index==ind)
	{
		index=strlen(str);
		flg=1;
		
	}
	i=0;
	
	i=lastindex;int j=0;
	while(i<index)
	{
		*(a+j)=*(str+i);
		i++;j++;
	}
	
	*(a+j)='\0';
}

struct dirent
{
	long d_ino;
	uint64_t d_off;
	unsigned short d_reclen;
	char d_name [256];
};

typedef struct DIR
{
	//struct file* fp;
	uint64_t addr;
	//struct dirent c_dirent;
}DIR;



uint64_t kopendir(char *name)
{
	
	struct files_list* h=flist_head;
	while(h)
	{
		h=h->next;
	}
	struct files_list* f=flist_head;
	
	
	
	while(f->next!=NULL)
	{

		if((strcmp(name,f->name)==0)&&(strcmp(f->typeflag,"5")==0)) 
		{
			
			
			curr=f;
			int i=0;
			
			strcpy(temp,f->name);
			while(i<=(strlen(curr->name)))
			{	
				if(*(f->name+i)=='/')
				{
			
					ind=i;
					//break;
				}

				i++;	
			}
			return (uint64_t)(f);
		}
		f=f->next;
	}	
	
	printf("No such Directory\n");
	return 0;
}

//struct files_list* prev=curr;
uint64_t kreaddir(uint64_t addr)
{
	if(curr==NULL || curr->next==NULL)
	{
		
		return 0;
	}
	

	struct files_list* f=curr->next;
	
	
	
	char o[50];
	char* old=o;
	parser(curr->name,old);
	while(f!=NULL)
	{	
		
		char n[50];
		char* now=n;
		parser(f->name,now);  //check if directory has changed
		if(strcmp(now,"x")==0)
		{
			return 0;
		}
		if((strcmp(old,now)!=0)||(flg==1))
		{	
			flg=0;
			struct dirent* d=(struct dirent*)(allocate_page()+VADDR);
			
			d->d_ino=0;
			d->d_off=curr->d_off;
			d->d_reclen=0;
			strcpy(d->d_name,f->name);
			
				curr=f;
				return (uint64_t)d;
			
		}
		f=f->next;
	}
	
	return 0;
	
}


