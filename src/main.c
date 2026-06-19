#include "compiler.h"
#include "executor.h"
#include "input_reader.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]) {
  (void)argc;
  (void)argv;
  
  Table* table = db_open_table("mydb.db");
  if (!table) {
    printf("Failed to initialize database!\n");
    exit(EXIT_FAILURE);
  }
  
  InputBuffer *input_buffer = new_input_buffer();

  while (true) {
    printf("db > ");
    read_input(input_buffer, stdin);
    // Meta-commands start with a dot
    if (input_buffer->buffer[0] == '.') {
      if (strcmp(input_buffer->buffer, ".exit") == 0) {
        printf("Exiting database...\n");
        db_close_table(table);
        exit(EXIT_SUCCESS);
      } else {
        printf("Unrecognized command '%s'.\n", input_buffer->buffer);
      }
      continue;
    }

    Statement statement;
    switch (prepare_statement(input_buffer, &statement)) {
      case PREPARE_SUCCESS:
        break;
      case PREPARE_SYNTAX_ERROR:
        printf("Syntax error. Could not parse statement.\n");
        continue;
      case PREPARE_UNRECOGNIZED_STATEMENT:
        printf("Unrecognized keyword at start of '%s'.\n", input_buffer->buffer);
        continue;
    }

    switch (execute_statement(&statement, table)) {
      case EXECUTE_SUCCESS:
        if (statement.type == STATEMENT_INSERT) {
          printf("Executed.\n");
        }
        break;
      case EXECUTE_TABLE_FULL:
        printf("Error: Table full.\n");
        break;
    }
  }
}
