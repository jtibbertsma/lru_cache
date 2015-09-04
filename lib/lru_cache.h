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
typedef int (*eqfunc)(void *, void *);

/* Definition for user defined default value calculation function */
typedef void * (*defaultfunc)(void *);

/* lrucache_new
 *
 * Return a pointer to a fresh lru_cache.
 *
 * Aruguments:
 *   hash:
 *     This is a pointer to a hash function. It takes a void * as an argument,
 *     and it should return an unsigned int. This is used to find the hash of
 *     key to the hash table.
 *
 *   equality:
 *     This is a pointer to a function that compares two values and returns a
 *     nonzero int if the two values are to be considered equal. This will be
 *     called on a pair of keys that have the same hash value.
 *
 *   max_size:
 *     The maximum number of items that the cache can hold before it starts
 *     removing items.
 *
 *   default (optional):
 *     This is a function pointer that calculates a default value from a key.
 *     It should return a poiner to a malloced object. It's important that
 *     the return value be malloced because free will be called on it when
 *     the lru_cache removes it. If this argument is NULL, then the cache will
 *     simply return NULL when one tries to fetch an item from the cache that
 *     isn't there.
 */
lru_cache *lrucache_new(hashfunc, eqfunc, uint32_t, defaultfunc);

/* lrucache_get
 * 
 * Given a key, return the corresponding value from the cache. If a default
 * func was provided and the key doesn't exist, use the defualt func to
 * build a new value and insert that into the cache before returning it. If
 * an item is found, it is considered the most recently used item.
 *
 * Arguments:
 *   key:
 *     The key to search for in the hash table.
 */
 void *lrucache_get(void *);

/* lrucache_get_no_default
 * 
 * Same lrucache_get, but don't attempt to build a default value. This is good
 * for testing if a given key is in the cache.
 *
 * Arguments:
 *   key:
 *     The key to search for in the hash table.
 */
 void *lrucache_get_no_default(void *);

/* lrucache_remove
 *
 * Search for a key in the cache, and remove it if it's found. Returns true if
 * an item got deleted
 *
 * Arguments:
 *   key:
 *     The key to search for in the hash table.
 */

#endif
