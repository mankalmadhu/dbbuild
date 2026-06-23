#ifndef SLOTTED_PAGE_H
#define SLOTTED_PAGE_H

#include "storage_types.h"
#include <stdint.h>
#include <stdbool.h>
#include <stdint.h>

void slotted_page_init(SlottedPage *sp, Page *page);

uint16_t slotted_page_get_row_count(const SlottedPage *sp);
bool slotted_page_has_space(const SlottedPage *sp, uint32_t row_size);
void *slotted_page_get_row(SlottedPage *sp, uint16_t slot_num);
StorageResult slotted_page_insert_row(SlottedPage *sp, uint32_t key,
                                      const void *row_data, uint32_t row_size);

#endif // SLOTTED_PAGE_H
