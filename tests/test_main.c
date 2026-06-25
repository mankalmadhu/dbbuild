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
void test_cursor_full_scan(void);
void test_cursor_table_find(void);

// Input Reader Tests
void test_new_input_buffer(void);
void test_read_input(void);

// Compiler & Executor Tests
void test_prepare_insert(void);
void test_prepare_select(void);
void test_prepare_select_with_filter(void);
void test_executor_insert_select(void);

// Slotted Page Tests
void test_slotted_page_find_slot(void);
void test_interior_node_binary_search(void);

int main(void) {
  printf("--- Running DB Tests ---\n");
  
  // Compiler & Executor Tests
  test_prepare_insert();
  test_prepare_select();
  test_prepare_select_with_filter();
  test_executor_insert_select();
  
  // Input Reader Tests
  test_new_input_buffer();
  test_read_input();
  
  // Storage Tests
  test_page_init();
  test_db_read_write();
  
  // Row Tests
  test_row_serialize();
  
  // Slotted Page Tests
  test_page_get_slot_array();
  test_page_get_row_data();
  test_page_insert_row();
  test_slotted_page_find_slot();
  test_interior_node_binary_search();

  // Table Tests
  test_table_open_close();
  test_table_insert_row();
  
  // Cursor Tests
  test_cursor_full_scan();
  test_cursor_table_find();
  
  printf("--- All tests passed! ---\n");
  return 0;
}
