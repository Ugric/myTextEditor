#include <stdio.h>
#include <unistd.h>
#include <termios.h>

int main() {
    struct termios oldt, newt;
    char ch;

    // Get current terminal attributes
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;

    // Set terminal to raw mode (disable buffering and echoing)
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);

    char lastCh[10];
    int lastChSize = sizeof(lastCh);
    int lastChIndex = lastChSize - 1;

    // Listen for key presses
    while (1) {
        ch = getchar();
        printf("Key pressed: %02X\n", ch);

        switch (ch) {
            case 0x41:
            case 0x42:
            case 0x43:
            case 0x44:
                for (int i = 0; i < lastChSize; i++){
                    printf("%02X ", lastCh[i]);
                }
                printf("\n");
                printf("%02X %02X\n", lastCh[(lastChIndex+(lastChSize-1)) % lastChSize], lastCh[lastChIndex]);
                printf("%i %i\n", (lastChIndex+(lastChSize-1)) % lastChSize, lastChIndex);
                if (lastCh[(lastChIndex+(lastChSize-1)) % lastChSize] == 0x1B && lastCh[lastChIndex] == 0x5B){
                    
                }
                printf("Arrow key pressed %02X\n", ch);
                break;
            case 0x7F:
                printf("Backspace key pressed\n");
                break;
            case 0x0A:
                printf("Enter key pressed\n");
                break;
            default:
                break;
        }
        lastChIndex = (lastChIndex + 1) % lastChSize;
        lastCh[lastChIndex] = ch;
    }

    // Restore terminal attributes
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);

    return 0;
}