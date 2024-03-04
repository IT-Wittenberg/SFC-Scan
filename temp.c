<<<<<<< HEAD
#include <stdio.h>
#include <time.h>
#include <stdlib.h>

const char buffer[4];

int strcmp(char str1[sizeof(buffer)], char str2[sizeof(buffer)]);

int main(void) {
    int array[4];
    for (int i = 0; i<4; i++) {
        array[i] = i;
        printf("%i", array[i]);
    }
    printf("\n");

    int reverseArray[4];

    int i = 0;
    for (int j = 3; j>=0; j--) {
        reverseArray[i] = array[j];
        printf("%i", reverseArray[i]);
        i++;
    }

    for (int k = 0; k<0; k++) {
        printf("%i", reverseArray[k]);
    }

    printf("\n");

    system("pause");
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
