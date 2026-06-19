#include "input_reader.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]) {
  (void)argc;
  (void)argv;
  InputBuffer *input_buffer = new_input_buffer();

  while (true) {
    printf("db > ");
    read_input(input_buffer, stdin);
    // Meta-commands start with a dot
    if (input_buffer->buffer[0] == '.') {
      if (strcmp(input_buffer->buffer, ".exit") == 0) {
        printf("Exiting database...\n");
        exit(EXIT_SUCCESS);
      } else {
        printf("Unrecognized command '%s'.\n", input_buffer->buffer);
      }
      continue;
    }

    // SQL compilation will go here!
    printf("Unrecognized keyword at start of '%s'.\n", input_buffer->buffer);
  }
}
