#include "../src/row.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void test_row_serialize() {
  printf("Running test_row_serialize...\n");
  
  Row original_row;
  original_row.id = 1;
  strncpy(original_row.username, "alice", COLUMN_USERNAME_SIZE);
  strncpy(original_row.email, "alice@example.com", COLUMN_EMAIL_SIZE);

  // 1. Get the row size
  uint32_t size = FixedLengthRowStrategy.get_row_size(&original_row);
  assert(size == ROW_SIZE);

  // 2. Allocate a buffer to simulate the space in a page
  void *buffer = malloc(size);
  assert(buffer != NULL);

  // 3. Serialize the row into the buffer
  FixedLengthRowStrategy.serialize(&original_row, buffer);

  // 4. Create a new row and deserialize from the buffer
  Row deserialized_row;
  FixedLengthRowStrategy.deserialize(buffer, &deserialized_row);

  // 5. Verify the data matches exactly
  assert(deserialized_row.id == 1);
  assert(strncmp(deserialized_row.username, "alice", COLUMN_USERNAME_SIZE) == 0);
  assert(strncmp(deserialized_row.email, "alice@example.com", COLUMN_EMAIL_SIZE) == 0);

  free(buffer);
  printf("test_row_serialize passed!\n");
}
