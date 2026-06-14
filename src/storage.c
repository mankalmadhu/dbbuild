#include "storage.h"
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int db_fd = -1;

StorageResult storage_init(void) {
  if (db_open("test.db") == STORAGE_SUCCESS) {
    return STORAGE_SUCCESS;
  }
  return STORAGE_ERROR;
}

int db_open(const char *filename) {

  db_fd = open(filename, O_RDWR | O_CREAT, 0644);

  if (db_fd == -1) {
    perror("Error opening database file");
    return STORAGE_ERROR;
  }

  return STORAGE_SUCCESS;
}

void db_close(void) {
  close(db_fd);
  db_fd = -1;
}

Page *page_get(uint32_t page_num) {
  Page *page = malloc(sizeof(*page));
  if (!page) return NULL;

  page->data = malloc(PAGE_SIZE);
  if (!page->data) {
    free(page);
    return NULL;
  }
  
  page->page_num = page_num;
  
  off_t offset = page_num * PAGE_SIZE;
  if (lseek(db_fd, offset, SEEK_SET) == -1) {
    perror("Error seeking to offset for page get");
    free(page->data);
    free(page);
    return NULL;
  }
  
  ssize_t bytes_read = read(db_fd, page->data, PAGE_SIZE);
  if (bytes_read == -1) {
    perror("Error reading page from file");
    free(page->data);
    free(page);
    return NULL;
  }

  // If reading a new or partial page, initialize it properly
  if (bytes_read < PAGE_SIZE) {
    page_init(page->data, page_num, PAGE_TYPE_TABLE_LEAF);
  }

  return page;
}

int page_write(Page *page) {
  off_t offset = page->page_num * PAGE_SIZE;
  
  if (lseek(db_fd, offset, SEEK_SET) == -1) {
    perror("Error seeking to offset for page write");
    return -1;
  }

  ssize_t bytes_written = write(db_fd, page->data, PAGE_SIZE);
  if (bytes_written == -1) {
    perror("Error writing page to file");
    return -1;
  }

  return 0;
}

void page_free(Page *page) {
  if (page) {
    if (page->data) {
      free(page->data);
    }
    free(page);
  }
}

void page_init(void *page_data, uint32_t page_id, PageType type) {
  memset(page_data, 0, PAGE_SIZE);

  PageHeader *header = (PageHeader *)page_data;
  header->page_id = page_id;
  header->next_page = 0;
  header->type = type;
  header->item_count = 0;
  header->free_space = PAGE_SIZE - sizeof(PageHeader);
  header->free_ptr = PAGE_SIZE;
}
