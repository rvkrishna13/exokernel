#include "types.h"
#include "param.h"
#include "memlayout.h"
#include "spinlock.h"
#include "riscv.h"
#include "stlb.h"
#include "defs.h"

// static struct stlb_cache *stlb_cache = NULL;
// struct stlb_slab *stlb_slab_head = NULL;
struct stlb_entry *t = NULL;

struct stlb_slab* stlb_slab_init() {
    struct stlb_slab *slab = (struct stlb_slab*)kalloc();
    if (slab == NULL) {
        // Handle allocation failure
		printf("stlb slab initialisation failed\n");
        return NULL;
    }
    slab->free_count = STLB_SLAB_SIZE;
    slab->free_list = &slab->entries[0];
    slab->next = NULL;
    
    for (int i = 0; i < STLB_SLAB_SIZE - 1; i++) {
        slab->entries[i].next = &slab->entries[i + 1];
    }
    slab->entries[STLB_SLAB_SIZE - 1].next = NULL;
    
    // stlb_slab_head = slab;
	return slab;
}

struct stlb_entry *stlb_slab_alloc(struct stlb_slab *stlb_slab_head) {
    struct stlb_slab *slab = stlb_slab_head;
    
    while (slab != NULL) {
        if (slab->free_count > 0) {
            struct stlb_entry *entry = slab->free_list;
            slab->free_list = entry->next;
            slab->free_count--;
            return entry;
        }
        slab = slab->next;
    }
    
    // No free entries available, allocate a new slab
    struct stlb_slab *new_slab = kalloc();
    if (new_slab == NULL) {
        // Handle allocation failure
        return NULL;
    }
    
    new_slab->free_count = STLB_SLAB_SIZE;
    new_slab->free_list = &new_slab->entries[0];
    new_slab->next = stlb_slab_head;
    
    for (int i = 0; i < STLB_SLAB_SIZE - 1; i++) {
        new_slab->entries[i].next = &new_slab->entries[i + 1];
    }
    new_slab->entries[STLB_SLAB_SIZE - 1].next = NULL;
    
    stlb_slab_head = new_slab;
    
    struct stlb_entry *entry = new_slab->free_list;
    new_slab->free_list = entry->next;
    new_slab->free_count--;
    
    return entry;
}

void stlb_slab_free(struct stlb_entry *entry) {
    struct stlb_slab *slab = slab;
    
    while (slab != NULL) {
        if (entry >= &slab->entries[0] && entry <= &slab->entries[STLB_SLAB_SIZE - 1]) {
            entry->next = slab->free_list;
            slab->free_list = entry;
            slab->free_count++;
            return;
        }
        slab = slab->next;
    }
}

void stlb_init(struct stlb_cache *stlb_cache){
	initlock(&stlb_cache->lock, "stlb_cache");
	stlb_cache->head = NULL;
	stlb_cache->tail = NULL;
	stlb_cache->size = 0;
	// stlb_slab_init();
}

void delete_entry_from_stlb(struct stlb_cache *stlb_cache, uint64 vpn){

	if(stlb_cache==NULL)
		return;
	struct stlb_entry *curr = stlb_cache->head;
	while(curr!=NULL){
		if(curr->vpn == vpn){
			stlb_cache->size = stlb_cache->size-1;
			if(stlb_cache->head==stlb_cache->tail){
				stlb_cache->head = NULL;
				stlb_cache->tail = NULL;
			}else if(stlb_cache->head==curr){
				stlb_cache->head = curr->next;
				stlb_cache->head->prev = NULL;
			}else if(stlb_cache->tail==curr){
				stlb_cache->tail = curr->prev;
				stlb_cache->tail->next = NULL;
			}else{
				curr->prev->next = curr->next;
				curr->next->prev = curr->prev;
			}
		kfree(curr);
			return;
		}
		curr = curr->next;
	}
}

void free_stlb(struct stlb_cache *stlb_cache){
	if(!stlb_cache)
		return;
	struct stlb_entry *curr = stlb_cache->head;
	while(curr!=NULL){
		struct stlb_entry *temp = curr->next;
		kfree(curr);
		curr = temp;
	}
}

struct stlb_entry* stlb_cache_contains(struct stlb_cache *stlb_cache, uint64 vpn, pte_t *pte){	
	if(stlb_cache==NULL)
		return NULL;
	struct stlb_entry *curr = stlb_cache->head;
	int i=0;
	while(curr!=NULL){
		if(curr->vpn==vpn)
		{
			if(pte!=NULL)
				curr->pte = pte;
			if(curr==stlb_cache->head)
				return curr;
			curr->prev->next = curr->next;
			stlb_cache->head->prev = curr;
			curr->next = stlb_cache->head;
			curr->prev = NULL;
			stlb_cache->head = curr;
			
			return curr;
		}
		curr = curr->next;
		i++;
	}
	return NULL;
}

void add_entry_when_full(struct stlb_cache *stlb_cache, uint64 vpn, pte_t *pte){
	stlb_cache->tail->vpn = vpn;
	stlb_cache->tail->pte = pte;
	stlb_cache->size = STLB_CAPACITY;
	if(stlb_cache->head!=stlb_cache->tail){
		struct stlb_entry *c = stlb_cache->tail->prev;
		stlb_cache->tail->prev->next = NULL;
		stlb_cache->tail->next = stlb_cache->head;
		stlb_cache->head->prev = stlb_cache->tail;
		stlb_cache->head = stlb_cache->tail;
		stlb_cache->head->prev = NULL;
		stlb_cache->tail = c;
	}
}

void add_entry(struct stlb_cache *stlb_cache, uint64 vpn, pte_t *pte){
	// struct stlb_entry *stlbe = stlb_slab_alloc(stlb_slab_head);
	// if(vpn==3000)
	// {
	// 	if(t==NULL)
	// 	t = (struct stlb_entry*)kalloc();
	// 	t->vpn = vpn;
	// 	t->pte = pte;
	// }
	struct stlb_entry *stlbe = (struct stlb_entry*)kalloc();
	if (stlbe == NULL) {
		return;
	}
	stlbe->vpn = vpn;
	stlbe->pte = pte;
	if(stlb_cache->head==NULL){
		stlbe->next = NULL;
		stlb_cache->tail = stlbe;
	}else{
		stlbe->next = stlb_cache->head;
		stlb_cache->head->prev = stlbe;
	}
	stlb_cache->size = stlb_cache->size+1;
	stlbe->prev = NULL;
	stlb_cache->head = stlbe;
	return;
}

void add_stlb_entry(struct stlb_cache *stlb_cache, uint64 vpn, pte_t *pte){
	
	struct stlb_entry *curr = stlb_cache_contains(stlb_cache, vpn, pte);
	if(curr!=NULL)
		return;

	if(stlb_cache->head==NULL || (stlb_cache->size < STLB_CAPACITY))
		add_entry(stlb_cache, vpn,pte);
	else
		add_entry_when_full(stlb_cache,vpn,pte);
}

void traverse_stlb(struct stlb_cache *stlb_cache){
	if(!stlb_cache)
		return;
	struct stlb_entry *curr = stlb_cache->head;
	int i=0;
	while(curr!=NULL){
		printf("0x%x, 0x%x\n", curr->vpn, *curr->pte);
		curr = curr->next;
		i++;
	}
}

void test_stlb(){
	struct stlb_cache *stlb_cache = (struct stlb_cache *)kalloc();
	// start_stlb();
	// printf("stlb cache 0x%x\n", stlb_cache);
	
	// stlb_init(stlb_cache);
	printf("start adding entries\n");
	for(int i=1; i<=10; i++){
		printf("adding entry with vpn %d\n",i);
		add_stlb_entry(stlb_cache, i%5,NULL);
	}
	// traverse_stlb(stlb_cache);
}