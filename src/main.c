#include <stdio.h>
#include <stdlib.h>
#include "storage.h"

int main(int argc, char *argv[]) {
    printf("Mini Relational Database\n");
    
    if (storage_init() != STORAGE_SUCCESS) {
        fprintf(stderr, "Failed to initialize storage\n");
        return 1;
    }
    
    return 0;
}
