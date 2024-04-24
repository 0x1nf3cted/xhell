CC = gcc
CFLAGS = -Wall -Wextra -g -lncurses 
SRC_DIR = src
TEST_DIR = tests
BUILD_DIR = build
EXECUTABLE = xhell
TEST_EXECUTABLE = test_xhell

SRCS = $(wildcard $(SRC_DIR)/*.c)
TEST_SRCS = $(wildcard $(TEST_DIR)/*.cpp)

OBJS = $(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR)/%.o,$(SRCS))
TEST_OBJS = $(patsubst $(TEST_DIR)/%.cpp,$(BUILD_DIR)/%.o,$(TEST_SRCS)) $(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR)/%.o,$(SRCS))

# Main target
all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ -lncurses

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c -o $@ $<

$(BUILD_DIR)/%.o: $(TEST_DIR)/%.cpp
	$(CC) $(CFLAGS) -I$(SRC_DIR) -c -o $@ $<

$(TEST_EXECUTABLE): $(TEST_OBJS)
	$(CC) $(CFLAGS) -o $@ $^ -lgtest -lgtest_main -pthread

clean:
	rm -f $(BUILD_DIR)/*.o $(EXECUTABLE) $(TEST_EXECUTABLE)

# Run executable
run: $(EXECUTABLE)
	./$(EXECUTABLE)

# Run tests
test: $(TEST_EXECUTABLE)
	./$(TEST_EXECUTABLE)

.PHONY: all clean run test
