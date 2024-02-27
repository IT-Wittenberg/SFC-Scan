#include <stdio.h>
#include <time.h>
#include <stdlib.h>

const char buffer[10];

int strcmp(char str1[sizeof(buffer)], char str2[sizeof(buffer)]);

int main(void) {
    char str1[sizeof(buffer)] = "ABCDEF";
    char str2[sizeof(buffer)] = "ABCDEFG";

    if (strcmp(str1, str2) == 1) {
        printf("Stimmen nicht ueberein :(");
    } else {
        printf("Stimmen ueberein :)");
    }

    return 0;
}

int strcmp(char str1[sizeof(buffer)], char str2[sizeof(buffer)]) {
    for (int i = 0; i < sizeof(buffer); i++) {
        if (str1[i] != str2[i]) {
            return 1;
        }
    }
    return 0;
}