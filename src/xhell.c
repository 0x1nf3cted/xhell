#include <stdio.h>
#include <ncurses.h> /* for screen display */
#include <string.h>
#include <stdlib.h>
#include <unistd.h> // for STDIN_FILENO
#include "buffer.h"

#define ALLOC_CHECK(ptr)                       \
    do                                         \
    {                                          \
        if (!(ptr))                            \
        {                                      \
            perror("Memory allocation error"); \
            exit(1);                           \
        }                                      \
    } while (0)

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

void refresh_s(shell_t *sh)
{
    char *text = retrieve_text(sh->current_buffer);
    ALLOC_CHECK(text);
    for (int i = 0; i < sh->shell_content_t.n_lines; i++)
    {
        mvprintw(i, 0, "%s", sh->shell_content_t.lines[i]->line_data);
    }
    write(STDOUT_FILENO, "\033[2K\033[G", 7);
    mvprintw(sh->shell_content_t.n_lines - 1, strlen(sh->prefix), "%s", text);
    refresh();
}

int construct_new_line(shell_t *sh)
{
    line_t *line = (line_t *)malloc(sizeof(line_t));
    ALLOC_CHECK(line);
    line->size = sizeof(strlen(sh->prefix));
    line->line_data = malloc(line->size);
    ALLOC_CHECK(line->line_data);
    strcat(line->line_data, sh->prefix);

    sh->shell_content_t.lines = realloc(sh->shell_content_t.lines, (sh->shell_content_t.n_lines + 1) * sizeof(line_t *));
    ALLOC_CHECK(sh->shell_content_t.lines);
    sh->shell_content_t.lines[sh->shell_content_t.n_lines] = line;
    sh->shell_content_t.n_lines++;

    return 0;
}

int append_buffer_to_line(shell_t *sh, buffer_t *buf)
{
    char *text = retrieve_text(buf);
    ALLOC_CHECK(text);
    strcat(sh->shell_content_t.lines[sh->shell_content_t.n_lines - 1]->line_data, text);
    return 0;
}

int main(void)
{
    initscr();
    noecho();
    char prefix[] = "/home/user > ";
    refresh();

    /*################################"" INITIALIZATION #########################################""*/

    shell_t *sh = (shell_t *)malloc(sizeof(shell_t));
    ALLOC_CHECK(sh);
    sh->shell_content_t.n_lines = 0;
    sh->shell_content_t.lines = NULL;
    sh->current_buffer = create_buffer(8);
    sh->prefix = malloc(sizeof(prefix));
    ALLOC_CHECK(sh->prefix);
    sh->prefix = strdup(prefix);
    construct_new_line(sh);
    refresh_s(sh);

    refresh_s(sh);
    char ch;
    while (true)
    {

        while (ch = getch())
        {
            if (ch == 27)
            { // Escape character
                int ch2 = getchar();
                if (ch2 == 91)
                { // [
                    int ch3 = getchar();
                    if (ch3 == 65)
                    { // A
                        printf("Up arrow\n");
                    }
                    else if (ch3 == 66)
                    { // B
                        printf("Down arrow\n");
                    }
                    else if (ch3 == 67)
                    { // C
                        move_cursor_right(sh->current_buffer);
                        refresh_s(sh->current_buffer);
                    }
                    else if (ch3 == 68)
                    { // D
                        move_cursor_left(sh->current_buffer);
                        refresh_s(sh->current_buffer);
                    }
                }
            }

            else if (ch == ENTER)
            {
                append_buffer_to_line(sh, sh->current_buffer);
                clear_buffer(sh->current_buffer);
                construct_new_line(sh);
                refresh_s(sh);
            }

            else if (ch == BACKSPACE)
            {
                backspace_character(sh->current_buffer);
                refresh_s(sh);
            }
            else
            {
                insert_character(sh->current_buffer, ch);
                refresh_s(sh);
            }
        }
    }

    free_buffer(sh->current_buffer);
    endwin();
    return 0;
}
