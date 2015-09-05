/* lru_cache.c */

#include <string.h>
#include "lru_cache.h"


#define DEFAULT_CAPA 16

/* Linked list node; the main elements of the hash table */
struct node {
  struct node *prev;
  struct node *next;
  void *key;
  void *value;
};

struct _lru_cache {
  uint32_t max;     /* Max size */
  uint32_t size;    /* Number of items in the cache */
  uint32_t capa;    /* Hash table capacity; must be power of 2 */
  uint32_t dcount;  /* Number of dummy slots in the hash table */
  hashfunc hash;
  eqfunc equality;
  defaultfunc calc_default;
  allocfunc allocate;
  freefunc deallocate;
  struct node *root;  /* linked list root */
  struct node *table; /* hash table */
};


/* Default hash func */
/* google djb2 */
static uint32_t
string_hash(char *string)
{
  uint32_t hash = 5381;
  int c;

  while (c = *string++) {
    hash = ((hash << 5) + hash) + c;
  }

  return hash;
}

/* Default string equality function */
static int
string_equality(char *string1, char *string2)
{
  return strcmp(string1, string2) == 0;
}

/* Default poiner equality */
static int
pointer_equality(void *pointer1, void *pointer2)
{
  return pointer1 == pointer2;
}

lru_cache *
lrucache_new(
  uint32_t max_size,
  hashfunc hash,
  eqfunc equality,
  defaultfunc calc_default,
  allocfunc allocate,
  freefunc deallocate)
{
  /* max_size can't be 0 */
  if (max_size == 0) {
    return NULL;
  }

  /* figure out what allocation func to use */
  if (allocate == NULL) {
    allocate = malloc;
  }

  /* allocate main struct */
  lru_cache *self = (lru_cache *)allocate(sizeof lru_cache);
  if (self == NULL) {
    return NULL;
  }
  memset(self, 0, sizeof lru_cache);

  /* set function pointers */
  self->allocate = allocate;
  self->hash = hash;
  if (hash == NULL) {
    self->hash = (hashfunc)string_hash;
  }
  self->equality = equality;
  if (equality == NULL) {
    if (hash == NULL) {
      self->equality = (eqfunc)string_equality;
    } else {
      self->equality = pointer_equality;
    }
  }
  self->calc_default = calc_default;
  self->deallocate = deallocate;
  if (deallocate == NULL) {
    self->deallocate = free;
  }

  /* set integers */
  self->max = max_size;
  self->size = 0;
  self->capa = DEFAULT_CAPA;
  self->dcount = 0;

  /* create circular linked list root; have it point at itself */
  self->root = (struct node *)self->allocate(sizeof struct node);
  if (self->root == NULL) {
    self->deallocate(self);
    return NULL;
  }
  memset(self->root, 0, sizeof struct node);
  self->root->prev = self->root;
  self->root->next = self->root;

  /* create hash table */
  self->table = (struct node *)self->allocate((sizeof struct node) * self->capa);
  if (self->table == NULL) {
    self->deallocate(self->root);
    self->deallocate(self);
    return NULL;
  }
  memset(self->table, 0, (sizeof struct node) * self->capa);

  return self;
}




void *
lrucache_get_no_default(lru_cache *self, void *key)
{
  struct node *node;
  int found;

  if ((found = fetch_node(self, key, &node)) < 0) {
    return NULL;
  }

  if (found) {
    move_node_to_front(self->root, node);
    return node->value;
  } else {
    return NULL;
  }
}

void *
lrucache_get(lru_cache *self, void *key)
{
  struct node *node;
  int found;

  if ((found = fetch_node(self, key, &node)) < 0) {
    return NULL;
  }

  if (found) {
    move_node_to_front(self->root, node);
    return node->value;
  } else {
    /* Try building a default value and inserting that into the cache */
    
  }
}