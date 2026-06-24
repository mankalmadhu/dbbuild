#ifndef CURSOR_H
#define CURSOR_H

#include "table.h"
#include <stdbool.h>
#include <stdint.h>

// Create a cursor pointing to the start of the table
Cursor *table_start(Table *table);

// Copy the row data for the current cursor position into the provided buffer
void cursor_get(Cursor *cursor, void *dest_buffer, uint32_t row_size);

// Advance the cursor to the next row
void cursor_advance(Cursor *cursor);

// Clean up cursor memory
void cursor_free(Cursor *cursor);

Cursor *table_find(Table *table, uint32_t key);

#endif // CURSOR_H
