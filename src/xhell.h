#ifndef SHELL_H
#define SHELL_H

#include <stdint.h>

typedef struct line
{
    uint32_t size;
    char *line_data;
} line_t;

typedef struct shell
{
    struct state_t
    {
        int n_lines;
        line_t **lines;
    } shell_content_t;

    unsigned int c_line, c_cols;

} shell_t;

enum Keypress
{
    ENTER = 10,
    ESCAPE = 27,
    BACKSPACE = 127,
    TAB = 9,
    CTRL_C = 3,
    CTRL_S = 19,
    CTRL_V = 22,
    CTRL_X = 24,
    UP_ARROW = 57,
    DOWN_ARROW = 80,
    RIGHT_ARROW = 77,
    LEFT_ARROW = 75
};

typedef struct buffer
{
    uint32_t cap;
    uint32_t size;
    char *data;
} buffer_t;

void refresh_s(shell_t *sh);

int append_to_cmd_buffer(buffer_t *buf, char c);

int remove_char(buffer_t *buf);

int clear_buffer(buffer_t *buf);

void free_buffer(buffer_t *buf);

int construct_new_line(shell_t *sh, char *prefix);

int append_buffer_to_line(shell_t *sh, buffer_t *buffer);

#endif /* SHELL_H */
