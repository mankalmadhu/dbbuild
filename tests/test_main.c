#include <stdio.h>

// Forward declarations of all test functions
void test_page_init(void);
void test_db_read_write(void);
void test_row_serialize(void);
void test_page_get_slot_array(void);
void test_page_get_row_data(void);
void test_page_insert_row(void);
void test_table_open_close(void);
void test_table_insert_row(void);

int main(void) {
  printf("--- Running DB Tests ---\n");
  
  // Storage Tests
  test_page_init();
  test_db_read_write();
  
  // Row Tests
  test_row_serialize();
  
  // Slotted Page Tests
  test_page_get_slot_array();
  test_page_get_row_data();
  test_page_insert_row();

  // Table Tests
  test_table_open_close();
  test_table_insert_row();
  
  printf("--- All tests passed! ---\n");
  return 0;
}
