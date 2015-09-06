#include <sys/defs.h>
#include <sys/utilities.h>
#include <sys/sbunix.h>
#include <sys/memory.h>
#define PAGE_SIZE 4096

struct page_list pl[40000]; 
int index = 0;
void init_phy(uint64_t start, uint64_t length, uint64_t physfree, uint64_t physbase)
{
	int page_count = 0;	
	uint64_t page = 0;
	for (page = start; page < (start + length); page = page + PAGE_SIZE)
	{
		if (page < physbase)
		{					/* I am marking Pages below physbase as used*/
			index = page / PAGE_SIZE;
			if (index == 0)
				continue;
			pl[index].addr = page;
			pl[index].flag = 0;
		}
		else if (page > physfree)
		{
			index = page / PAGE_SIZE;
			pl[index].addr = page;
			pl[index].flag = 1;
			page_count++;
		}
	}
	//printf("No. of pages: %d\n",page_count);
}

uint64_t allocate_page()
{
	int i = 1;
	while(1)
	{
		if(pl[i].flag == 1)
		{
			pl[i].flag = 0;
			return pl[i].addr;
		}
		else i++;
	}
}
