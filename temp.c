#include <stdio.h>
#include <time.h>
#include <stdlib.h>

char *getenv(const char *varname);

int main(void) {
    printf("HOME : %s\n", getenv("HOME"));

    system("pause");
    return 0;
}