#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

// Signal handler function
void sigint_handler(int sig) {
    // Stop audio playback when program receives SIGINT (Ctrl+C)
    system("killall afplay"); // Stops all afplay processes
    exit(0);
}

int main() {
    // Path to the sound file
    const char *soundFilePath = "Salaar_Music.m4a"; // Update with your audio file path
    
    // Construct the command to play the sound using afplay
    char command[300]; // Increase the buffer size to accommodate the longer command
    snprintf(command, sizeof(command), "afplay \"%s\" &", soundFilePath); // & to run in background
    
    // Register signal handler for SIGINT (Ctrl+C)
    signal(SIGINT, sigint_handler);

    // Play the sound using system command
    system(command);

    // Keep the program running until terminated by SIGINT
    while (1) {
        // Do nothing, let the signal handler handle program exit
    }
    // Press control C on mac to stop the music

    return 0;
}
