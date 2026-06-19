#include "../src/input_reader.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void test_new_input_buffer() {
    printf("Running test_new_input_buffer...\n");
    InputBuffer* input = new_input_buffer();
    assert(input != NULL);
    assert(input->buffer == NULL);
    assert(input->buffer_length == 0);
    assert(input->input_length == 0);
    free(input);
    printf("test_new_input_buffer passed!\n");
}

void test_read_input() {
    printf("Running test_read_input...\n");
    
    // 1. Create a dummy file to act as our "user typing"
    FILE* mock_input = fopen("test_mock_typing.txt", "w+");
    fputs("insert 1 bob bob@test.com\n", mock_input);
    rewind(mock_input); // Go back to the beginning of the file to read it
    
    // 2. Test the function
    InputBuffer* input = new_input_buffer();
    read_input(input, mock_input);
    
    // 3. Verify it worked perfectly!
    assert(input->input_length == 25);
    assert(strcmp(input->buffer, "insert 1 bob bob@test.com") == 0);
    
    // 4. Cleanup
    free(input->buffer);
    free(input);
    fclose(mock_input);
    remove("test_mock_typing.txt");
    
    printf("test_read_input passed!\n");
}
