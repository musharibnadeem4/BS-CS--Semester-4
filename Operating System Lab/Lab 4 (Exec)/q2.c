#include <stdio.h>
#include <unistd.h>
#include <string.h>

int main() {
    char data[100];
    printf("Enter the command you want to run (ls, date, cal, exit): ");
    scanf("%s", data);

    if (strcmp(data, "ls") == 0) {
        execl("/bin/ls", "ls", NULL);
    } else if (strcmp(data, "date") == 0) {
        execlp("date", "date", NULL);
    } else if (strcmp(data, "cal") == 0) {
        char *args[] = {"cal", NULL};
        execv("/usr/bin/cal", args);
    } else if (strcmp(data, "exit") == 0) {
        execl("/bin/sh", "sh", "-c", "exit 0", NULL);
    } else {
        printf("Command Not Found\n");
    }

    return 0;
}
