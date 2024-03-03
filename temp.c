#include <stdio.h>
#include <time.h>
#include <stdlib.h>

const char buffer[10];

int strcmp(char str1[sizeof(buffer)], char str2[sizeof(buffer)]);

int main(void) {
    int num[1];
    float numf[1];

    for (int i = 0; i<sizeof(num); i++) {
        scanf("%i", &num[i]);
    }

    printf("%d\n", num[0]);
    printf("%d\n", num[1]);

    return num[1];
}

int strcmp(char str1[sizeof(buffer)], char str2[sizeof(buffer)]) {
    for (int i = 0; i < sizeof(buffer); i++) {
        if (str1[i] != str2[i]) {
            return 1;
        }
    }
    return 0;
}