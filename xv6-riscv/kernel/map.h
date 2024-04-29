#include "types.h"

#define HASHMAP_SIZE 512
#define ENTRY_POOL_SIZE 512

struct hashmap_entry {
  uint key;
  struct stlb_entry *value;
  struct hashmap_entry *next;
};

struct hashmap{
  struct spinlock lock;
  struct hashmap_entry *entries[HASHMAP_SIZE];
} ;

void hashmap_init(struct hashmap *hashmap);
void hashmap_put(struct hashmap *hashmap, uint key, struct stlb_entry *value);
struct stlb_entry *hashmap_get(struct hashmap *hashmap, uint key);
int hashmap_contains(struct hashmap *hashmap, uint key);