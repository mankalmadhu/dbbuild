#include "../src/table.h"
#include "../src/row.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void test_table_open_close() {
  printf("Running test_table_open_close...\n");
  const char *db_file = "test_table_open.db";
  remove(db_file);

  // 1. Open a brand new table
  Table *table = db_open_table(db_file);
  assert(table != NULL);
  assert(table->num_pages == 0);
  db_close_table(table);

  // 2. Create a fake file with exactly 3 pages of data
  FILE *f = fopen(db_file, "w");
  fseek(f, (PAGE_SIZE * 3) - 1, SEEK_SET);
  fputc('\0', f);
  fclose(f);

  // 3. Open existing file and verify num_pages is calculated correctly
  table = db_open_table(db_file);
  assert(table != NULL);
  assert(table->num_pages == 3);
  db_close_table(table);

  remove(db_file);
  printf("test_table_open_close passed!\n");
}

void test_table_insert_row() {
  printf("Running test_table_insert_row...\n");
  const char *db_file = "test_table.db";
  remove(db_file);

  Table *table = db_open_table(db_file);
  assert(table != NULL);
  assert(table->num_pages == 0);

  Row r = { .id = 1 };
  strncpy(r.username, "charlie", COLUMN_USERNAME_SIZE);
  strncpy(r.email, "charlie@example.com", COLUMN_EMAIL_SIZE);

  uint32_t row_size = FixedLengthRowStrategy.get_row_size(&r);
  void *buffer = malloc(row_size);
  
  // Insert 20 rows. 
  // With 4KB pages and ~292 byte rows, a page fits exactly 13 rows.
  // This loop will force the table to allocate a second page!
  for (int i = 0; i < 20; i++) {
      r.id = i;
      FixedLengthRowStrategy.serialize(&r, buffer);
      StorageResult res = table_insert_row(table, buffer, row_size);
      assert(res == STORAGE_SUCCESS);
  }

  // Verify that a second page was automatically created
  assert(table->num_pages == 2);

  free(buffer);
  db_close_table(table);
  remove(db_file);
  printf("test_table_insert_row passed!\n");
}
