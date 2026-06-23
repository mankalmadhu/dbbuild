#include "executor.h"
#include "cursor.h"
#include "row.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static ExecuteResult execute_insert(Statement *statement, Table *table) {
  ExecuteResult result;
  result.type = STATEMENT_INSERT;
  result.rows_affected = 0;
  result.result_buffer = NULL;
  result.row_count = 0;

  Row *row_to_insert = &(statement->row_to_insert);
  uint32_t row_size = FixedLengthRowStrategy.get_row_size(row_to_insert);
  void *buffer = malloc(row_size);
  FixedLengthRowStrategy.serialize(row_to_insert, buffer);

  StorageResult res =
      table_insert_row(table, row_to_insert->id, buffer, row_size);
  free(buffer);

  if (res == STORAGE_SUCCESS) {
    result.status = EXECUTE_SUCCESS;
    result.rows_affected = 1;
  } else {
    result.status = EXECUTE_TABLE_FULL;
  }
  return result;
}

static ExecuteResult execute_select(Statement *statement, Table *table) {
  ExecuteResult result;
  result.type = STATEMENT_SELECT;
  result.rows_affected = 0;
  result.row_count = 0;
  result.result_buffer = NULL;

  (void)statement;
  Row r; // Dummy to get size for temp buffer
  uint32_t row_size = FixedLengthRowStrategy.get_row_size(&r);

  uint32_t capacity = 10;
  result.result_buffer = malloc(capacity * sizeof(Row));

  void *temp_buffer = malloc(row_size);

  Cursor *cursor = table_start(table);
  while (!cursor->end_of_table) {
    cursor_get(cursor, temp_buffer, row_size);

    if (result.row_count >= capacity) {
      capacity *= 2;
      result.result_buffer =
          realloc(result.result_buffer, capacity * sizeof(Row));
    }

    Row *rows = (Row *)result.result_buffer;
    FixedLengthRowStrategy.deserialize(temp_buffer, &rows[result.row_count]);

    result.row_count++;
    cursor_advance(cursor);
  }
  cursor_free(cursor);
  free(temp_buffer);

  result.status = EXECUTE_SUCCESS;
  return result;
}

ExecuteResult execute_statement(Statement *statement, Table *table) {
  switch (statement->type) {
  case STATEMENT_INSERT:
    return execute_insert(statement, table);
  case STATEMENT_SELECT:
    return execute_select(statement, table);
  }

  ExecuteResult err;
  err.status = EXECUTE_ERROR;
  return err;
}
