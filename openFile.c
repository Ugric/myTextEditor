#include "openFile.h"
#include "config.h"
#include "render.h"

#include <ncurses.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

int openFile(char* path, int mode) {
    char* content = "";
    
    if (strlen(path) != 0) {
        FILE *fptr;
        fptr = fopen(path, "r");
        if(fptr != NULL) {
            // Read the contents of the file into the content variable
            fseek(fptr, 0, SEEK_END);
            long fsize = ftell(fptr);
            fseek(fptr, 0, SEEK_SET);
            content = malloc(fsize + 1);
            fread(content, 1, fsize, fptr);
            content[fsize] = 0;
            fclose(fptr);
        }
    }

    struct FileData fileData = {path, content, 0, 0, 0};


    initscr();
    cbreak();
    keypad(stdscr, TRUE);
    
    render(fileData);

    int choice;
    while (1) {
        choice = getch(); // Get user input
        render(fileData); // Re-render the menu
    }

    end:
    free(content);
    // Clean up and exit ncurses
    endwin();

    return 0;
}
