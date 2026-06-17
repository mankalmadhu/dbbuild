#include "cursor.h"
#include "slotted_page.h"
#include "storage.h"
#include "storage_types.h"
#include <stdlib.h>
#include <string.h>

Cursor* table_start(Table* table) {
  Cursor* cursor = malloc(sizeof(*cursor));
  cursor->table = table;
  cursor->page_num = 0;
  cursor->slot_num = 0;
  cursor->end_of_table = (block_storage_get_page_count(table->block_storage) == 0);
  return cursor;
}

void cursor_get(Cursor* cursor, void* dest_buffer, uint32_t row_size) {
  if (cursor->end_of_table) {
    return;
  }
  Page* page = block_storage_get_page(cursor->table->block_storage, cursor->page_num);
  SlottedPage sp;
  slotted_page_init(&sp, page);
  void* row_data = slotted_page_get_row(&sp, cursor->slot_num);

  if (row_data == NULL) {
    cursor->end_of_table = true;
    page_free(page);
    return;
  }

  memcpy(dest_buffer, row_data, row_size);
  page_free(page);
}

void cursor_advance(Cursor* cursor) {
  if (cursor->end_of_table) {
    return;
  }
  Page* page = block_storage_get_page(cursor->table->block_storage, cursor->page_num);
  SlottedPage sp;
  slotted_page_init(&sp, page);
  cursor->slot_num++;
  if (cursor->slot_num >= slotted_page_get_row_count(&sp)) {
    cursor->page_num++;
    cursor->slot_num = 0;
    if (cursor->page_num >= block_storage_get_page_count(cursor->table->block_storage)) {
      cursor->end_of_table = true;
    }
  }
  page_free(page);
}

void cursor_free(Cursor* cursor) {
  if (cursor) {
    free(cursor);
  }
}
