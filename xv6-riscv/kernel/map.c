#include "types.h"
#include "param.h"
#include "memlayout.h"
#include "spinlock.h"
#include "riscv.h"
#include "map.h"
#include "defs.h"

void hashmap_init(struct hashmap *hashmap) {
  initlock(&hashmap->lock, "hashmap");
  for (int i = 0; i < HASHMAP_SIZE; i++) {
    hashmap->entries[i] = 0;
  }
}

uint hash(uint key) {
  return key % HASHMAP_SIZE;
}

struct stlb_entry *hashmap_get(struct hashmap *hashmap,uint key) {
  uint index = hash(key);
  struct hashmap_entry *entry = hashmap->entries[index];
  while (entry) {
    if (entry->key == key) {
      return entry->value;
    }
    entry = entry->next;
  }
  return 0;
}

void hashmap_put(struct hashmap *hashmap, uint key, struct stlb_entry *value) {
  uint index = hash(key);
  struct hashmap_entry *entry = hashmap->entries[index];
  while (entry) {
    if (entry->key == key) {
      entry->value = value;
      return;
    }
    entry = entry->next;
  }
  struct hashmap_entry *new_entry = (struct hashmap_entry *)kalloc();
  new_entry->key = key;
  new_entry->value = value;
  new_entry->next = hashmap->entries[index];
  hashmap->entries[index] = new_entry;
}


void test_map() {
  printf("test map start\n");
  struct hashmap hashmap;
  // Initialize the global hashmap
  hashmap_init(&hashmap);
  
  struct stlb_entry *value = NULL;
  printf("putting values\n");
  
  // Check if kalloc() returns a valid pointer before putting values
  struct hashmap_entry *entry1 = (struct hashmap_entry *)kalloc();
  if (entry1 != NULL) {
    entry1->key = 1;
    entry1->value = value;
    hashmap_put(&hashmap,1, value);
  }
  
  struct stlb_entry *value1 = NULL;
  struct hashmap_entry *entry2 = (struct hashmap_entry *)kalloc();
  if (entry2 != NULL) {
    entry2->key = 2;
    entry2->value = value1;
    hashmap_put(&hashmap,2, value1);
  }
  
  printf("getting values\n");
  struct stlb_entry *e1 = hashmap_get(&hashmap,1);
  struct stlb_entry *e2 = hashmap_get(&hashmap,2);
  printf("%lx, %lx\n", (uint64)e1, (uint64)e2);
}