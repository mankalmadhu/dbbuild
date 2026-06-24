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
  uint32_t required_space = row_size + sizeof(Slot);
  return sp->header->free_space >= required_space;
}

static Slot *get_slot_array(const SlottedPage *sp) {
  uint8_t *data_ptr = (uint8_t *)sp->raw_page->data;
  data_ptr += sizeof(PageHeader);
  return (Slot *)data_ptr;
}

void *slotted_page_get_row(SlottedPage *sp, uint16_t slot_num) {
  if (slot_num >= sp->header->item_count) {
    return NULL;
  }

  Slot *slot_array = get_slot_array(sp);
  uint16_t row_offset = slot_array[slot_num].offset;

  if (row_offset == 0) {
    return NULL;
  }

  uint8_t *data_ptr = (uint8_t *)sp->raw_page->data;
  data_ptr += row_offset;
  return (void *)data_ptr;
}

static uint16_t find_slot_index(const SlottedPage *sp, uint32_t key,
                                bool *exact_match) {
  Slot *slot_array = get_slot_array(sp);
  int low = 0;
  int high = sp->header->item_count - 1;
  *exact_match = false;

  while (low <= high) {
    int mid = low + (high - low) / 2;
    if (slot_array[mid].key == key) {
      *exact_match = true;
      return mid;
    } else if (slot_array[mid].key < key) {
      low = mid + 1;
    } else {
      high = mid - 1;
    }
  }
  return low; // insertion point
}

bool slotted_page_has_key(const SlottedPage *sp, uint32_t key) {
  bool exact_match;
  find_slot_index(sp, key, &exact_match);
  return exact_match;
}

uint16_t slotted_page_find_insert_index(const SlottedPage *sp, uint32_t key) {
  bool exact_match;
  return find_slot_index(sp, key, &exact_match);
}

StorageResult slotted_page_insert_row(SlottedPage *sp, uint32_t key,
                                      const void *row_data, uint32_t row_size) {
  if (!slotted_page_has_space(sp, row_size)) {
    return STORAGE_ERROR;
  }

  uint32_t required_space = row_size + sizeof(Slot);
  sp->header->free_ptr -= row_size;

  Slot *slot_array = get_slot_array(sp);
  bool exact_match;
  int insert_idx = find_slot_index(sp, key, &exact_match);

  if (exact_match)
    return STORAGE_DUPLICATE_KEY; // optional: no duplicate keys

  if (insert_idx < sp->header->item_count) {
    memmove(&slot_array[insert_idx + 1], &slot_array[insert_idx],
            (sp->header->item_count - insert_idx) * sizeof(Slot));
  }

  slot_array[insert_idx].key = key;
  slot_array[insert_idx].offset = sp->header->free_ptr;

  uint8_t *data_ptr = (uint8_t *)sp->raw_page->data;
  data_ptr += sp->header->free_ptr;
  memcpy(data_ptr, row_data, row_size);

  sp->header->free_space -= required_space;
  sp->header->item_count++;

  return STORAGE_SUCCESS;
}
