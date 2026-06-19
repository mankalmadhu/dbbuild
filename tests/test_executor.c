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
    assert(res == EXECUTE_SUCCESS);
    
    // We can't easily assert the output of execute_select because it prints to stdout,
    // but we can at least ensure it doesn't crash!
    Statement stmt_select;
    stmt_select.type = STATEMENT_SELECT;
    res = execute_statement(&stmt_select, table);
    assert(res == EXECUTE_SUCCESS);
    
    db_close_table(table);
    remove(db_file);
    
    printf("test_executor_insert_select passed!\n");
}
