#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <stdlib.h>

const int globalDelay = 2; // set delay between next trys 
const int maxTryOpenFile = 3; // open the file for writing try "maxTryOpenFile" times
int programLimiter = 5; // define the Limit the Programm trys to scan the computer
int isRunning = 0; // Bool expression that escape the While loop ( do not Change to 1 )
int unexpectedExit = 0; // Bool yes or no | 0 or 1
int counter = 0; // Counting

void logAdd(char *text, char *host);

int main(void) {
    printf("PATH : %s\n", getenv("PATH"));
    // get client hostname
    system("hostname > t.txt");
    FILE *pFp = fopen("t.txt", "r");
    char hostname[255]; // hold 1 Line
    fgets(hostname, 255, pFp); // read line 1
    fclose(pFp);
    sleep(globalDelay);
    system("del t.txt");

    // scan section start
    logAdd("SFC Scan gestartet ...", hostname);

    int programLimiter = 5; // define the Limit the Programm trys to scan the computer
    int isRunning = 0; // Bool expression that escape the While loop ( do not Change to 1 )
    int unexpectedExit = 0; // Bool yes or no | 0 or 1
    int counter = 0; // Counting

    // Pointer declaration
    int *pProgramLimiter = &programLimiter, 
    *pIsRunning = &isRunning, 
    *pUnexpectedExit = &unexpectedExit, 
    *pCounter = &counter;

    while (*pIsRunning == 0 && *pCounter < *pProgramLimiter) {
        int sfc_result = system("sfc /scannow");
        //int sfc_result = 0;
        int *pSfc_result = &sfc_result;
        if (*pSfc_result == 0) {
            // no damaged files found
            if (*pCounter > 0) {
                logAdd("Gefundene Integritätsverletzungen wurde repariert", hostname);
            } else {
                logAdd("Keine Integritätsverletzungen gefunden", hostname);
            }
            *pIsRunning ++;
        } else if (*pSfc_result == 1) {
            // damaged files found
            if (*pCounter < *pProgramLimiter-1) {
               logAdd("ETWAS IST SCHIEFGELAUFEN! Wird Scan erneut ausgeführt", hostname);
            } else if (*pCounter == *pProgramLimiter-1) {
                logAdd("Unerwarteter fehler. Abgebrochen", hostname);
                *pIsRunning ++;
                *pUnexpectedExit ++;
            }
        } else {
            // unkown return value from scan
            logAdd("ETWAS IST SCHIEFGELAUFEN! Unerwateter fehler", hostname);
            *pIsRunning ++;
            *pUnexpectedExit ++;
            return -1;
        }
        *pCounter ++;
    }

    if (*pUnexpectedExit == 1) {
        logAdd("PROGRAMM WURDE MIT UNERWARTETEN FEHLER BEENDET! Exitcode : 10", hostname);
        return 10;
    } else if (*pCounter == 5) {
        logAdd("SCAN WURDE ABGEBROCHEN! LIMIT ERREICHT! Exitcode : 12", hostname);
        return 12;
    } else {
        logAdd("Scan erfolgreich beendet. Exitcode : 0", hostname);
        return 0;
    }    
}

void logAdd(char *text, char *host) {
    // Logfile handling
    char *logfile = "D:\\Dokumente\\VisualStudio\\GitHub\\SFC-Scann\\sfc.log";
    // char *logfile = getenv("LOGPATH");

    for(int i = 0; i < maxTryOpenFile; i++) {
        FILE *pFp = fopen(logfile, "a");

        if (pFp == NULL) {
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
            fprintf(pFp, "%d-%02d-%02d %02d:%02d - { %s", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, host);
            fprintf(pFp, "                           %s }\n", text);
            fclose(pFp);
            i = maxTryOpenFile;
        }
    }
}
