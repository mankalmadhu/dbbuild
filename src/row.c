#include "row.h"
#include <stdio.h>
#include <string.h>

static uint32_t fixed_get_row_size(void* row) {
  (void)row;
  return ROW_SIZE;
}

static void fixed_serialize(void* source_row, void* destination_buffer) {

  memcpy(destination_buffer, source_row, ROW_SIZE);
}

static void fixed_deserialize(void* source_buffer, void* destination_row) {
  memcpy(destination_row, source_buffer, ROW_SIZE);
}

// The global instance of our fixed-length strategy
const RowFormatStrategy FixedLengthRowStrategy = {
    .get_row_size = fixed_get_row_size,
    .serialize = fixed_serialize,
    .deserialize = fixed_deserialize};
