#include "types.h"
#include "param.h"
#include "memlayout.h"
#include "spinlock.h"
#include "riscv.h"
#include "stlb.h"
#include "defs.h"

static struct stlb_cache stlb_cache;

void stlb_init(){
	printf("intialising stlb\n");
	initlock(&stlb_cache.lock, "stlb_cache");
	stlb_cache.head = NULL;
	stlb_cache.tail = NULL;
	printf("initialised stlb\n");
}

void add_entry_when_full(uint vpn, pte_t pte){
	stlb_cache.tail->vpn = vpn;
	stlb_cache.size = stlb_capacity;
	if(stlb_cache.head!=stlb_cache.tail){
		struct stlb_entry *c = stlb_cache.tail->prev;
		stlb_cache.tail->prev->next = NULL;
		stlb_cache.tail->next = stlb_cache.head;
		stlb_cache.head->prev = stlb_cache.tail;
		stlb_cache.head = stlb_cache.tail;
		stlb_cache.head->prev = NULL;
		stlb_cache.tail = c;
	}
}

void add_entry(uint vpn, pte_t pte){
	struct stlb_entry *stlbe = (struct stlb_entry*)kalloc();
	stlbe->vpn = vpn;
	stlbe->pte = pte;
	if(stlb_cache.head==NULL){
		stlb_cache.size = 1;
		stlbe->next = NULL;
		stlb_cache.tail = stlbe;
	}else{
		stlb_cache.size = stlb_cache.size+1;
		stlbe->next = stlb_cache.head;
		stlb_cache.head->prev = stlbe;
	}
	stlbe->prev = NULL;
	stlb_cache.head = stlbe;
}

void add_stlb_entry(uint64 vpn, pte_t pte){
	acquire(&stlb_cache.lock);
	if(stlb_cache.head==NULL || stlb_cache.size<stlb_capacity)
		add_entry(vpn,pte);
	else
	{
		printf("adding when full\n");
		add_entry_when_full(vpn, pte);
	}
	release(&stlb_cache.lock);
}

void traverse_stlb(){
	struct stlb_entry *curr = stlb_cache.head;
	while(curr!=NULL){
		printf("%d entry\n", curr->vpn);
		curr = curr->next;
	}
}

void test_stlb(){
	for(int i=1; i<100; i++){
		printf("adding entry with vpn %d\n",i);
		add_stlb_entry(i,i);
	}
}