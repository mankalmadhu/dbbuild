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

StorageResult table_insert_row(Table* table, void* row_data,
                               uint32_t row_size) {
  uint32_t num_pages = block_storage_get_page_count(table->block_storage);
  uint32_t target_page_num = (num_pages == 0) ? 0 : num_pages - 1;

  Page* target_page = block_storage_get_page(table->block_storage, target_page_num);

  if (!target_page) {
    return STORAGE_ERROR;
  }

  StorageResult res = page_insert_row(target_page, row_data, row_size);

  if (res == STORAGE_ERROR) {
    page_free(target_page);
    target_page_num++;
    target_page = block_storage_get_page(table->block_storage, target_page_num);
    
    // Explicitly initialize the new page
    page_init(target_page->data, target_page_num, PAGE_TYPE_TABLE_LEAF);
    
    res = page_insert_row(target_page, row_data, row_size);
  }

  if (res == STORAGE_SUCCESS) {
    block_storage_write_page(table->block_storage, target_page);
    if (target_page_num >= table->block_storage->num_pages) {
      table->block_storage->num_pages = target_page_num + 1;
    }
  }
  page_free(target_page);
  return res;
}
