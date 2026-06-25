#include "interior_node.h"
#include "storage_types.h"
#include <stdlib.h>

static InteriorSlot *get_interior_slot_array(const SlottedPage *sp) {
  return (InteriorSlot *)((uint8_t *)sp->raw_page->data + sizeof(PageHeader));
}

uint32_t interior_node_find_child(const SlottedPage *sp, uint32_t key) {
  InteriorSlot *array = get_interior_slot_array(sp);

  int low = 0;
  int high = sp->header->item_count - 1;
  int result = -1;

  while (low <= high) {
    int mid = low + (high - low) / 2;
    if (array[mid].max_key >= key) {
      result = mid;
      high = mid - 1;
    } else {
      low = mid + 1;
    }
  }

  if (result != -1) {
    return array[result].child_page_num;
  }
  return 0; // fallback if not found
}

StorageResult interior_node_insert(SlottedPage *sp, uint32_t max_key,
                                   uint32_t child_page_num) {
  InteriorSlot *array = get_interior_slot_array(sp);

  int insert_pos = 0;
  while (insert_pos < sp->header->item_count &&
         array[insert_pos].max_key < max_key) {
    insert_pos++;
  }

  if (insert_pos < sp->header->item_count &&
      array[insert_pos].max_key == max_key) {
    return STORAGE_DUPLICATE_KEY;
  }

  if (sp->header->free_space < sizeof(InteriorSlot)) {
    return STORAGE_ERROR; // Or a new error type: STORAGE_PAGE_FULL
  }

  for (int i = sp->header->item_count; i > insert_pos; i--) {
    array[i] = array[i - 1];
  }

  array[insert_pos].max_key = max_key;
  array[insert_pos].child_page_num = child_page_num;

  // Update the header
  sp->header->item_count++;
  sp->header->free_space -= sizeof(InteriorSlot);
  sp->header->free_ptr += sizeof(InteriorSlot);

  return STORAGE_SUCCESS;
}
