#include <stdio.h>
#include <ncurses.h> /* for screen display */
#include <string.h>
#include <stdlib.h>
#include <unistd.h> // for STDIN_FILENO
#include "buffer.h"

// check if the allocation was successful
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

/*
refresh the screen each time the function is called
*/

void refresh_s(shell_t *sh)
{
    char *text = retrieve_text(sh->current_buffer);
    ALLOC_CHECK(text);
    for (int i = 0; i < sh->shell_content_t.n_lines; i++)
    {
        mvprintw(i, 0, "%s", sh->shell_content_t.lines[i]->line_data);
    }
    move(sh->shell_content_t.n_lines - 1, 0); // move to begining of line
    clrtoeol();                               // clear line
    mvprintw(sh->shell_content_t.n_lines - 1, 0, "%s", sh->prefix);
    mvprintw(sh->shell_content_t.n_lines - 1, strlen(sh->prefix), "%s", text);
    refresh();
}

/*
create a new line and add it to the shell
*/

int construct_new_line(shell_t *sh)
{
    line_t *line = (line_t *)malloc(sizeof(line_t));
    ALLOC_CHECK(line);

    sh->shell_content_t.lines = realloc(sh->shell_content_t.lines, (sh->shell_content_t.n_lines + 1) * sizeof(line_t *));
    ALLOC_CHECK(sh->shell_content_t.lines);
    sh->shell_content_t.lines[sh->shell_content_t.n_lines] = line;
    sh->shell_content_t.n_lines++;

    sh->current_buffer = create_buffer(8);

    return 0;
}

/*
this wierd function will basically append the current buffer + the prefix to the last line

*/
int append_buffer_to_line(shell_t *sh)
{
    char *text = retrieve_text(sh->current_buffer);
    ALLOC_CHECK(text);
    // some real crappy code; will refactor later
    sh->shell_content_t.lines[sh->shell_content_t.n_lines - 1]->size = strlen(sh->prefix);
    size_t len = sh->shell_content_t.lines[sh->shell_content_t.n_lines - 1]->size;
    sh->shell_content_t.lines[sh->shell_content_t.n_lines - 1]->line_data = malloc(len);
    ALLOC_CHECK(sh->shell_content_t.lines[sh->shell_content_t.n_lines - 1]->line_data);
    strcat(sh->shell_content_t.lines[sh->shell_content_t.n_lines - 1]->line_data, sh->prefix);

    // Get the lengths of the strings
    size_t line_len = strlen(sh->shell_content_t.lines[sh->shell_content_t.n_lines - 1]->line_data);
    size_t text_len = strlen(text);

    // Allocate enough space for the new string
    char *new_line = malloc(line_len + text_len + 1); // +1 for the null terminator
    ALLOC_CHECK(new_line);

    // Concatenate the strings
    strncpy(new_line, sh->shell_content_t.lines[sh->shell_content_t.n_lines - 1]->line_data, line_len);
    strcat(new_line, text);

    // Free the old line and replace it with the new one
    free(sh->shell_content_t.lines[sh->shell_content_t.n_lines - 1]->line_data);
    sh->shell_content_t.lines[sh->shell_content_t.n_lines - 1]->line_data = new_line;

    free(text);
    free_buffer(sh->current_buffer);
    return 0;
}

int main(void)
{
    initscr();
    noecho();
    char prefix[] = "/home/user > ";
    refresh();

    /*################################"" INITIALIZATION #########################################""*/

    // shell struct initialization, it will hold the prefix `current directory`
    shell_t *sh = (shell_t *)malloc(sizeof(shell_t));
    ALLOC_CHECK(sh);
    sh->shell_content_t.n_lines = 0;
    sh->shell_content_t.lines = NULL;
    sh->current_buffer = create_buffer(8);
    sh->prefix = malloc(sizeof(prefix));
    ALLOC_CHECK(sh->prefix);
    sh->prefix = strdup(prefix);
    // this will create the gap buffer that will hold the commands
    construct_new_line(sh);
    // refresh the screen
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
                    /*
                    not working for now, it will cause a seg fault*/
                    else if (ch3 == 67)
                    { // C
                      // move_cursor_right(sh->current_buffer);
                      // refresh_s(sh->current_buffer);
                    }
                    else if (ch3 == 68)
                    { // D
                      // move_cursor_left(sh->current_buffer);
                      // refresh_s(sh->current_buffer);
                    }
                }
            }

            else if (ch == ENTER)
            {
                /*
                each time enter is clicked we will append the current buffer + the prefix to the current line
                and then construct a new line that will hold the data for the next command
                */
                append_buffer_to_line(sh);
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
