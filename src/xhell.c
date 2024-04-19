#include <stdio.h>
#include <ncurses.h> /* for screen display */
#include <string.h>
#include <stdlib.h>

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
    unsigned int cursor_x, cursor_y;
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
        mvprintw(i, 0, "%s", sh->shell_content_t.lines[i]->line_data);
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
        buf->data = realloc(buf->data, buf->cap);
    }
    if (buf->size > 0)
    {
        buf->size--;
        buf->data[buf->size] = '\0';
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
    line->line_data = (char *)malloc(strlen(prefix));
    if (line->line_data == NULL)
    {
        perror("Memmory allocation error");
        return 1;
    }
    line->line_data = strdup(prefix);
    line->size += strlen(prefix);
    sh->shell_content_t.n_lines += 1;
    sh->cursor_y += 1;
    sh->cursor_x = 0;
    sh->shell_content_t.lines = (line_t **)realloc(sh->shell_content_t.lines, sizeof(line_t *) * sh->shell_content_t.n_lines);
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
    sh->cursor_x += l->size;
    return 0;
}

int main(void)
{
    initscr();

    noecho();

    char prefix[] = "/home/user > ";

    refresh();

    /*################################"" INITIALIZATION #########################################""*/

    buffer_t *buffer = (buffer_t *)malloc(sizeof(buffer_t));
    if (buffer == NULL)
    {
        perror("Memmory allocation error");
        exit(1);
    }
    buffer->size = 0;
    buffer->data = malloc(8);
    if (buffer->data == NULL)
    {
        perror("Memmory allocation error");
        exit(1);
    }
    buffer->cap = 8;

    line_t *line = (line_t *)malloc(sizeof(line_t));
    if (line == NULL)
    {
        perror("Memmory allocation error");
        exit(1);
    }
    line->size = 0;
    line->line_data = (char *)malloc(strlen(prefix));
    if (line->line_data == NULL)
    {
        perror("Memmory allocation error");
        exit(1);
    }

    line->line_data = strcat(line->line_data, prefix);
    line->size += strlen(prefix);

    shell_t *sh = (shell_t *)malloc(sizeof(shell_t));
    sh->c_cols = 0;
    sh->cursor_x = 0;
    sh->cursor_y = 0;
    sh->c_line = 0;
    sh->shell_content_t.n_lines = 1;
    sh->shell_content_t.lines = malloc(sh->shell_content_t.n_lines * sizeof(char *));
    if (sh->shell_content_t.lines == NULL)
    {
        perror("Memmory allocation error");
        exit(1);
    }
    sh->cursor_x += line->size;
    sh->shell_content_t.lines[sh->shell_content_t.n_lines - 1] = line;

    // mvprintw(0, 0, prefix);
    refresh_s(sh);
    char ch;
    while (true)
    {

        while (ch = getch())
        {
            if (ch == ENTER)
            {
                append_buffer_to_line(sh, buffer);
                clear_buffer(buffer);
                construct_new_line(sh, prefix);
                refresh_s(sh);
            }
            else if (ch == BACKSPACE)
            {
                remove_char(buffer);
                mvprintw(sh->shell_content_t.n_lines - 1, sh->shell_content_t.lines[sh->shell_content_t.n_lines - 1]->size, "%s", buffer->data);

                // refresh_s(sh);
            }
            else
            {
                append_to_cmd_buffer(buffer, ch);
                mvprintw(sh->shell_content_t.n_lines - 1, sh->shell_content_t.lines[sh->shell_content_t.n_lines - 1]->size, "%s", buffer->data);
                //  refresh_s(sh);
            }
        }
    }

    free_buffer(buffer);
    endwin();
    return 0;
}
