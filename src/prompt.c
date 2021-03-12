#include "prompt.h"

/*
 * Prints the prompt to the console
 */
void printPrompt() {
    time_t currentTime;
    char timeStringBuffer[26];
    struct tm* tm_info;

    currentTime = time(NULL);
    tm_info = localtime(&currentTime);

    strftime(timeStringBuffer, sizeof(timeStringBuffer), "[%d/%m %H:%M]", tm_info);
    
    printf("%s # ", timeStringBuffer);
}