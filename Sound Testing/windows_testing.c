#include <windows.h>
#include <mmsystem.h>

int main() {
    // Compile the program with linking against the winmm library
    system("gcc windows_testing.c -o windows_testing -lwinmm");

    // Execute the compiled program
    system(".\\windows_testing.exe");

    // Path to your sound file
    const char *soundFilePath = "Explosion.mp3"; // Adjusted path

    // Play the sound file
    PlaySound(soundFilePath, NULL, SND_FILENAME | SND_ASYNC);

    // Keep the program running to allow the sound to play
    while (1) {
        Sleep(100); // Sleep for a short time to reduce CPU usage
    }

    return 0;
}
