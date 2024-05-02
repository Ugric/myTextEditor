#include "openFile.h"
#include "config.h"
#include "render.h"
#include "cursorMovement.h"
#include "lengths.h"

#include <ncurses.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <locale.h>

int openFile(char *Path)
{
    char *path = malloc(strlen(Path) + 1);
    char *message = malloc(1);
    message[0] = '\0';
    strcpy(path, Path);
    ESCDELAY = 0;
    char *content = malloc(1);
    content[0] = '\0';

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
    char *content_COPY = malloc(strlen(content) + 1);
    strcpy(content_COPY, content);
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
    char *newContent;
    int running = 1;
    while (1)
    {
        choice = getch(); // Get user input
        // sprintf(path, "%s, %02X", path, choice);
        int stop = 0;
        if (mode != 2)
        {
            cursorMovement(choice, &fileData, &content, &across, &stop);
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
                case 9:
                    newContent = malloc(strlen(content) + 2);
                    strncpy(newContent, content, cursorToCharPos(content, fileData.cursor_x, fileData.cursor_y));
                    newContent[cursorToCharPos(content, fileData.cursor_x, fileData.cursor_y)] = ' ';
                    newContent[cursorToCharPos(content, fileData.cursor_x, fileData.cursor_y) + 1] = ' ';
                    strncpy(newContent + cursorToCharPos(content, fileData.cursor_x, fileData.cursor_y) + 2, content + cursorToCharPos(content, fileData.cursor_x, fileData.cursor_y), strlen(content) - cursorToCharPos(content, fileData.cursor_x, fileData.cursor_y));
                    newContent[strlen(content) + 2] = '\0';
                    free(content);
                    content = newContent;
                    fileData.cursor_x += 2;
                    across = fileData.cursor_x;
                    break;
                case 0x17:
                case KEY_DC:
                    if (strlen(content) > 0)
                    {
                        int cursorPos = cursorToCharPos(content, fileData.cursor_x, fileData.cursor_y);
                        if (cursorPos == (int)strlen(content))
                        {
                            break;
                        }
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
                        if (strlen(command) > 2)
                        {
                            path = realloc(path, strlen(command) - 2);
                            strncpy(path, command + 2, strlen(command) - 2);
                            path[strlen(command) - 2] = '\0';
                            FILE *fptr;
                            fptr = fopen(path, "w");
                            if (fptr != NULL)
                            {
                                fwrite(content, 1, strlen(content), fptr);
                                fclose(fptr);
                                content_COPY = realloc(content_COPY, strlen(content) + 1);
                                strcpy(content_COPY, content);
                            }
                        }
                        break;
                    case 'o':
                        if (strlen(command) > 2)
                        {
                            char *newPath = malloc(strlen(command) - 2);
                            strncpy(newPath, command + 2, strlen(command) - 2);
                            newPath[strlen(command) - 2] = '\0';
                            FILE *fptr;
                            fptr = fopen(newPath, "r");
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
                                free(path);
                                path = newPath;
                            }
                            else
                            {
                                content = realloc(content, 1);
                                content[0] = '\0';
                            }
                            fileData = (struct FileData){&path, &content, &command, &mode, 0, 0, 0, 0, 0};
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
    free(content_COPY);
    free(path);
    free(message);
    // Clean up and exit ncurses
    endwin();

    return 0;
}
