#include "slotted_page.h"
#include <stddef.h>
#include <stdint.h>
#include <string.h>

// --- Slotted Page Implementation ---

uint16_t* page_get_slot_array(Page* page) {
  uint8_t* data_ptr = (uint8_t *)page->data;
  data_ptr += sizeof(PageHeader);
  return (uint16_t *)data_ptr;
}

void* page_get_row_data(Page* page, uint16_t slot_num) {
  PageHeader* header = (PageHeader *)page->data;

  // 1. MUST check bounds before reading from the array!
  if (slot_num >= header->item_count) {
    return NULL;
  }

  uint16_t* data_ptr = page_get_slot_array(page);
  uint16_t row_offset = data_ptr[slot_num];

  // 2. Check for deleted/empty slot
  if (row_offset == 0) {
    return NULL;
  }

  uint8_t* data_ptr2 = (uint8_t *)page->data;
  data_ptr2 += row_offset;
  return (void *)data_ptr2;
}

StorageResult page_insert_row(Page* page, void* row_data, uint32_t row_size) {
  PageHeader* header = (PageHeader *)page->data;
  uint16_t required_space = row_size + sizeof(uint16_t);

  if (header->free_space < required_space) {
    return STORAGE_ERROR;
  }

  header->free_ptr -= row_size;
  uint8_t* data_ptr = (uint8_t *)page->data;
  data_ptr += header->free_ptr;

  memcpy(data_ptr, row_data, row_size);

  uint16_t* data_ptr2 = page_get_slot_array(page);
  data_ptr2[header->item_count] = header->free_ptr;

  header->free_space -= required_space;
  header->item_count++;

  return STORAGE_SUCCESS;
}
