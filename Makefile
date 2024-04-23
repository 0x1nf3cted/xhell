CC = gcc
CFLAGS = -Wall -Wextra -g -lncurses
SRC_DIR = src
BUILD_DIR = build
EXECUTABLE = xhell

# List of source files
SRCS = $(wildcard $(SRC_DIR)/*.c)

# List of object files
OBJS = $(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR)/%.o,$(SRCS))

# Main target
all: $(EXECUTABLE)

# Rule to build the executable
$(EXECUTABLE): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

# Rule to compile source files into object files
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c -o $@ $<

# Clean rule
clean:
	rm -f $(BUILD_DIR)/*.o $(EXECUTABLE)

# Run rule
run: $(EXECUTABLE)
	./$(EXECUTABLE)

.PHONY: all clean run