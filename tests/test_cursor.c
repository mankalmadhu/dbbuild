#include "../src/table.h"
#include "../src/row.h"
#include "../src/cursor.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void test_cursor_full_scan() {
    printf("Running test_cursor_full_scan...\n");
    const char* db_file = "test_cursor.db";
    remove(db_file);

    Table* table = db_open_table(db_file);
    assert(table != NULL);

    // 1. Insert 20 rows to force spanning across 2 pages
    Row r;
    uint32_t row_size = FixedLengthRowStrategy.get_row_size(&r);
    void* buffer = malloc(row_size);
    
    for (int i = 0; i < 20; i++) {
        r.id = i;
        sprintf(r.username, "user%d", i);
        sprintf(r.email, "user%d@example.com", i);
        FixedLengthRowStrategy.serialize(&r, buffer);
        StorageResult res = table_insert_row(table, r.id, buffer, row_size);
        assert(res == STORAGE_SUCCESS);
    }

    // 2. Use a cursor to read all 20 rows back in order
    Cursor* cursor = table_start(table);
    assert(cursor != NULL);

    uint32_t rows_read = 0;
    while (!cursor->end_of_table) {
        // Read the current row
        cursor_get(cursor, buffer, row_size);
        Row retrieved_row;
        FixedLengthRowStrategy.deserialize(buffer, &retrieved_row);
        
        // Assert we get the exact data back in perfect order
        assert(retrieved_row.id == rows_read);
        
        // Advance to the next row (or next page!)
        cursor_advance(cursor);
        rows_read++;
    }

    assert(rows_read == 20); // We should have successfully scanned exactly 20 rows!

    cursor_free(cursor);
    free(buffer);
    db_close_table(table);
    remove(db_file);
    printf("test_cursor_full_scan passed!\n");
}
