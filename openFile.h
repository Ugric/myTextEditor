#ifndef OPENFILE_H
#define OPENFILE_H

#include <ncurses.h>

struct FileData {
    char* path;
    char* content;
    int cursor_x;
    int cursor_y;
    int mode;
};

// Function to open a file
int openFile(char* path, int mode);

#endif // OPENFILE_H