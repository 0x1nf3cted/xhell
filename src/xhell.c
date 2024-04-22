#include <stdio.h>
#include <ncurses.h> /* for screen display */
#include <string.h>
#include <stdlib.h>
#include "xhell.h"

#define ERROR_FREE(data)                       \
    do                                         \
    {                                          \
        if (data == NULL)                      \
            perror("mEMORY ALLOCATION ERROR"); \
        exit(1);                               \
    } while (0)

void refresh_s(shell_t *sh)
{
    for (int i = 0; i < sh->shell_content_t.n_lines; i++)
    {
        mvprintw(i, 0, sh->shell_content_t.lines[i]->line_data);
    }
    refresh();
}

int append_to_cmd_buffer(buffer_t *buf, char c)
{

    if (buf->size == buf->cap)
    {
        buf->cap *= 2;
        buf->data = realloc(buf->data, buf->cap);
        if (buf->data == NULL)
        {
            perror("Memmory allocation error");
            return 1;
        }
    }
    buf->data[buf->size++] = c;

    return 0;
}

int remove_char(buffer_t *buf)
{
    if ((buf->size == buf->cap) && buf->cap > 8)
    {
        buf->cap /= 2;
    }
    if (buf->size > 0)
    {
        buf->size--;
        buf->data = realloc(buf->data, buf->cap);
    }
}
int clear_buffer(buffer_t *buf)
{
    memset(buf->data, 0, buf->size);
    buf->size = 0;
    buf->cap = 8;
    // buf->data = malloc(sizeof(char) * buf->cap);
    // buf->data[0] = '\0';
}

void free_buffer(buffer_t *buf)
{
    free(buf->data);
    free(buf);
}

// TODO: add buffer to the current line
// add  the prefix to the current line
//

int construct_new_line(shell_t *sh, char *prefix)
{
    line_t *line = (line_t *)malloc(sizeof(line_t));
    if (line == NULL)
    {
        perror("Memmory allocation error");
        return 1;
    }
    line->size = 0;
    line->line_data = (char *)malloc(strlen(prefix) * sizeof(char));
    if (line->line_data == NULL)
    {
        perror("Memmory allocation error");
        return 1;
    }
    line->line_data = strdup(prefix);
    line->size += strlen(prefix);
    sh->shell_content_t.n_lines += 1;
    sh->shell_content_t.lines = (line_t**)realloc(sh->shell_content_t.lines, sh->shell_content_t.n_lines + 1);
    if (sh->shell_content_t.lines == NULL)
    {
        perror("Memmory allocation error");
        return 1;
    }
    sh->shell_content_t.lines[sh->shell_content_t.n_lines - 1] = line;
    return 0;
}

int append_buffer_to_line(shell_t *sh, buffer_t *buffer)
{
    line_t *l = sh->shell_content_t.lines[sh->shell_content_t.n_lines - 1];
    l->line_data = realloc(l->line_data, l->size + buffer->size);
    if (l->line_data == NULL)
    {
        perror("Memmory allocation error");
        return 1;
    }
    l->line_data = strcat(l->line_data, buffer->data);
    l->size += buffer->size;
    return 0;
}