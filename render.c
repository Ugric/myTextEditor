#include "openFile.h"
#include "config.h"

#include <ncurses.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

void render(struct FileData fileData)
{
    if (COLS < 30 || LINES < 10)
    {
        clear();
        mvprintw(0, 0, "Terminal too small. Please resize to at least 30x10.");
        refresh();
        return;
    }
    char *content = *fileData.content;
    clear();

    // Display Header

    mvprintw(0, 0, "%s - %s\n", programName, *fileData.path);

    // Display content
    char *line = malloc(12);
    sprintf(line, "%d", fileData.numLines);
    int widthOfLineNumText = strlen(line) + 1;
    if (widthOfLineNumText < 4)
    {
        widthOfLineNumText = 4;
    }
    int contentBeginX = widthOfLineNumText + 2;
    int contentBeginY = 2;
    int contentHeight = LINES - 4;
    int contentWidth = COLS - contentBeginX - 1;

    for (int i = 0; i < COLS; i++)
    {
        if (i == contentBeginX - 1)
        {
            printw("╤");
        }
        else
        {
            printw("═");
        }
    }
    int LineNum = 1;
    int newLine = 1;
    int length = strlen(content);
    int lineLength = 0;
    int scroll_x = fileData.cursor_x / (contentWidth + 1);
    int overLine = 0;
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
            if ((i != 0) && (newLine))
            {
                move(contentBeginY + LineNum - fileData.scroll - 1, 0);
            }
            char *line = malloc(12);
            lineLength = 0;
            sprintf(line, "%d", LineNum);
            int linelen = strlen(line);
            for (int i = 0; i < widthOfLineNumText - linelen; i++)
            {
                printw(" ");
            }
            printw("%s", line);
            free(line);
            printw("  ");
            LineNum++;
            newLine = 0;
            overLine = 0;
        }
        if (i < length)
        {
            lineLength++;
            if (content[i] == '\n')
            {
                newLine = 1;
            }
            else if (!overLine)
            {
                if ((lineLength > scroll_x * (contentWidth + 1)) && (lineLength <= ((contentWidth + 1) * (scroll_x + 1))))
                {
                    printw("%c", content[i]);
                }
                else if (lineLength > ((contentWidth + 1) * (scroll_x + 1)))
                {
                    overLine = 1;
                }
            }
        }
    }

    // side line
    for (int i = 0; i < contentHeight; i++)
    {
        move(i + contentBeginY, contentBeginX - 1);
        printw("│");
    }

    // footer
    move(LINES - 2, 0);
    for (int i = 0; i < COLS; i++)
    {
        if (i == contentBeginX - 1)
        {
            printw("╧");
        }
        else
        {
            printw("═");
        }
    }
    if (*fileData.showMessage)
    {
        printw("%s", *fileData.message);
    }
    else
    {
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
    }
    switch (*fileData.mode)
    {
    case 0:
    case 1:
        move(contentBeginY + fileData.cursor_y - fileData.scroll, contentBeginX + (((fileData.cursor_x) % (contentWidth + 1))));
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
