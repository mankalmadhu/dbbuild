#include "cursor.h"
#include <stdlib.h>

Cursor *table_start(Table *table) {
  Cursor *cursor = malloc(sizeof(*cursor));
  cursor->table = table;
  cursor->page_num = 0;
  cursor->slot_num = 0;
  cursor->end_of_table = (table_get_page_count(table) == 0);
  return cursor;
}

void cursor_get(Cursor *cursor, void *dest_buffer, uint32_t row_size) {
  if (cursor->end_of_table) {
    return;
  }

  bool success = table_read_row(cursor->table, cursor->page_num,
                                cursor->slot_num, dest_buffer, row_size);
  if (!success) {
    cursor->end_of_table = true;
  }
}

void cursor_advance(Cursor *cursor) {
  if (cursor->end_of_table) {
    return;
  }

  cursor->slot_num++;
  if (cursor->slot_num >=
      table_get_row_count_for_page(cursor->table, cursor->page_num)) {
    cursor->page_num++;
    cursor->slot_num = 0;
    if (cursor->page_num >= table_get_page_count(cursor->table)) {
      cursor->end_of_table = true;
    }
  }
}

void cursor_free(Cursor *cursor) {
  if (cursor) {
    free(cursor);
  }
}

Cursor *table_find(Table *table, uint32_t key) {
  Cursor *cursor = malloc(sizeof(Cursor));
  cursor->table = table;
  cursor->page_num = 0;

  cursor->slot_num = table_find_slot_index(table, cursor->page_num, key);

  // If the index returned is the item_count, we are at the end
  cursor->end_of_table = (cursor->slot_num >= table_get_row_count_for_page(
                                                  table, cursor->page_num));

  return cursor;
}
