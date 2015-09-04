# lru_cache

I want to write an least recently used cache in C. It's been a while since I've written anything in C, and I don't want to get rusty.
This seems like it would be a fun data structure to try and implement.

The idea of an lru cache is that it's a hash table, but it won't grow past a certain size. When it hits the max size, the next insert causes one item

## Complexity

Inserts and retrievals are done in O(1) time, and space complexity is also O(1), since the cache can only grow to a certain size.

## Approach

The cache is implemented as hash table. The hash table is implements as a contiguous array of node structs. But the node
structs themselves form a circular doubly linked list. There is also a root node that isn't held in the hash table itself. The previous
node from the root node is the least recently used item, and the node after the root node is the most recently used item.

## Usage

The keys in the table are void pointers. It's up to the user to provide a hash function. More detailed usage information is
in the header file.