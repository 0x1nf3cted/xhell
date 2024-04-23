#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/param.h>

#include "buffer.h"

#define MIN_buffer_t_SIZE 1024

int back_text_size(buffer_t *buf)
{
  return buf->total_size - buf->gap_end_pos;
}

int used_text_size(buffer_t *buf)
{
  return buf->cursor_pos + back_text_size(buf);
}

buffer_t *
create_buffer(size_t initial_size)
{
  buffer_t *buf = malloc(sizeof *buf);
  if (!buf)
  {
    return NULL;
  }
  initial_size = MAX(initial_size, MIN_buffer_t_SIZE);
  buf->storage = malloc(initial_size);
  if (!buf->storage)
  {
    free(buf);
    return NULL;
  }
  buf->total_size = initial_size;
  buf->cursor_pos = 0;
  buf->gap_end_pos = initial_size;
  return buf;
}

void free_buffer(buffer_t *buf)
{
  if (!buf)
  {
    return;
  }
  free(buf->storage);
  free(buf);
}

void move_back_text(buffer_t *buf, char *new_storage, size_t new_size)
{
  memmove(new_storage + new_size - back_text_size(buf),
          buf->storage + buf->gap_end_pos,
          back_text_size(buf));
}

void shrink_buffer_t(buffer_t *buf, size_t new_size)
{
  new_size = MAX(new_size, MIN_buffer_t_SIZE);
  if (new_size < used_text_size(buf))
  {
    return 1;
  }
  move_back_text(buf, buf->storage, new_size);
  buf->gap_end_pos = new_size - back_text_size(buf);
  buf->total_size = new_size;
  char *new_storage = realloc(buf->storage, new_size);
  if (new_storage)
  {
    buf->storage = new_storage;
  }
}

bool expand_buffer_t(buffer_t *buf, size_t new_size)
{
  new_size = MAX(new_size, MIN_buffer_t_SIZE);
  if (buf->total_size >= new_size)
  {
    return false;
  }
  char *new_storage = realloc(buf->storage, new_size);
  if (!new_storage)
  {
    return false;
  }
  move_back_text(buf, new_storage, new_size);
  buf->storage = new_storage;
  buf->gap_end_pos = new_size - back_text_size(buf);
  buf->total_size = new_size;
  return true;
}

bool insert_character(shell_t *sh, char c)
{
  if (sh->current_buffer->cursor_pos == sh->current_buffer->gap_end_pos)
  {
    size_t new_size = sh->current_buffer->total_size * 2;
    if (!expand_buffer_t(sh->current_buffer, new_size))
    {
      return false;
    }
  }
  sh->current_buffer->storage[sh->current_buffer->cursor_pos++] = c;
  sh->cursor_x++;

  return true;
}

void move_cursor_left(shell_t *sh)
{
  if (sh->current_buffer->cursor_pos > 0)
  {
    sh->current_buffer->storage[--sh->current_buffer->gap_end_pos] = sh->current_buffer->storage[--sh->current_buffer->cursor_pos];
    sh->cursor_x--;
  }
}

void move_cursor_right(shell_t *sh)
{
  if (sh->current_buffer->gap_end_pos < sh->current_buffer->total_size)
  {
    sh->current_buffer->storage[sh->current_buffer->cursor_pos++] = sh->current_buffer->storage[sh->current_buffer->gap_end_pos++];
    sh->cursor_x++;
  }
}

void backspace_character(shell_t *sh)
{
  if (sh->current_buffer->cursor_pos > 0)
  {
    sh->current_buffer->cursor_pos--;
    sh->cursor_x--;
  }
  if (used_text_size(sh->current_buffer) < sh->current_buffer->total_size / 4)
  {
    shrink_buffer_t(sh->current_buffer, sh->current_buffer->total_size / 2);
  }
}



char *
retrieve_text(buffer_t *buf)
{
  if (SIZE_MAX == used_text_size(buf))
  {
    return NULL;
  }
  char *text = malloc(used_text_size(buf) + 1);
  if (!text)
  {
    return NULL;
  }
  strncpy(text, buf->storage, buf->cursor_pos);
  strncpy(text + buf->cursor_pos, buf->storage + buf->gap_end_pos, buf->total_size - buf->gap_end_pos);
  text[used_text_size(buf)] = '\0';
  return text;
}

void clear_buffer(buffer_t *buf)
{
  buf->cursor_pos = 0;
  buf->gap_end_pos = buf->total_size;
  memset(buf->storage, 0, buf->total_size);
}
