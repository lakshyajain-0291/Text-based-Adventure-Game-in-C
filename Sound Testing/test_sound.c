#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

void sigint_handler(int sig) {
    system("killall afplay");
    exit(0);
}

int main() {
    const char *soundFilePath = "Smooth_funky.mp3";

    char command[300];
    snprintf(command, sizeof(command), "afplay \"%s\" &", soundFilePath);

    signal(SIGINT, sigint_handler);

    system(command);

    while (1) {
    }
    return 0;
}
