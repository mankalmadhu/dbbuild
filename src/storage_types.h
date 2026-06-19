#ifndef STORAGE_TYPES_H
#define STORAGE_TYPES_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <sys/types.h>

#define PAGE_SIZE 4096
#define COLUMN_USERNAME_SIZE 32
#define COLUMN_EMAIL_SIZE 255

typedef enum {
  PAGE_TYPE_INVALID = 0,
  PAGE_TYPE_TABLE_LEAF = 1,
  PAGE_TYPE_TABLE_INTERIOR = 2
} PageType;

typedef enum { STORAGE_SUCCESS = 0, STORAGE_ERROR = 1 } StorageResult;

typedef struct {
  int file_descriptor;
  uint32_t file_length;
  uint32_t num_pages;
} BlockStorage;

typedef struct {
  void *data;
  uint32_t page_num;
} Page;

typedef struct {
  uint32_t page_id;
  uint32_t next_page;
  uint16_t type;
  uint16_t item_count;
  uint16_t free_space;
  uint16_t free_ptr;
} PageHeader;

typedef struct {
  Page *raw_page;
  PageHeader *header;
} SlottedPage;

typedef struct {
  BlockStorage *block_storage;
} Table;

typedef struct {
  Table *table;
  uint32_t page_num;
  uint16_t slot_num;
  bool end_of_table;
} Cursor;

typedef struct {
  uint32_t id;
  char username[COLUMN_USERNAME_SIZE];
  char email[COLUMN_EMAIL_SIZE];
} Row;

typedef struct {
  char *buffer;
  size_t buffer_length;
  ssize_t input_length;
} InputBuffer;

typedef enum {
  PREPARE_SUCCESS,
  PREPARE_UNRECOGNIZED_STATEMENT,
  PREPARE_SYNTAX_ERROR
} PrepareResult;

typedef enum { 
  STATEMENT_INSERT, 
  STATEMENT_SELECT 
} StatementType;

typedef struct {
  StatementType type;
  Row row_to_insert; // Only used by insert statements
} Statement;

#endif