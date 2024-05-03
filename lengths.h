#ifndef LENGTHS_H
#define LENGTHS_H

struct lineAndCharNum
{
    int lineNum;
    int charNum;
};

struct lineAndCharNum getLineAndCharNum(char *content);

int lineLength(char *content, int cursor_y);

int cursorToCharPos(char *content, int cursor_x, int cursor_y);

int indent(char *content, int cursor_y, int cursor_x);

#endif // LENGTHS_H
