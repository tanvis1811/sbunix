#include <sys/utilities.h>

struct page_list
{
	uint64_t addr;
	int flag;
};

void init_phy(uint64_t start, uint64_t length, uint64_t pf, uint64_t pb);
uint64_t allocate_page();