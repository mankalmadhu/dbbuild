#include <stdio.h>
#include <stdlib.h>
#include "storage.h"

int main(int argc, char* argv[]) {
    (void)argc;
    (void)argv;
    printf("Mini Relational Database\n");
    
    BlockStorage* block_storage = block_storage_open("test.db");
    if (!block_storage) {
        fprintf(stderr, "Failed to initialize storage\n");
        return 1;
    }
    
    Page* page = block_storage_get_page(block_storage, 0);
    if (page) {
        PageHeader* header = (PageHeader*)page->data;
        printf("Got page 0. Type: %d, Item Count: %d\n", header->type, header->item_count);
        page_free(page);
    }
    
    block_storage_close(block_storage);
    
    return 0;
}
