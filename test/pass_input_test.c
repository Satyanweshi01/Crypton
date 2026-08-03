#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Detect operating system at compile time
#if defined(_WIN32) || defined(_WIN64)
#define PLATFORM_WINDOWS 1
#include <conio.h>
#else
#define PLATFORM_WINDOWS 0
#endif

#include <stdlib.h>

void clearConsole()
{
    if (PLATFORM_WINDOWS)
    {
        system("cls");
    }
    else
    {
        system("clear");
    }
}

static void read_password(const char *prompt, char *buffer, size_t size)
{
    printf("%s", prompt);
    fflush(stdout);

    // 1. Enter Raw/No-Echo Mode depending on OS
    if (!PLATFORM_WINDOWS)
    {
        system("stty -icanon -echo");
    }

    size_t i = 0;
    int ch;

    while (i < size - 1)
    {
        // 2. Fetch the character without automatic echoing
        if (PLATFORM_WINDOWS)
        {
            ch = _getch(); // Windows specific raw char fetch
        }
        else
        {
            ch = getchar(); // Unix raw char fetch (since stty altered behavior)
        }

        // Handle Enter key (Windows uses \r, Unix uses \n)
        if (ch == '\n' || ch == '\r' || ch == EOF)
        {
            break;
        }

        // Handle Backspace (ASCII 8 or 127)
        if (ch == 127 || ch == '\b')
        {
            if (i > 0)
            {
                i--;
                printf("\b \b");
                fflush(stdout);
            }
            continue;
        }

        buffer[i++] = (char)ch;
        printf("#");
        fflush(stdout);
    }

    buffer[i] = '\0';

    // 3. Restore terminal mode
    if (!PLATFORM_WINDOWS)
    {
        system("stty sane");
    }

    printf("\n");
}

int main()
{
    char buffer[32];

    clearConsole();
    read_password("Password: ", buffer, sizeof(buffer));

    printf("%s", buffer);
}
