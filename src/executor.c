#include "executor.h"
#include "row.h"
#include "cursor.h"
#include <stdio.h>
#include <stdlib.h>

static ExecuteResult execute_insert(Statement* statement, Table* table) {
  Row* row_to_insert = &(statement->row_to_insert);
  uint32_t row_size = FixedLengthRowStrategy.get_row_size(row_to_insert);
  void* buffer = malloc(row_size);
  FixedLengthRowStrategy.serialize(row_to_insert, buffer);

  StorageResult res = table_insert_row(table, buffer, row_size);
  free(buffer);

  if (res == STORAGE_SUCCESS) {
      return EXECUTE_SUCCESS;
  } else {
      return EXECUTE_TABLE_FULL;
  }
}

static ExecuteResult execute_select(Statement* statement, Table* table) {
  (void)statement;
  Row r;
  uint32_t row_size = FixedLengthRowStrategy.get_row_size(&r);
  void* buffer = malloc(row_size);
  
  Cursor* cursor = table_start(table);
  while (!cursor->end_of_table) {
      cursor_get(cursor, buffer, row_size);
      FixedLengthRowStrategy.deserialize(buffer, &r);
      printf("(%d, %s, %s)\n", r.id, r.username, r.email);
      cursor_advance(cursor);
  }
  cursor_free(cursor);
  free(buffer);
  
  return EXECUTE_SUCCESS;
}

ExecuteResult execute_statement(Statement* statement, Table* table) {
  switch (statement->type) {
    case STATEMENT_INSERT:
      return execute_insert(statement, table);
    case STATEMENT_SELECT:
      return execute_select(statement, table);
  }
  return EXECUTE_SUCCESS;
}
