#include "table.h"
#include "slotted_page.h"
#include "storage.h"
#include "storage_types.h"
#include <stdlib.h>
#include <sys/stat.h>

Table *db_open_table(const char *filename) {
  // TODO: Step 1
  if (db_open(filename) != STORAGE_SUCCESS) {
    return NULL;
  }

  Table *table = malloc(sizeof(*table));
  if (!table) {
    return NULL;
  }

  struct stat st;
  if (stat(filename, &st) == 0) {
    table->num_pages = st.st_size / PAGE_SIZE;
  } else {
    table->num_pages = 0;
  }

  return table;
}

void db_close_table(Table *table) {
  db_close();
  if (table) {
    free(table);
  }
}

StorageResult table_insert_row(Table *table, void *row_data,
                               uint32_t row_size) {
  // TODO: Step 2
  uint32_t target_page_num = (table->num_pages == 0) ? 0 : table->num_pages - 1;

  Page *target_page = page_get(target_page_num);

  if (!target_page) {
    return STORAGE_ERROR;
  }

  StorageResult res = page_insert_row(target_page, row_data, row_size);

  if (res == STORAGE_ERROR) {
    page_free(target_page);
    target_page_num++;
    target_page = page_get(target_page_num);
    res = page_insert_row(target_page, row_data, row_size);
  }

  if (res == STORAGE_SUCCESS) {
    page_write(target_page);
    if (target_page_num >= table->num_pages) {
      table->num_pages = target_page_num + 1;
    }
  }
  page_free(target_page);
  return res;
}
