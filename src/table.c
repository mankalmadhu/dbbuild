#include "table.h"
#include "slotted_page.h"
#include "storage.h"
#include "storage_types.h"
#include <stdlib.h>

Table* db_open_table(const char* filename) {
  BlockStorage* block_storage = block_storage_open(filename);
  if (!block_storage) {
    return NULL;
  }

  Table* table = malloc(sizeof(*table));
  if (!table) {
    block_storage_close(block_storage);
    return NULL;
  }

  table->block_storage = block_storage;

  return table;
}

void db_close_table(Table* table) {
  if (table) {
    if (table->block_storage) {
      block_storage_close(table->block_storage);
    }
    free(table);
  }
}

uint32_t table_get_page_count(Table* table) {
    return block_storage_get_page_count(table->block_storage);
}

Page* table_get_page(Table* table, uint32_t page_num) {
    return block_storage_get_page(table->block_storage, page_num);
}

StorageResult table_insert_row(Table* table, void* row_data,
                               uint32_t row_size) {
  uint32_t num_pages = table_get_page_count(table);
  
  Page* target_page = NULL;
  if (num_pages == 0) {
      target_page = block_storage_append_page(table->block_storage);
  } else {
      target_page = block_storage_get_page(table->block_storage, num_pages - 1);
  }

  if (!target_page) {
    return STORAGE_ERROR;
  }

  SlottedPage sp;
  slotted_page_init(&sp, target_page);
  StorageResult res = slotted_page_insert_row(&sp, row_data, row_size);

  if (res == STORAGE_ERROR) {
    page_free(target_page);
    target_page = block_storage_append_page(table->block_storage);
    
    slotted_page_init(&sp, target_page);
    res = slotted_page_insert_row(&sp, row_data, row_size);
  }

  if (res == STORAGE_SUCCESS) {
    block_storage_write_page(table->block_storage, target_page);
  }
  page_free(target_page);
  return res;
}
