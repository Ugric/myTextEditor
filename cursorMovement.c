#include "cursorMovement.h"
#include "openFile.h"
#include "lengths.h"

#include <stdio.h>

void cursorMovement(char choice, struct FileData *fileData, char **content, int *across, int *stop)
{
    int cursorLineLength;
    int lineWidth;
    switch (choice)
    {
    case 3:
        if ((*fileData).cursor_y > 0)
        {
            (*fileData).cursor_y--;
            int cursorLineLength = lineLength(*content, (*fileData).cursor_y);
            (*fileData).cursor_x = *across;
            if ((*fileData).cursor_x > cursorLineLength)
            {
                (*fileData).cursor_x = cursorLineLength;
            }
        }
        *stop = 1;
        break;
    case 2:
        if ((*fileData).cursor_y < (*fileData).numLines - 1)
        {
            (*fileData).cursor_y++;
            int cursorLineLength = lineLength(*content, (*fileData).cursor_y);
            (*fileData).cursor_x = *across;
            if ((*fileData).cursor_x > cursorLineLength)
            {
                (*fileData).cursor_x = cursorLineLength;
            }
        }
        *stop = 1;
        break;
    case 4:
        if ((*fileData).cursor_x > 0)
        {
            (*fileData).cursor_x--;
            *across = (*fileData).cursor_x;
        }
        else if ((*fileData).cursor_y > 0)
        {
            (*fileData).cursor_y--;
            *across = lineLength(*content, (*fileData).cursor_y);
        }
        cursorLineLength = lineLength(*content, (*fileData).cursor_y);
        (*fileData).cursor_x = *across;
        if ((*fileData).cursor_x > cursorLineLength)
        {
            (*fileData).cursor_x = cursorLineLength;
        }
        *stop = 1;
        break;
    case 5:
        lineWidth = lineLength(*content, (*fileData).cursor_y);
        if (*across < lineWidth)
        {
            *across += 1;
        }
        else if ((*fileData).cursor_y < (*fileData).numLines - 1)
        {
            *across = 0;
            (*fileData).cursor_y++;
        }
        cursorLineLength = lineLength(*content, (*fileData).cursor_y);
        (*fileData).cursor_x = *across;
        if ((*fileData).cursor_x > cursorLineLength)
        {
            (*fileData).cursor_x = cursorLineLength;
        }
        *stop = 1;
        break;
    }
    return;
}
