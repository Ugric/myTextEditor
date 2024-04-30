#include "openFile.h"
#include "config.h"

#include <ncurses.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

void render(struct FileData fileData)
{
    char *content = *fileData.content;
    clear();

    // Display Header

    mvprintw(0, 0, "%s - %s\n", programName, *fileData.path);

    // Display content
    char *line = malloc(1);
    sprintf(line, "%d", fileData.numLines);
    int widthOfLineNumText = strlen(line) + 1;
    int contentBeginX = widthOfLineNumText + 3;
    int contentBeginY = 2;
    int contentHeight = LINES - 4;
    int contentWidth = COLS - contentBeginX - 1;

    for (int i = 0; i < COLS; i++)
    {
        if (i == contentBeginX - 2)
        {
            printw("╤");
        }
        else
        {
            printw("═");
        }
    }

    move(contentBeginY, 0);

    int LineNum = 1;
    int newLine = 1;
    int length = strlen(content);
    int lineLength = 0;
    for (int i = 0; (i < length + 1) && (LineNum <= (fileData.scroll + contentHeight + contentBeginY - 1)); i++)
    {
        if (LineNum < fileData.scroll + 1)
        {
            if (i < length && content[i] == '\n')
            {
                LineNum++;
            }
            continue;
        }
        if (newLine)
        {
            char *line = malloc(1);
            lineLength = 0;
            sprintf(line, "%d", LineNum);
            for (int i = 0; i < widthOfLineNumText - strlen(line); i++)
            {
                printw(" ");
            }
            printw("%s", line);
            free(line);
            printw(" │ ");
            LineNum++;
            newLine = 0;
        }
        if (i < length && content[i] == '\n')
        {
            newLine = 1;

        }
        if ((i < length) && (lineLength-1 < contentWidth))
        {
            printw("%c", content[i]);
            lineLength++;
        }
    }

    // footer
    move(LINES - 2, 0);
    for (int i = 0; i < COLS; i++)
    {
        if (i == contentBeginX - 2)
        {
            printw("╧");
        }
        else
        {
            printw("═");
        }
    }
    switch (*fileData.mode)
    {
    case 0:
        printw("Normal Mode");
        break;
    case 1:
        printw("Insert Mode");
        break;
    case 2:
        printw(":%s", *fileData.command);
        break;
    default:
        printw("Unknown Mode");
        break;
    }
    switch (*fileData.mode)
    {
    case 0:
    case 1:
        move(contentBeginY + fileData.cursor_y - fileData.scroll, contentBeginX + fileData.cursor_x);
        break;
    case 2:
        move(LINES - 1, 1 + strlen(*fileData.command));
        break;
    default:
        break;
    }

    refresh();
    free(line);
}