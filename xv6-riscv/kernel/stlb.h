#include "types.h"

const int stlb_capacity = 100;

struct stlb_entry{
	uint64 ppn;
	uint64 vpn;
};

struct stlb{
	struct stlb_entry *stlb_entries;
	int size;
	int clock;
}stlb;

void stlb_init(void);

void stlb_initialize(void);

void add_stlb_entry_add(uint64 ppn, uint64 vpn);