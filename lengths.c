#include "lengths.h"

#include <string.h>
#include <stdio.h>

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

int indent(char *content, int cursor_y, int cursor_x)
{
    int numLines = 0;
    int numChars = strlen(content);
    int i = 0;
    if (cursor_y != 0)
    {
        for (; i < numChars; i++)
        {
            if (content[i] == '\n')
            {
                numLines++;
                if (numLines == cursor_y)
                {
                    break;
                }
            }
        }
        i++;
    }
    int indent = 0;
    for (int j = i; j < numChars; j++)
    {
        if (content[j] == ' ' && indent < cursor_x)
        {
            indent++;
        }
        else
        {
            break;
        }
    }
    return indent;
}

int cursorToCharPos(char *content, int cursor_x, int cursor_y)
{
    if (cursor_y == 0)
    {
        return cursor_x;
    }
    int numLines = 0;
    int numChars = strlen(content);
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
        }
    }
    return i;
}
