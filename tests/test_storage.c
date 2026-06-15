#include "../src/storage.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

void test_page_init() {
  printf("Running test_page_init...\n");

  // Allocate 4KB of raw memory to simulate a page
  void *raw_page = malloc(PAGE_SIZE);
  assert(raw_page != NULL);

  // Initialize it as a table data page with ID 7
  page_init(raw_page, 7, PAGE_TYPE_TABLE_LEAF);

  // Read the header back and verify
  PageHeader *header = (PageHeader *)raw_page;
  assert(header->page_id == 7);
  assert(header->type == PAGE_TYPE_TABLE_LEAF);
  assert(header->item_count == 0);
  assert(header->free_space == PAGE_SIZE - sizeof(PageHeader));
  assert(header->free_ptr == PAGE_SIZE);

  printf("test_page_init passed!\n");
  free(raw_page);
}

void test_db_read_write() {
  printf("Running test_db_read_write...\n");
  const char *db_file = "test.db";
  remove(db_file);

  // 1. Open the DB via storage_init
  StorageResult res = storage_init();
  assert(res == STORAGE_SUCCESS);

  // 2. Get a page (should create it and initialize if it doesn't exist)
  Page* p = page_get(0);
  assert(p != NULL);
  assert(p->data != NULL);

  // 3. Modify header->item_count = 42
  PageHeader *header = (PageHeader *)p->data;
  header->item_count = 42;

  // 4. Write it back
  int w_res = page_write(p);
  assert(w_res == 0);

  // 5. Free it
  page_free(p);

  // 6. Close DB
  db_close();

  // 7. Re-initialize + gets page 0 again
  res = storage_init();
  assert(res == STORAGE_SUCCESS);

  Page* p_reloaded = page_get(0);
  assert(p_reloaded != NULL);
  PageHeader *reloaded_header = (PageHeader *)p_reloaded->data;
  
  // 8. Verify item_count is still 42
  assert(reloaded_header->page_id == 0);
  assert(reloaded_header->type == PAGE_TYPE_TABLE_LEAF);
  assert(reloaded_header->item_count == 42);

  page_free(p_reloaded);

  db_close();
  remove(db_file);
  printf("test_db_read_write passed!\n");
}


