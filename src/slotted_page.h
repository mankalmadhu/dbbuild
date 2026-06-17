#ifndef SLOTTED_PAGE_H
#define SLOTTED_PAGE_H

#include <stdint.h>
#include "storage_types.h"

// --- Slotted Page API ---

// 1. Calculate where the slot array starts inside the page
uint16_t* page_get_slot_array(Page* page);

// 2. Get a pointer to the start of a specific row's data in the page
void* page_get_row_data(Page* page, uint16_t slot_num);

// 3. Insert a row into the page. Returns STORAGE_SUCCESS or STORAGE_ERROR if full.
StorageResult page_insert_row(Page* page, void* row_data, uint32_t row_size);

#endif // SLOTTED_PAGE_H
