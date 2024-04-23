#include "xhell.h"
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>

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
    buffer->data = malloc(sizeof(char) * 8);
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
    line->line_data = (char *)malloc(strlen(prefix) * sizeof(char));
    if (line->line_data == NULL)
    {
        perror("Memmory allocation error");
        exit(1);
    }

    line->line_data = strcat(line->line_data, prefix);
    line->size += strlen(prefix);

    shell_t *sh = (shell_t *)malloc(sizeof(shell_t));
    sh->c_cols = 0;
    sh->c_line = 0;
    sh->shell_content_t.n_lines = 1;
    sh->shell_content_t.lines = malloc(sh->shell_content_t.n_lines * sizeof(char *));
    if (sh->shell_content_t.lines == NULL)
    {
        perror("Memmory allocation error");
        exit(1);
    }
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
            // else if (ch == BACKSPACE)
            // {
            //     remove_char(buffer);
            //     mvprintw(sh->shell_content_t.n_lines, sh->shell_content_t.lines[sh->shell_content_t.n_lines - 1]->size, sh->shell_content_t.lines[sh->shell_content_t.n_lines - 1]->line_data);
            // }
            else
            {
                append_to_cmd_buffer(buffer, ch);
                mvprintw(sh->shell_content_t.n_lines - 1, sh->shell_content_t.lines[sh->shell_content_t.n_lines - 1]->size, buffer->data);
                // refresh_s(sh);
            }
        }
    }

    free_buffer(buffer);
    endwin();
    return 0;
}