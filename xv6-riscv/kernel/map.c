#include "types.h"
#include "param.h"
#include "memlayout.h"
#include "spinlock.h"
#include "riscv.h"
#include "map.h"
#include "defs.h"

void hashmap_init(struct hashmap hashmap) {
  initlock(&hashmap.lock, "hashmap");
  memset(hashmap.table, 0, sizeof(hashmap.table));
  memset(hashmap.entry_pool, 0, sizeof(hashmap.entry_pool));
  hashmap.entry_pool_index = 0;
}

uint jenkins_hash(uint key) {
  key = (key + 0x7ed55d16) + (key << 12);
  key = (key ^ 0xc761c23c) ^ (key >> 19);
  key = (key + 0x165667b1) + (key << 5);
  key = (key + 0xd3a2646c) ^ (key << 9);
  key = (key + 0xfd7046c5) + (key << 3);
  key = (key ^ 0xb55a4f09) ^ (key >> 16);
  return key;
}

uint hash(uint key) {
  return jenkins_hash(key) % HASHMAP_SIZE;
}

entry *allocate_entry(struct hashmap *hashmap) {
  if (hashmap->entry_pool_index >= ENTRY_POOL_SIZE) {
    // panic("hashmap: entry pool exhausted");
	printf("unable to create entry\n");
  }
  return &hashmap->entry_pool[hashmap->entry_pool_index++];
}

void hashmap_put(struct hashmap hashmap, uint key, struct stlb_entry *value) {
  acquire(&hashmap.lock);
  uint index = hash(key);
  entry *e = hashmap.table[index];
  while (e != 0) {
    if (e->key == key) {
      e->value = value;
      release(&hashmap.lock);
      return;
    }
    e = e->next;
  }
  e = allocate_entry(&hashmap);
  e->key = key;
  e->value = value;
  e->next = hashmap.table[index];
  hashmap.table[index] = e;
  release(&hashmap.lock);
}

struct stlb_entry *hashmap_get(struct hashmap hashmap, uint key) {
  acquire(&hashmap.lock);
  uint index = hash(key);
  entry *e = hashmap.table[index];
  while (e != 0) {
    if (e->key == key) {
      struct stlb_entry *value = e->value;
      release(&hashmap.lock);
      return value;
    }
    e = e->next;
  }
  release(&hashmap.lock);
  return 0;
}

int hashmap_contains(struct hashmap hashmap, uint key) {
  acquire(&hashmap.lock);
  uint index = hash(key);
  entry *e = hashmap.table[index];
  while (e != 0) {
    if (e->key == key) {
      release(&hashmap.lock);
      return 1;
    }
    e = e->next;
  }
  release(&hashmap.lock);
  return 0;
}


void test_map(){
	struct hashmap hashmap;
	struct stlb_entry *value = NULL;
	printf("putting values\n");
	hashmap_put(hashmap,1,value);
	struct stlb_entry *value1 = NULL;
	hashmap_put(hashmap, 2, value1);
	printf("getting values\n");
	struct stlb_entry *e1 = hashmap_get(hashmap, 1);
	struct stlb_entry *e2 = hashmap_get(hashmap, 2);
	printf("%lx, %lx\n", e1,e2);
}