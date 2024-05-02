#include <stdio.h>
#include <unistd.h>
#include <termios.h>
#include <sys/ioctl.h>
#include <stdio.h>
#include <unistd.h>
#include "openFile.h"
#include <signal.h>

int main(int argc, char *argv[])
{
    signal(SIGINT, SIG_IGN);
    char *path = "";
    if (argc > 1)
    {
        path = argv[1];
    }
    return openFile(path);
}
