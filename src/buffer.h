#ifndef buffer_t_H
#define buffer_t_H

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/param.h>

#define MIN_buffer_t_SIZE 1024

typedef struct Gapbuffer_t
{
  size_t total_size;
  size_t cursor_pos;
  size_t gap_end_pos;
  char *storage;
} buffer_t;

buffer_t *create_buffer(size_t initial_size);
void free_buffer(buffer_t *buf);

int back_text_size(buffer_t *buf);
int used_text_size(buffer_t *buf);

void move_back_text(buffer_t *buf, char *new_storage, size_t new_size);
void shrink_buffer_t(buffer_t *buf, size_t new_size);
bool expand_buffer_t(buffer_t *buf, size_t new_size);
bool insert_character(buffer_t *buf, char c);
void move_cursor_left(buffer_t *buf);
void move_cursor_right(buffer_t *buf);
void backspace_character(buffer_t *buf);
void delete_character(buffer_t *buf);
char *retrieve_text(buffer_t *buf);
void clear_buffer(buffer_t *buf);

#endif // buffer_t_H
