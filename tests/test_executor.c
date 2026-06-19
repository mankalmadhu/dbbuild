#include "../src/executor.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void test_executor_insert_select() {
    printf("Running test_executor_insert_select...\n");
    
    const char* db_file = "test_executor.db";
    remove(db_file);
    
    Table* table = db_open_table(db_file);
    assert(table != NULL);
    
    Statement stmt_insert;
    stmt_insert.type = STATEMENT_INSERT;
    stmt_insert.row_to_insert.id = 999;
    strcpy(stmt_insert.row_to_insert.username, "test_user");
    strcpy(stmt_insert.row_to_insert.email, "test@test.com");
    
    ExecuteResult res = execute_statement(&stmt_insert, table);
    assert(res.status == EXECUTE_SUCCESS);
    assert(res.rows_affected == 1);
    
    Statement stmt_select;
    stmt_select.type = STATEMENT_SELECT;
    res = execute_statement(&stmt_select, table);
    assert(res.status == EXECUTE_SUCCESS);
    assert(res.row_count == 1); // We just inserted 1 row!
    
    // Because result_buffer is now an array of Row structs, we can easily assert!
    Row* rows = (Row*)res.result_buffer;
    assert(rows[0].id == 999);
    assert(strcmp(rows[0].username, "test_user") == 0);
    assert(strcmp(rows[0].email, "test@test.com") == 0);
    
    // Clean up dynamic buffer
    if (res.result_buffer) {
        free(res.result_buffer);
    }
    
    db_close_table(table);
    remove(db_file);
    
    printf("test_executor_insert_select passed!\n");
}
