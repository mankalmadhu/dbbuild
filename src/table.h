#ifndef TABLE_H
#define TABLE_H

#include <stdint.h>
#include "storage.h"

// Open the DB and create a Table abstraction
Table* db_open_table(const char* filename);

// Close the DB and free the Table
void db_close_table(Table* table);

// Delegate page access
uint32_t table_get_page_count(Table* table);
Page* table_get_page(Table* table, uint32_t page_num);

// Read row data directly from the table
bool table_read_row(Table* table, uint32_t page_num, uint16_t slot_num, void* dest_buffer, uint32_t row_size);
uint16_t table_get_row_count_for_page(Table* table, uint32_t page_num);

// Insert a row, allocating new pages if necessary
StorageResult table_insert_row(Table* table, void* row_data, uint32_t row_size);

#endif // TABLE_H
