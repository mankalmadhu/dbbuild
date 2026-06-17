#include "storage.h"
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

BlockStorage* block_storage_open(const char* filename) {
  int fd = open(filename, O_RDWR | O_CREAT, 0644);
  if (fd == -1) {
    perror("Error opening database file");
    return NULL;
  }

  BlockStorage* block_storage = malloc(sizeof(BlockStorage));
  if (!block_storage) {
    close(fd);
    return NULL;
  }

  block_storage->file_descriptor = fd;

  off_t file_length = lseek(fd, 0, SEEK_END);
  if (file_length == -1) {
    perror("Error seeking to end of file");
    free(block_storage);
    close(fd);
    return NULL;
  }

  block_storage->file_length = file_length;
  block_storage->num_pages = file_length / PAGE_SIZE;

  return block_storage;
}

void block_storage_close(BlockStorage* block_storage) {
  if (block_storage) {
    close(block_storage->file_descriptor);
    free(block_storage);
  }
}

Page* block_storage_get_page(BlockStorage* block_storage, uint32_t page_num) {
  Page* page = malloc(sizeof(Page));
  if (!page)
    return NULL;

  page->data = malloc(PAGE_SIZE);
  if (!page->data) {
    free(page);
    return NULL;
  }

  page->page_num = page_num;

  off_t offset = page_num * PAGE_SIZE;
  if (lseek(block_storage->file_descriptor, offset, SEEK_SET) == -1) {
    perror("Error seeking to offset for page get");
    free(page->data);
    free(page);
    return NULL;
  }

  ssize_t bytes_read = read(block_storage->file_descriptor, page->data, PAGE_SIZE);
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

int block_storage_write_page(BlockStorage* block_storage, Page* page) {
  off_t offset = page->page_num * PAGE_SIZE;

  if (lseek(block_storage->file_descriptor, offset, SEEK_SET) == -1) {
    perror("Error seeking to offset for page write");
    return -1;
  }

  ssize_t bytes_written = write(block_storage->file_descriptor, page->data, PAGE_SIZE);
  if (bytes_written == -1) {
    perror("Error writing page to file");
    return -1;
  }

  return 0;
}

uint32_t block_storage_get_page_count(BlockStorage* block_storage) {
  if (block_storage)
    return block_storage->num_pages;
  return 0;
}

Page* block_storage_append_page(BlockStorage* block_storage) {
  uint32_t new_page_num = block_storage->num_pages;
  
  Page* page = malloc(sizeof(Page));
  if (!page) return NULL;

  page->data = malloc(PAGE_SIZE);
  if (!page->data) {
    free(page);
    return NULL;
  }
  
  page->page_num = new_page_num;
  page_init(page->data, new_page_num, PAGE_TYPE_TABLE_LEAF);
  
  block_storage->num_pages++;
  block_storage->file_length += PAGE_SIZE;
  
  return page;
}

void page_free(Page* page) {
  if (page) {
    if (page->data) {
      free(page->data);
    }
    free(page);
  }
}

void page_init(void* page_data, uint32_t page_id, PageType type) {
  memset(page_data, 0, PAGE_SIZE);

  PageHeader* header = (PageHeader *)page_data;
  header->page_id = page_id;
  header->next_page = 0;
  header->type = type;
  header->item_count = 0;
  header->free_space = PAGE_SIZE - sizeof(PageHeader);
  header->free_ptr = PAGE_SIZE;
}
