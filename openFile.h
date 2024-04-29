#ifndef OPENFILE_H
#define OPENFILE_H

#include <ncurses.h>

struct FileData
{
    char **path;
    char **content;
    int *mode;
    int cursor_x;
    int cursor_y;
    int scroll;
    int numLines;
    int numChars;
};

// Function to open a file
int openFile(char *path);

#endif // OPENFILE_H