#ifndef STORAGE_H
#define STORAGE_H

#include <stdbool.h>
#include <stdint.h>

#include "storage_types.h"

// Initialize a raw chunk of memory as a fresh page

// Core BlockStorage API
BlockStorage *block_storage_open(const char *filename);
void block_storage_close(BlockStorage *block_storage);
Page *block_storage_get_page(BlockStorage *block_storage, uint32_t page_num);
int block_storage_write_page(BlockStorage *block_storage, Page *page);
uint32_t block_storage_get_page_count(BlockStorage *block_storage);

void page_init(void *page_data, uint32_t page_id, PageType type);
void page_free(Page *page);

#endif // STORAGE_H
