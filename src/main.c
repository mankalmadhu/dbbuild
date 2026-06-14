#include <stdio.h>
#include <stdlib.h>
#include "storage.h"

int main(int argc, char *argv[]) {
    (void)argc;
    (void)argv;
    printf("Mini Relational Database\n");
    
    if (storage_init() != STORAGE_SUCCESS) {
        fprintf(stderr, "Failed to initialize storage\n");
        return 1;
    }
    
    Page* page = page_get(0);
    if (page) {
        PageHeader* header = (PageHeader*)page->data;
        printf("Got page 0. Type: %d, Item Count: %d\n", header->type, header->item_count);
        free(page->data);
        free(page);
    }
    
    db_close();
    
    return 0;
}
