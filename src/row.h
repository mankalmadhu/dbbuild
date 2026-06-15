#ifndef ROW_H
#define ROW_H

#include <stdint.h>
#include "storage_types.h"

// 1. Define a hardcoded Fixed-Length Row (for our initial tests)
#define COLUMN_USERNAME_SIZE 32
#define COLUMN_EMAIL_SIZE 255

typedef struct {
  uint32_t id;
  char username[COLUMN_USERNAME_SIZE];
  char email[COLUMN_EMAIL_SIZE];
} Row;

#define ROW_SIZE sizeof(Row)

// 2. Define the Row Format Strategy Interface
typedef struct {
  // Returns the total serialized size of the given row
  uint32_t (*get_row_size)(void *row);
  
  // Serializes a row object into a raw byte buffer (inside a page)
  void (*serialize)(void *source_row, void *destination_buffer);
  
  // Deserializes a raw byte buffer into a row object
  void (*deserialize)(void *source_buffer, void *destination_row);
} RowFormatStrategy;

// 3. Expose a default fixed-length strategy
extern const RowFormatStrategy FixedLengthRowStrategy;

#endif // ROW_H
