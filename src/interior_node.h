#ifndef INTERIOR_NODE_H
#define INTERIOR_NODE_H

#include "storage_types.h"

// Binary searches the interior node and returns the child_page_num to traverse
// down into
uint32_t interior_node_find_child(const SlottedPage *sp, uint32_t key);

// Inserts a new (max_key, child_page_num) pair into the interior node while
// keeping it sorted
StorageResult interior_node_insert(SlottedPage *sp, uint32_t max_key,
                                   uint32_t child_page_num);

#endif
