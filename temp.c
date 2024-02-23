#include <stdio.h>
#include <time.h>
#include <stdlib.h>

char *getenv(const char *varname);

int main(void) {
    printf("Test : %s\n", getenv("TEST"));

    system("pause");
    return 0;
}