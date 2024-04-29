#include "openFile.h"
#include "config.h"
#include "render.h"

#include <ncurses.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

struct lineAndCharNum
{
    int lineNum;
    int charNum;
};

struct lineAndCharNum getLineAndCharNum(char *content)
{
    int numLines = 1;
    int numChars = strlen(content);
    for (int i = 0; i < strlen(content); i++)
    {
        if (content[i] == '\n')
        {
            numLines++;
        }
    }
    struct lineAndCharNum lineAndCharNum = {numLines, numChars};
    return lineAndCharNum;
}

int lineLength(char *content, int cursor_y)
{
    int numLines = 0;
    int numChars = strlen(content);
    int across = 0;
    for (int i = 0; i < strlen(content); i++)
    {
        if (content[i] == '\n')
        {
            if (numLines == cursor_y)
            {
                return across;
            }
            numLines++;
            across = 0;
        }
        else
        {
            across++;
        }
    }
    return across;
}

int openFile(char *path)
{
    ESCDELAY = 0;
    char *content = malloc(1);

    if (strlen(path) != 0)
    {
        FILE *fptr;
        fptr = fopen(path, "r");
        if (fptr != NULL)
        {
            // Read the contents of the file into the content variable
            fseek(fptr, 0, SEEK_END);
            long fsize = ftell(fptr);
            fseek(fptr, 0, SEEK_SET);
            free(content);
            content = malloc(fsize + 1);
            fread(content, 1, fsize, fptr);
            content[fsize] = 0;
            fclose(fptr);
        }
    }
    int mode = 0;

    struct FileData fileData = {&path, &content, &mode, 0, 0, 0, 0, 0};

    initscr();
    cbreak();
    keypad(stdscr, TRUE);

    struct lineAndCharNum lineAndCharNum = getLineAndCharNum(content);
    fileData.numLines = lineAndCharNum.lineNum;
    fileData.numChars = lineAndCharNum.charNum;
    render(fileData);

    int across = 0;

    int choice;
    int cursorLineLength;
    int running = 1;
    while (1)
    {
        choice = getch(); // Get user input
        if (mode != 2)
        {
            switch (choice)
            {
            case KEY_UP:
                if (fileData.cursor_y > 0)
                {
                    fileData.cursor_y--;
                    if (fileData.cursor_y < fileData.scroll)
                    {
                        fileData.scroll--;
                    }
                    int cursorLineLength = lineLength(content, fileData.cursor_y);
                    fileData.cursor_x = across;
                    if (fileData.cursor_x > cursorLineLength)
                    {
                        fileData.cursor_x = cursorLineLength;
                    }
                }
                break;
            case KEY_DOWN:
                if (fileData.cursor_y < fileData.numLines - 1)
                {
                    fileData.cursor_y++;
                    if (fileData.cursor_y - (LINES - 4) == fileData.scroll)
                    {
                        fileData.scroll++;
                    }
                    int cursorLineLength = lineLength(content, fileData.cursor_y);
                    fileData.cursor_x = across;
                    if (fileData.cursor_x > cursorLineLength)
                    {
                        fileData.cursor_x = cursorLineLength;
                    }
                }
                break;
            case KEY_LEFT:
                if (across > 0)
                {
                    across--;
                }
                cursorLineLength = lineLength(content, fileData.cursor_y);
                fileData.cursor_x = across;
                if (fileData.cursor_x > cursorLineLength)
                {
                    fileData.cursor_x = cursorLineLength;
                }
                break;
            case KEY_RIGHT:
                int lineWidth = lineLength(content, fileData.cursor_y);
                if (across < lineWidth)
                {
                    across++;
                }
                cursorLineLength = lineLength(content, fileData.cursor_y);
                fileData.cursor_x = across;
                if (fileData.cursor_x > cursorLineLength)
                {
                    fileData.cursor_x = cursorLineLength;
                }
                break;
            }
        }

        switch (mode)
        {
        case 0:
            switch (choice)
            {
            case ':':
                mode = 2;
                break;
            case 'i':
                mode = 1;
                break;
            default:
                break;
            }
            break;
        case 1:
            switch (choice)
            {
            case 27:
                mode = 0;
                break;

            default:
                break;
            }
        case 2:
            switch (choice)
            {
            case 27:
                mode = 0;
                break;
            default:
                break;
            }
            break;
        default:
            break;
        }
        if (!running)
        {
            break;
        }
        lineAndCharNum = getLineAndCharNum(content);
        fileData.numLines = lineAndCharNum.lineNum;
        fileData.numChars = lineAndCharNum.charNum;
        render(fileData); // Re-render the menu
    }
    free(content);
    // Clean up and exit ncurses
    endwin();

    return 0;
}