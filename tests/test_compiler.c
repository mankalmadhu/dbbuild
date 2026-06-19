#include "../src/compiler.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void test_prepare_insert() {
    printf("Running test_prepare_insert...\n");
    InputBuffer input;
    input.buffer = "insert 123 alice alice@example.com";
    input.buffer_length = strlen(input.buffer);
    input.input_length = input.buffer_length;
    
    Statement stmt;
    PrepareResult res = prepare_statement(&input, &stmt);
    
    assert(res == PREPARE_SUCCESS);
    assert(stmt.type == STATEMENT_INSERT);
    assert(stmt.row_to_insert.id == 123);
    assert(strcmp(stmt.row_to_insert.username, "alice") == 0);
    assert(strcmp(stmt.row_to_insert.email, "alice@example.com") == 0);
    
    printf("test_prepare_insert passed!\n");
}

void test_prepare_select() {
    printf("Running test_prepare_select...\n");
    InputBuffer input;
    input.buffer = "select";
    input.buffer_length = strlen(input.buffer);
    input.input_length = input.buffer_length;
    
    Statement stmt;
    PrepareResult res = prepare_statement(&input, &stmt);
    
    assert(res == PREPARE_SUCCESS);
    assert(stmt.type == STATEMENT_SELECT);
    
    printf("test_prepare_select passed!\n");
}
