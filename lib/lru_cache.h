/* lru_cache.h
 *
 * Misc notes:
 *  * When items are removed from the cache, free is called on both the key and
 *    the value. Same goes for when the destructor is called on the cache.
 */

#ifndef JT_LRU_CACHE
#define JT_LRU_CACHE

#include <stdlib.h>

/* Definition of lru_cache */
typedef struct _lru_cache lru_cache;

/* Definition for user defined hash function */
typedef uint32_t (*hashfunc)(void *);

/* Definition for user defined equality function */
typedef int    (*eqfunc)(void *, void *);

/* Definition for user defined default value calculation function */
typedef void * (*defaultfunc)(void *);

/* Definition of a user defined memory allocation function */
typedef void * (*allocfunc)(void *);

/* Definition of a user defined memory deallocation function */
typedef void   (*freefunc)(void *);

/* lrucache_new
 *
 * Return a pointer to a fresh lru_cache.
 *
 * Aruguments:
 *   max_size:
 *     The maximum number of items that the cache can hold before it starts
 *     removing items.
 *
 *   hash (optional):
 *     A pointer to a hash function. It takes a void * as an argument,
 *     and it should return an unsigned int. This is used to find the hash of
 *     key to the hash table. If this is null, then all keys will be treated
 *     as pointers to null-terminated strings, and we'll use a default string
 *     hash function.
 *
 *   equality (optional):
 *     A pointer to a function that compares two values and returns a
 *     nonzero int if the two values are to be considered equal. This will be
 *     called on a pair of keys that have the same hash value. If this is NULL,
 *     and the hash argument is also NULL, then we'll use a default string
 *     comparison function. If this is NULL and hash is non-NULL, we'll use
 *     pointer equality.
 *
 *   default (optional):
 *     A function pointer that calculates a default value from a key.
 *     If this is NULL, then the cache will simply return NULL when
 *     one tries to fetch an item from the cache that isn't there.
 *
 *   allocate (optional):
 *     A function that will be used to allocate memory. If NULL, then we'll
 *     just use malloc.
 *
 *   deallocate (optional):
 *     This is called on any key or values that are removed from the cache,
 *     and also on the cache itself. If NULL, then we'll just use free.
 */
lru_cache *lrucache_new(
  uint32_t max_size,
  hashfunc,
  eqfunc,
  defaultfunc,
  allocfunc,
  freefunc
);

/* lrucache_get
 * 
 * Given a key, return the corresponding value from the cache. If a default
 * func was provided and the key doesn't exist, use the defualt func to
 * build a new value and insert that into the cache before returning it. If
 * an item is found, it is considered the most recently used item.
 */
void *lrucache_get(lru_cache *self, void *key);

/* lrucache_get_no_default
 * 
 * Same as lrucache_get, but don't attempt to build a default value. This is good
 * for testing if a given key is in the cache.
 */
void *lrucache_get_no_default(lru_cache *self, void *key);

/* lrucache_remove
 *
 * Search for a key in the cache, and remove it if it's found. Returns true if
 * an item got deleted.
 *
 */
int lrucache_remove(lru_cache *self, void *key);

/* lrucache_insert
 *
 * Put a value in the cache. If the key is already in the cache, then the
 * memory of the old value will be freed. Returns true if an item gets inserted
 * into the cache. (This would only return false in the case of a memory error.)
 */
int lrucache_insert(lru_cache *self, void *key, void *value);

/* lrucache_insert_no_replace
 *
 * Same as insert, but don't replace an item in the cache. If a key is already in
 * the cashe, this will return false.
 */
int lrucache_insert_no_replace(lru_cache *self, void *key, void *value);

/* lrucache_destroy
 *
 * Frees all memory used by the cache using either free or a user provided function
 */
void lrucache_destroy(lru_cache *self);

#endif
