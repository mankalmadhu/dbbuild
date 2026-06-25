#include "../src/interior_node.h"
#include "../src/slotted_page.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void test_interior_node_binary_search() {
  printf("Running test_interior_node_binary_search...\n");

  Page page;
  page.data = malloc(PAGE_SIZE);
  memset(page.data, 0, PAGE_SIZE); // Initialize all to 0
  page.page_num = 0;

  SlottedPage sp;
  slotted_page_init(&sp, &page);
  sp.header->type = PAGE_TYPE_TABLE_INTERIOR;
  sp.header->free_space = PAGE_SIZE - sizeof(PageHeader);
  sp.header->free_ptr = PAGE_SIZE;

  StorageResult res;
  
  // We will insert 3 children:
  // Page 1 covers keys <= 10
  // Page 2 covers keys <= 20
  // Page 3 covers anything else (UINT32_MAX)
  
  res = interior_node_insert(&sp, 10, 1);
  assert(res == STORAGE_SUCCESS);
  
  res = interior_node_insert(&sp, 20, 2);
  assert(res == STORAGE_SUCCESS);
  
  // UINT32_MAX is 4294967295
  res = interior_node_insert(&sp, 4294967295, 3);
  assert(res == STORAGE_SUCCESS);
  
  // Now we test searching!
  
  // Searching for key=5 should route to Page 1
  assert(interior_node_find_child(&sp, 5) == 1);
  
  // Searching for key=10 should route to Page 1
  assert(interior_node_find_child(&sp, 10) == 1);
  
  // Searching for key=15 should route to Page 2
  assert(interior_node_find_child(&sp, 15) == 2);
  
  // Searching for key=20 should route to Page 2
  assert(interior_node_find_child(&sp, 20) == 2);
  
  // Searching for key=25 should route to Page 3
  assert(interior_node_find_child(&sp, 25) == 3);

  // Searching for key=9999 should route to Page 3
  assert(interior_node_find_child(&sp, 9999) == 3);
  
  free(page.data);
  printf("test_interior_node_binary_search passed!\n");
}
