#ifndef STORAGE_TYPES_H
#define STORAGE_TYPES_H

#include <stdint.h>
#include <stdbool.h>

// We will use 4KB pages to align with standard OS block sizes.
#define PAGE_SIZE 4096

// Helps us identify what kind of data lives on this page
typedef enum {
  PAGE_TYPE_INVALID = 0,
  PAGE_TYPE_TABLE_LEAF = 1,
  PAGE_TYPE_TABLE_INTERIOR = 2
} PageType;

// The metadata at the very beginning of every 4KB page
typedef struct {
  uint32_t page_id; // The unique number of this page (0, 1, 2...)
  uint32_t
      next_page; // Points to next logical page for leaf traversal or overflow
  uint16_t type; // Uses the PageType enum
  uint16_t item_count; // Number of rows/records currently stored on this page
  uint16_t free_space; // How many bytes of free space are left?
  uint16_t
      free_ptr; // Byte offset to where the next piece of data should be written
} PageHeader;

// Example return codes
typedef enum { STORAGE_SUCCESS = 0, STORAGE_ERROR = 1 } StorageResult;

// Wrapper for a page in memory
typedef struct {
  void *data;
  uint32_t page_num;
} Page;

#endif // STORAGE_TYPES_H
