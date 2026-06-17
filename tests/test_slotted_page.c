#include "../src/slotted_page.h"
#include "../src/storage.h"
#include "../src/row.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void test_page_get_slot_array() {
  printf("Running test_page_get_slot_array...\n");
  
  // Allocate raw page memory
  Page page;
  page.data = malloc(PAGE_SIZE);
  assert(page.data != NULL);
  
  page_init(page.data, 1, PAGE_TYPE_TABLE_LEAF);
  
  // Test slot array pointer
  uint16_t* slot_array = page_get_slot_array(&page);
  
  // It should start exactly after PageHeader
  uint8_t* expected_ptr = ((uint8_t *)page.data) + sizeof(PageHeader);
  assert((uint8_t *)slot_array == expected_ptr);
  
  free(page.data);
  printf("test_page_get_slot_array passed!\n");
}

void test_page_get_row_data() {
  printf("Running test_page_get_row_data...\n");
  
  Page page;
  page.data = malloc(PAGE_SIZE);
  assert(page.data != NULL);
  
  page_init(page.data, 1, PAGE_TYPE_TABLE_LEAF);
  PageHeader* header = (PageHeader *)page.data;
  
  // Fake an inserted row
  header->item_count = 1;
  uint16_t* slots = page_get_slot_array(&page);
  slots[0] = 4000; // Fake row at byte offset 4000
  
  // Test valid lookup
  uint8_t* row_ptr = (uint8_t *)page_get_row_data(&page, 0);
  uint8_t* expected_ptr = ((uint8_t *)page.data) + 4000;
  assert(row_ptr == expected_ptr);
  
  // Test out of bounds lookup (should return NULL)
  void* invalid_ptr = page_get_row_data(&page, 99);
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
  
  // 1. Prepare a serialized row
  Row r1 = { .id = 1 };
  strncpy(r1.username, "bob", COLUMN_USERNAME_SIZE);
  strncpy(r1.email, "bob@example.com", COLUMN_EMAIL_SIZE);
  
  uint32_t row_size = FixedLengthRowStrategy.get_row_size(&r1);
  void* buffer = malloc(row_size);
  FixedLengthRowStrategy.serialize(&r1, buffer);
  
  // 2. Insert row 1
  StorageResult res = page_insert_row(&page, buffer, row_size);
  assert(res == STORAGE_SUCCESS);
  
  PageHeader* header = (PageHeader *)page.data;
  assert(header->item_count == 1);
  assert(header->free_ptr == PAGE_SIZE - row_size);
  
  // 3. Insert row 2
  Row r2 = { .id = 2 };
  FixedLengthRowStrategy.serialize(&r2, buffer);
  res = page_insert_row(&page, buffer, row_size);
  assert(res == STORAGE_SUCCESS);
  
  assert(header->item_count == 2);
  assert(header->free_ptr == PAGE_SIZE - (2 * row_size));
  
  // 4. Verify we can get them back
  void* retrieved_data = page_get_row_data(&page, 0);
  Row retrieved_row;
  FixedLengthRowStrategy.deserialize(retrieved_data, &retrieved_row);
  assert(retrieved_row.id == 1);
  
  retrieved_data = page_get_row_data(&page, 1);
  FixedLengthRowStrategy.deserialize(retrieved_data, &retrieved_row);
  assert(retrieved_row.id == 2);
  
  free(buffer);
  free(page.data);
  printf("test_page_insert_row passed!\n");
}
