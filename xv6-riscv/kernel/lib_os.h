#include "types.h"

#define MAX_PHY_PAGES 20
#define MAX_BUFFER_PAGES 25

struct user_va_pa_map_node{
	uint64 va;
	uint64 pa;
	struct user_va_pa_map_node *next;
};

struct pp_map{
	uint64 pid;
	struct user_va_pa_map_node *map;
	int size;
	uint64 (*func)(void);
};

struct buffer_node{
	uint64 va;
	char *content;
	struct buffer_node *next;
};

void add_user_va_pa(uint64 pid, uint64 va, uint64 pa);

void del_map(uint64 pid);

void add_va_pa_map(uint64 va, uint64 pa);

void try_alloc_page();

void swap_buffer(uint64 va);

uint64 ghi();

int handle_swap(uint64 va);

