#include "openFile.h"
#include "config.h"
#include "render.h"

#include <ncurses.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <locale.h>

struct lineAndCharNum
{
    int lineNum;
    int charNum;
};

struct lineAndCharNum getLineAndCharNum(char *content)
{
    int numLines = 1;
    int numChars = strlen(content);
    for (int i = 0; i < numChars; i++)
    {
        if (content[i] == '\n')
        {
            numLines++;
        }
    }
    return (struct lineAndCharNum){numLines, numChars};
}

int lineLength(char *content, int cursor_y)
{
    int numLines = 0;
    int numChars = strlen(content);
    int across = 0;
    for (int i = 0; i < numChars; i++)
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

int cursorToCharPos(char *content, int cursor_x, int cursor_y)
{
    if (cursor_y == 0)
    {
        return cursor_x;
    }
    int numLines = 0;
    int numChars = strlen(content);
    int across = 0;
    int i;
    for (i = 0; i < numChars; i++)
    {
        if (content[i] == '\n')
        {
            numLines++;
            if (numLines == cursor_y)
            {
                return i + cursor_x + 1;
            }
            across = 0;
        }
        else
        {
            across++;
        }
    }
    return i;
}

int openFile(char *path)
{
    ESCDELAY = 0;
    char *content = malloc(0);

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
            content = realloc(content, fsize + 1);
            fread(content, 1, fsize, fptr);
            content[fsize] = 0;
            fclose(fptr);
        }
    }
    // path = malloc(100);
    int mode = 0;
    char *command = malloc(1);
    command[0] = '\0';

    struct FileData fileData = {&path, &content, &command, &mode, 0, 0, 0, 0, 0};

    setlocale(LC_ALL, "");
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
    char *newContent;
    int lineWidth;
    int running = 1;
    while (1)
    {
        choice = getch(); // Get user input
        // sprintf(path, "%s, %02X", path, choice);
        int stop = 0;
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
                stop = 1;
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
                stop = 1;
                break;
            case KEY_LEFT:
                if (fileData.cursor_x > 0)
                {
                    fileData.cursor_x--;
                    across = fileData.cursor_x;
                }
                else if (fileData.cursor_y > 0)
                {
                    fileData.cursor_y--;
                    across = lineLength(content, fileData.cursor_y);
                }
                cursorLineLength = lineLength(content, fileData.cursor_y);
                fileData.cursor_x = across;
                if (fileData.cursor_x > cursorLineLength)
                {
                    fileData.cursor_x = cursorLineLength;
                }
                stop = 1;
                break;
            case KEY_RIGHT:
                lineWidth = lineLength(content, fileData.cursor_y);
                if (across < lineWidth)
                {
                    across++;
                }
                else if (fileData.cursor_y < fileData.numLines - 1)
                {
                    across = 0;
                    fileData.cursor_y++;
                }
                cursorLineLength = lineLength(content, fileData.cursor_y);
                fileData.cursor_x = across;
                if (fileData.cursor_x > cursorLineLength)
                {
                    fileData.cursor_x = cursorLineLength;
                }
                stop = 1;
                break;
            }
        }
        if (!stop)
        {
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
                case 10:
                    newContent = malloc(strlen(content) + 2);
                    strncpy(newContent, content, cursorToCharPos(content, fileData.cursor_x, fileData.cursor_y));
                    newContent[cursorToCharPos(content, fileData.cursor_x, fileData.cursor_y)] = '\n';
                    strncpy(newContent + cursorToCharPos(content, fileData.cursor_x, fileData.cursor_y) + 1, content + cursorToCharPos(content, fileData.cursor_x, fileData.cursor_y), strlen(content) - cursorToCharPos(content, fileData.cursor_x, fileData.cursor_y));
                    newContent[strlen(content) + 1] = '\0';
                    free(content);
                    content = newContent;
                    fileData.cursor_y++;
                    fileData.cursor_x = 0;
                    across = 0;
                    break;
                case 27:
                    mode = 0;
                    break;
                case KEY_BACKSPACE:
                case 0x7F:
                    if (strlen(content) > 0)
                    {
                        int cursorPos = cursorToCharPos(content, fileData.cursor_x, fileData.cursor_y);
                        if (cursorPos == 0)
                        {
                            break;
                        }
                        int length = lineLength(content, fileData.cursor_y - 1);
                        char *newContent = malloc(strlen(content));
                        strncpy(newContent, content, cursorPos - 1);
                        strncpy(newContent + cursorPos - 1, content + cursorPos, strlen(content) - cursorPos);
                        newContent[strlen(content) - 1] = '\0';
                        free(content);
                        content = newContent;
                        fileData.cursor_x--;
                        if (fileData.cursor_x < 0 && fileData.cursor_y > 0)
                        {
                            fileData.cursor_y--;
                            fileData.cursor_x = length;
                        }
                        across = fileData.cursor_x;
                    }
                    break;
                case 0x17:
                case KEY_DC:
                    if (strlen(content) > 0)
                    {
                        int cursorPos = cursorToCharPos(content, fileData.cursor_x, fileData.cursor_y);
                        if (cursorPos == strlen(content))
                        {
                            break;
                        }
                        int length = lineLength(content, fileData.cursor_y);
                        char *newContent = malloc(strlen(content));
                        strncpy(newContent, content, cursorPos);
                        strncpy(newContent + cursorPos, content + cursorPos + 1, strlen(content) - cursorPos);
                        newContent[strlen(content) - 1] = '\0';
                        free(content);
                        content = newContent;
                        across = fileData.cursor_x;
                    }
                    break;
                default:
                    if (isprint(choice))
                    {
                        // sprintf(path, "%s, %02X", path, choice);
                        int cursorPos = cursorToCharPos(content, fileData.cursor_x, fileData.cursor_y);
                        char *newContent = malloc(strlen(content) + 2);
                        strncpy(newContent, content, cursorPos);
                        newContent[cursorPos] = choice;
                        strncpy(newContent + cursorPos + 1, content + cursorPos, strlen(content) - cursorPos);
                        newContent[strlen(content) + 1] = '\0';
                        free(content);
                        content = newContent;
                        fileData.cursor_x++;
                        across = fileData.cursor_x;
                    }
                    break;
                }
                break;
            case 2:
                switch (choice)
                {
                case 27:
                    mode = 0;
                    break;
                case KEY_BACKSPACE:
                case 0x7F:
                    if (strlen(command) > 0)
                    {
                        char *newCommand = malloc(strlen(command));
                        strncpy(newCommand, command, strlen(command) - 1);
                        newCommand[strlen(command) - 1] = '\0';
                        free(command);
                        command = newCommand;
                    }
                    break;
                case '\n':

                    switch (command[0])
                    {
                    case 'w':
                        if (strlen(path) != 0)
                        {
                            FILE *fptr;
                            fptr = fopen(path, "w");
                            if (fptr != NULL)
                            {
                                fwrite(content, 1, strlen(content), fptr);
                                fclose(fptr);
                            }
                        }
                        if (strlen(command) == 1 || command[1] != 'q')
                            break;
                    case 'q':
                        running = 0;
                        break;
                    case 's':
                        if (strlen(command) > 1)
                        {
                            char *newPath = malloc(strlen(command) - 1);
                            strncpy(newPath, command + 1, strlen(command) - 1);
                            newPath[strlen(command) - 1] = '\0';
                            FILE *fptr;
                            fptr = fopen(newPath, "w");
                            if (fptr != NULL)
                            {
                                fwrite(content, 1, strlen(content), fptr);
                                fclose(fptr);
                            }
                            free(path);
                            path = newPath;
                        }
                        break;
                    case 'l':
                        if (strlen(command) > 2)
                        {
                            char *line = malloc(strlen(command) - 2);
                            strncpy(line, command + 2, strlen(command) - 2);
                            line[strlen(command) - 2] = '\0';
                            int lineNum = atoi(line);
                            int numLines = 0;
                            int numChars = strlen(content);
                            int i;
                            for (i = 0; i < numChars; i++)
                            {
                                if (content[i] == '\n')
                                {
                                    numLines++;
                                    if (numLines == lineNum)
                                    {
                                        break;
                                    }
                                }
                            }
                            fileData.cursor_y = numLines - 1;
                            fileData.cursor_x = 0;
                        }
                        break;
                    default:
                        break;
                    }

                    command = realloc(command, 1);
                    command[0] = '\0';
                    mode = 0;
                    break;
                default:
                    if (isprint(choice))
                    {
                        char *newCommand = malloc(strlen(command) + 2);
                        strncpy(newCommand, command, strlen(command));
                        newCommand[strlen(command)] = choice;
                        newCommand[strlen(command) + 1] = '\0';
                        free(command);
                        command = newCommand;
                    }
                    break;
                }
                break;
            default:
                break;
            }
        }
        if (!running)
        {
            break;
        }
        if (fileData.cursor_y < fileData.scroll)
        {
            fileData.scroll = fileData.cursor_y;
        }
        if (fileData.cursor_y - (LINES - 4) >= fileData.scroll)
        {
            fileData.scroll = fileData.cursor_y - (LINES - 4) + 1;
        }
        lineAndCharNum = getLineAndCharNum(content);
        fileData.numLines = lineAndCharNum.lineNum;
        fileData.numChars = lineAndCharNum.charNum;
        render(fileData); // Re-render the menu
    }

    free(content);
    free(command);
    // Clean up and exit ncurses
    endwin();

    return 0;
}
