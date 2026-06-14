CC = gcc
CFLAGS = -Wall -Wextra -g -std=c11 -I./src
LDFLAGS =

SRC_DIR = src
OBJ_DIR = obj
TEST_DIR = tests

SRCS = $(wildcard $(SRC_DIR)/*.c)
OBJS = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SRCS))

# Separate main.o from the rest of the objects for testing
CORE_OBJS = $(filter-out $(OBJ_DIR)/main.o, $(OBJS))

TARGET = db
TEST_TARGET = test_runner

.PHONY: all clean test

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

clean:
	rm -rf $(OBJ_DIR) $(TARGET) $(TEST_TARGET)

TEST_SRCS = $(wildcard $(TEST_DIR)/*.c)

test: $(CORE_OBJS)
	$(CC) $(CFLAGS) -o $(TEST_TARGET) $(TEST_SRCS) $(CORE_OBJS) $(LDFLAGS)
	./$(TEST_TARGET)
