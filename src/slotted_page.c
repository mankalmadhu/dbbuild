#include "slotted_page.h"
#include <stddef.h>
#include <string.h>

void slotted_page_init(SlottedPage *sp, Page *page) {
  sp->raw_page = page;
  sp->header = (PageHeader *)page->data;
}

uint16_t slotted_page_get_row_count(const SlottedPage *sp) {
  return sp->header->item_count;
}

bool slotted_page_has_space(const SlottedPage *sp, uint32_t row_size) {
  uint32_t required_space = row_size + sizeof(uint16_t);
  return sp->header->free_space >= required_space;
}

static uint16_t *get_slot_array(SlottedPage *sp) {
  uint8_t *data_ptr = (uint8_t *)sp->raw_page->data;
  data_ptr += sizeof(PageHeader);
  return (uint16_t *)data_ptr;
}

void *slotted_page_get_row(SlottedPage *sp, uint16_t slot_num) {
  if (slot_num >= sp->header->item_count) {
    return NULL;
  }

  uint16_t *slot_array = get_slot_array(sp);
  uint16_t row_offset = slot_array[slot_num];

  if (row_offset == 0) {
    return NULL;
  }

  uint8_t *data_ptr = (uint8_t *)sp->raw_page->data;
  data_ptr += row_offset;
  return (void *)data_ptr;
}

StorageResult slotted_page_insert_row(SlottedPage *sp, uint32_t key,
                                      const void *row_data, uint32_t row_size) {
  if (!slotted_page_has_space(sp, row_size)) {
    return STORAGE_ERROR;
  }

  uint32_t required_space = row_size + sizeof(uint16_t);
  sp->header->free_ptr -= row_size;

  uint8_t *data_ptr = (uint8_t *)sp->raw_page->data;
  data_ptr += sp->header->free_ptr;
  memcpy(data_ptr, row_data, row_size);

  uint16_t *slot_array = get_slot_array(sp);
  slot_array[sp->header->item_count] = sp->header->free_ptr;

  sp->header->free_space -= required_space;
  sp->header->item_count++;

  return STORAGE_SUCCESS;
}
