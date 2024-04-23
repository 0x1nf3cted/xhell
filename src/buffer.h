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

typedef struct line
{
  int size;
  char *line_data;
} line_t;

typedef struct shell
{
  struct state_t
  {
    int n_lines;
    line_t **lines;
  } shell_content_t;
  buffer_t *current_buffer;
  unsigned int cursor_x, cursor_y;
  unsigned int c_line, c_cols;
  char *prefix;

} shell_t;

buffer_t *create_buffer(size_t initial_size);
void free_buffer(buffer_t *buf);

int back_text_size(buffer_t *buf);
int used_text_size(buffer_t *buf);

void move_back_text(buffer_t *buf, char *new_storage, size_t new_size);
void shrink_buffer_t(buffer_t *buf, size_t new_size);
bool expand_buffer_t(buffer_t *buf, size_t new_size);
bool insert_character(shell_t *sh, char c);
void move_cursor_left(shell_t *sh);
void move_cursor_right(shell_t *sh);
void backspace_character(shell_t *sh);
void delete_character(buffer_t *buf);
char *retrieve_text(buffer_t *buf);
void clear_buffer(buffer_t *buf);

#endif // buffer_t_H
