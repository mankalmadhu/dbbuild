#ifndef STORAGE_H
#define STORAGE_H

#include <stdbool.h>
#include <stdint.h>

#include "storage_types.h"

// Initialize the storage subsystem
StorageResult storage_init(void);

// Initialize a raw chunk of memory as a fresh page
void page_init(void *page_data, uint32_t page_id, PageType type);

// Core Pager API
int db_open(const char *filename);
void db_close(void);
Page* page_get(uint32_t page_num);
int page_write(Page* page);

#endif // STORAGE_H
