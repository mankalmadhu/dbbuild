#include "../src/slotted_page.h"
#include "../src/storage.h"
#include "../src/row.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void test_page_get_slot_array() {
  printf("Running test_page_get_slot_array...\n");
  
  Page page;
  page.data = malloc(PAGE_SIZE);
  assert(page.data != NULL);
  
  page_init(page.data, 1, PAGE_TYPE_TABLE_LEAF);
  
  SlottedPage sp;
  slotted_page_init(&sp, &page);
  
  // Since get_slot_array is static now, we can't test it directly.
  // We will just verify it creates successfully and has 0 rows.
  assert(slotted_page_get_row_count(&sp) == 0);
  
  free(page.data);
  printf("test_page_get_slot_array passed!\n");
}

void test_page_get_row_data() {
  printf("Running test_page_get_row_data...\n");
  
  Page page;
  page.data = malloc(PAGE_SIZE);
  assert(page.data != NULL);
  
  page_init(page.data, 1, PAGE_TYPE_TABLE_LEAF);
  SlottedPage sp;
  slotted_page_init(&sp, &page);
  
  // Test out of bounds lookup (should return NULL)
  void* invalid_ptr = slotted_page_get_row(&sp, 99);
  assert(invalid_ptr == NULL);
  
  free(page.data);
  printf("test_page_get_row_data passed!\n");
}

void test_page_insert_row() {
  printf("Running test_page_insert_row...\n");
  
  Page page;
  page.data = malloc(PAGE_SIZE);
  assert(page.data != NULL);
  
  page_init(page.data, 1, PAGE_TYPE_TABLE_LEAF);
  SlottedPage sp;
  slotted_page_init(&sp, &page);
  
  // 1. Prepare a serialized row
  Row r1 = { .id = 1 };
  strncpy(r1.username, "bob", COLUMN_USERNAME_SIZE);
  strncpy(r1.email, "bob@example.com", COLUMN_EMAIL_SIZE);
  
  uint32_t row_size = FixedLengthRowStrategy.get_row_size(&r1);
  void* buffer = malloc(row_size);
  FixedLengthRowStrategy.serialize(&r1, buffer);
  
  // 2. Insert row 1
  StorageResult res = slotted_page_insert_row(&sp, r1.id, buffer, row_size);
  assert(res == STORAGE_SUCCESS);
  
  assert(slotted_page_get_row_count(&sp) == 1);
  
  // 3. Insert row 2
  Row r2 = { .id = 2 };
  FixedLengthRowStrategy.serialize(&r2, buffer);
  res = slotted_page_insert_row(&sp, r2.id, buffer, row_size);
  assert(res == STORAGE_SUCCESS);
  
  assert(slotted_page_get_row_count(&sp) == 2);
  
  // 4. Verify we can get them back
  void* retrieved_data = slotted_page_get_row(&sp, 0);
  Row retrieved_row;
  FixedLengthRowStrategy.deserialize(retrieved_data, &retrieved_row);
  assert(retrieved_row.id == 1);
  
  retrieved_data = slotted_page_get_row(&sp, 1);
  FixedLengthRowStrategy.deserialize(retrieved_data, &retrieved_row);
  assert(retrieved_row.id == 2);
  
  free(buffer);
  free(page.data);
  printf("test_page_insert_row passed!\n");
}
