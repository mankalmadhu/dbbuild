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

void test_slotted_page_find_slot() {
  printf("Running test_slotted_page_find_slot...\n");
  
  Page page;
  page.data = malloc(PAGE_SIZE);
  page.page_num = 0;

  SlottedPage sp;
  slotted_page_init(&sp, &page);
  
  // Set up header manually or use an init function if one exists
  sp.header->page_id = 0;
  sp.header->next_page = 0;
  sp.header->type = PAGE_TYPE_TABLE_LEAF;
  sp.header->item_count = 0;
  sp.header->free_space = PAGE_SIZE - sizeof(PageHeader);
  sp.header->free_ptr = PAGE_SIZE;

  Row r1 = { .id = 5 };
  Row r2 = { .id = 1 };
  Row r3 = { .id = 10 };
  
  char buffer[500];
  uint32_t row_size = FixedLengthRowStrategy.get_row_size(&r1);
  
  // Insert keys out of order: 5, 1, 10
  FixedLengthRowStrategy.serialize(&r1, buffer);
  slotted_page_insert_row(&sp, r1.id, buffer, row_size);
  
  FixedLengthRowStrategy.serialize(&r2, buffer);
  slotted_page_insert_row(&sp, r2.id, buffer, row_size);
  
  FixedLengthRowStrategy.serialize(&r3, buffer);
  slotted_page_insert_row(&sp, r3.id, buffer, row_size);
  
  // Array should now be internally sorted: 1, 5, 10
  assert(slotted_page_get_row_count(&sp) == 3);
  
  // Verify exact matches
  assert(slotted_page_has_key(&sp, 1) == true);
  assert(slotted_page_has_key(&sp, 5) == true);
  assert(slotted_page_has_key(&sp, 10) == true);
  
  // Verify missing keys
  assert(slotted_page_has_key(&sp, 0) == false);
  assert(slotted_page_has_key(&sp, 7) == false);
  
  // Verify correct index returned by binary search
  assert(slotted_page_find_insert_index(&sp, 1) == 0);
  assert(slotted_page_find_insert_index(&sp, 5) == 1);
  assert(slotted_page_find_insert_index(&sp, 10) == 2);
  
  // Verify insertion indexes for missing keys
  assert(slotted_page_find_insert_index(&sp, 0) == 0); // belongs before 1
  assert(slotted_page_find_insert_index(&sp, 3) == 1); // belongs between 1 and 5
  assert(slotted_page_find_insert_index(&sp, 7) == 2); // belongs between 5 and 10
  assert(slotted_page_find_insert_index(&sp, 15) == 3); // belongs after 10
  
  free(page.data);
  printf("test_slotted_page_find_slot passed!\n");
}
