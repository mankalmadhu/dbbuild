#ifndef ROW_H
#define ROW_H

#include "storage_types.h"
#include <stdint.h>

// 1. Define a hardcoded Fixed-Length Row (for our initial tests)
#define COLUMN_USERNAME_SIZE 32
#define COLUMN_EMAIL_SIZE 255

#define ROW_SIZE sizeof(Row)

typedef struct {
  uint32_t (*get_row_size)(void *row);
  void (*serialize)(void *source_row, void *destination_buffer);
  void (*deserialize)(void *source_buffer, void *destination_row);
} RowFormatStrategy;

// 3. Expose a default fixed-length strategy
extern const RowFormatStrategy FixedLengthRowStrategy;

#endif // ROW_H
