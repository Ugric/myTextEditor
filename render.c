#include "openFile.h"

#include <ncurses.h>
#include <string.h>
#include <stdio.h>

void render(struct FileData fileData) {
    clear();
    
    // Display Header
    printw("%s\n", fileData.path);
    int screenWidth = getmaxx(stdscr);
    for (int i = 0; i < screenWidth; i++) {
        printw("-");
    }

    // Display content
    int widthOfLineNumText = 4;
    int contentBeginX = widthOfLineNumText + 3;
    int contentBeginY = 2;
    int contentHeight = LINES - 4;

    int LineNum = 1;
    int newLine = 1;
    for (int i = 0; i < strlen(fileData.content); i++) {
        if (newLine) {
            printw("%4d | ", LineNum);
            LineNum++;
            newLine = 0;
        }
        if (fileData.content[i] == '\n') {
            newLine = 1;
        }
        printw("%c", fileData.content[i]);
    }
    

    // footer
    move(LINES - 2, 0);
    for (int i = 0; i < screenWidth; i++) {
        printw("-");
    }
    int numLines = 0;
    int numChars = 0;
    for (int i = 0; i < strlen(fileData.content); i++) {
        if (fileData.content[i] == '\n') {
            numLines++;
            numChars = 0;
        } else {
            numChars++;
        }
    }
    printw("Lines: %d, Chars: %d, Cursor: %d, %d", numLines, numChars, fileData.cursor_x, fileData.cursor_y);
    
    move(contentBeginY, contentBeginX);

    refresh();
}