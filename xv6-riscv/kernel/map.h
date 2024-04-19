#include "types.h"

#define HASHMAP_SIZE 512
#define ENTRY_POOL_SIZE 512

typedef struct entry {
  uint key;
  struct stlb_entry *value;
  struct entry *next;
} entry;

struct hashmap{
  struct spinlock lock;
  entry *table[HASHMAP_SIZE];
  entry entry_pool[ENTRY_POOL_SIZE];
  int entry_pool_index;
};

void hashmap_init(struct hashmap hashmap);
void hashmap_put(struct hashmap hashmap, uint key, struct stlb_entry *value);
struct stlb_entry *hashmap_get(struct hashmap hashmap, uint key);
int hashmap_contains(struct hashmap hashmap, uint key);