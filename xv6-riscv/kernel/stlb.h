#include "types.h"

static int stlb_capacity = 10;

struct stlb_entry{
	uint vpn;
	pte_t pte;
	struct stlb_entry *next;
	struct stlb_entry *prev;
};

struct stlb_cache{
	struct spinlock lock;
	struct stlb_entry stlbe;
	struct stlb_entry *head;
	struct stlb_entry *tail;
	int size;
};
