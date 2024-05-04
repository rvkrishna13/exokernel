#include "types.h"

#define STLB_SLAB_SIZE 300
#define STLB_CAPACITY 300

struct stlb_entry{
	uint64 vpn;
	pte_t *pte;
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

struct stlb_slab {
    struct stlb_entry entries[STLB_SLAB_SIZE];
    int free_count;
    struct stlb_entry *free_list;
    struct stlb_slab *next;
};