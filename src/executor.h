#ifndef EXECUTOR_H
#define EXECUTOR_H

#include "storage_types.h"
#include "table.h"

typedef enum { 
  EXECUTE_SUCCESS, 
  EXECUTE_TABLE_FULL,
  EXECUTE_ERROR
} ExecuteStatus;

typedef struct {
  ExecuteStatus status;
  StatementType type;
  
  uint32_t rows_affected;
  
  void* result_buffer;
  uint32_t row_count;
} ExecuteResult;

ExecuteResult execute_statement(Statement* statement, Table* table);

#endif // EXECUTOR_H
