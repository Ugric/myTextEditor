#include <stdio.h>
#include <unistd.h>
#include <termios.h>
#include <sys/ioctl.h>
#include <stdio.h>
#include <unistd.h>
#include "openFile.h"

int main(int argc, char *argv[])
{
    char *path = "";
    if (argc > 1)
    {
        path = argv[1];
    }
    return openFile(path);
}