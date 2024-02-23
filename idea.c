#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <stdlib.h>

const int globalDelay = 2; // set delay between next trys 
const int maxTryOpenFile = 3; // open the file for writing try "maxTryOpenFile" times
int programLimiter = 5; // define the Limit the Programm trys to scan the computer
int isRunning = 0; // Bool expression that escape the While loop ( do not Change to 1 )
int unexpectedExit = 0; // Bool yes or no | 0 or 1

void logAdd(char *text, char *host);

int main(void) {
    // get client hostname
    system("hostname > t.txt");
    FILE *fp = fopen("t.txt", "r");
    char hostname[255]; // hold 1 Line
    fgets(hostname, 255, fp); // read line 1
    fclose(fp);
    sleep(globalDelay);
    system("del t.txt");

    // scan section start
    logAdd("SFC Scan gestartet ...", hostname);

    int counter = 0;

    while (isRunning == 0 && counter < programLimiter) {
        //int sfc_result = system("sfc /scannow");
        int sfc_result = 0;

        if (sfc_result == 0) {
            // no damaged files found
            if (counter > 0) {
                logAdd("Gefundene Integritätsverletzungen wurde repariert", hostname);
            } else {
                logAdd("Keine Integritätsverletzungen gefunden", hostname);
            }
            isRunning ++;
        } else if (sfc_result == 1) {
            // damaged file found
            if (counter < programLimiter-1) {
               logAdd("ETWAS IST SCHIEFGELAUFEN! Wird Scan erneut ausgeführt", hostname);
            } else if (counter == programLimiter-1) {
                logAdd("Unerwarteter fehler. Abgebrochen", hostname);
                isRunning ++;
                unexpectedExit ++;
            }
        } else {
            // unkown return value from scan
            logAdd("ETWAS IST SCHIEFGELAUFEN! Unerwateter fehler", hostname);
            isRunning ++;
            unexpectedExit ++;
            return -1;
        }
        counter ++;
    }

    if (unexpectedExit == 1) {
        logAdd("PROGRAMM WURDE MIT UNERWARTETEN FEHLER BEENDET! Exitcode : 10", hostname);
        return 10;
    } else if (counter == 5) {
        logAdd("SCAN WURDE ABGEBROCHEN! LIMIT ERREICHT! Exitcode : 12", hostname);
        return 12;
    } else {
        logAdd("Scan erfolgreich beendet. Exitcode : 0", hostname);
        return 0;
    }    
}

void logAdd(char *text, char *host) {
    // Logfile handling
    //char *logfile = "\\\\stadtwbbaram\\dip$\\Log\\client_repair.log";
    printf("TEST : %s\n", getenv("TEST"));
    char *logfile = getenv("LOGPATH");

    for(int i = 0; i < maxTryOpenFile; i++) {
        FILE *fp = fopen(logfile, "a");

        if (fp == NULL) {
            // cant open file
            printf("Error opening the file %s\n", logfile);
            // wait x secconds until next try
            sleep(globalDelay);
            if (i == maxTryOpenFile-1) printf("Limit reached. No log entry created\n");
        } else {
            // set timestamp
            const time_t t = time(NULL);
            struct tm tm = *localtime(&t);
            // create log
            fprintf(fp, "%d-%02d-%02d %02d:%02d - { %s", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, host);
            fprintf(fp, "                           %s }\n", text);
            fclose(fp);
            i = maxTryOpenFile;
        }
    }
}
