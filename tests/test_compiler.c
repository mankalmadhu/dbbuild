#include "../src/compiler.h"
#include "../src/input_reader.h"
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

  InputBuffer *input = new_input_buffer();
  Statement statement;

  const char *valid_select = "select";
  input->buffer_length = strlen(valid_select);
  input->buffer = (char *)malloc(input->buffer_length + 1);
  strcpy(input->buffer, valid_select);

  PrepareResult res = prepare_statement(input, &statement);
  assert(res == PREPARE_SUCCESS);
  assert(statement.type == STATEMENT_SELECT);
  assert(statement.has_filter == false);

  printf("test_prepare_select passed!\n");
  free(input->buffer);
  free(input);
}

void test_prepare_select_with_filter() {
  printf("Running test_prepare_select_with_filter...\n");

  InputBuffer *input = new_input_buffer();
  Statement statement;

  const char *valid_select = "select 15";
  input->buffer_length = strlen(valid_select);
  input->buffer = (char *)malloc(input->buffer_length + 1);
  strcpy(input->buffer, valid_select);

  PrepareResult res = prepare_statement(input, &statement);
  assert(res == PREPARE_SUCCESS);
  assert(statement.type == STATEMENT_SELECT);
  assert(statement.has_filter == true);
  assert(statement.filter_id == 15);

  printf("test_prepare_select_with_filter passed!\n");
  free(input->buffer);
  free(input);
}
