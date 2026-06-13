#ifndef STORAGE_H
#define STORAGE_H

#include <stdint.h>
#include <stdbool.h>

// Example return codes
typedef enum {
    STORAGE_SUCCESS = 0,
    STORAGE_ERROR = 1
} StorageResult;

// Initialize the storage subsystem
StorageResult storage_init(void);

#endif // STORAGE_H
