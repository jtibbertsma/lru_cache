# lru_cache

I want to write an least recently used cache in C. It's been a while since I've written anything in C, and I don't want to get rusty.
This seems like it would be a fun data structure to try and implement.

The idea of an lru cache is that it's a hash table, but it won't grow past a certain size. When it hits the max size, the next insert causes one item to be deleted. The clever part is that the item that gets deleted will be the one that was used the least recently,
hence the name.

## Complexity

Inserts and retrievals are done in O(1) time, and space complexity is also O(1), since the cache can only grow to a certain size.

## Approach

The cache is implemented using a hash table and circular doubly linked list.

## Usage

The keys in the table are void pointers. The user can provide a hash function to hash the keys, so the keys can be anything. If no hash function is given, then a reasonable default is used.