#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

// Global Variables
const int globalDelay = 2; // set delay between next trys 
const int maxTryOpenFile = 3; // open the file for writing try "maxTryOpenFile" times
const char * logfile = "C:\\Users\\bscholz\\My Stuff\\MyVSC\\GitHub\\SFC-Scann\\sfc.log";
const char buffer[13]; // global Buffer used for getHostname {13 letter long }
char hostname[sizeof(buffer)];


void getHost(void);
void logAdd(char *text, char host[sizeof(buffer)]);


int main(void) {
    getHost();
    // scan section start
    logAdd("SFC Scan gestartet ...", hostname);
}

void getHost(void) {
    // get client hostname
    system("hostname > t.txt");
    
    char fileText[sizeof(buffer)];
    FILE *fp;
    fp=fopen("t.txt","r");
    fgets(fileText,sizeof(buffer),fp);
    fclose(fp);

    system("del t.txt");;

    strcpy(hostname, fileText);
}

void logAdd(char *message, char host[sizeof(buffer)]) {
    // Logfile handling
    for(int i = 0; i < maxTryOpenFile; i++) {
        FILE *pFp = fopen(logfile, "a");

        if (pFp == NULL) {
            // cant open file
            printf("Error opening the file %s\n", logfile);
            // wait x secconds until next try
            sleep(globalDelay);
            if (i == maxTryOpenFile-1) {
                printf("Limit reached. No log entry created\n");
            }
        } else {
            // set timestamp
            const time_t t = time(NULL);
            struct tm tm = *localtime(&t);
            // create log
            fprintf(pFp, "%d-%02d-%02d %02d:%02d - { %s } - { %s }", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, host, message);
            fclose(pFp);
            i = maxTryOpenFile;
        }
    }
}
