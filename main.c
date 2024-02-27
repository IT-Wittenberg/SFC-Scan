#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

// Global Variables
const char * logfile = "C:\\Users\\bscholz\\My Stuff\\MyVSC\\GitHub\\SFC-Scann\\sfc.log";
const int scanLimiter = 1; // limit for retry scan { 1 means 1 retry}
const int fileLimiter = 3; // open the file for writing try "maxTryOpenFile" times
const char buffer[13]; // global Buffer used for getHostname {13 byte long }
char hostname[sizeof(buffer)];

// Prototypes
void getHost(void);
void logAdd(char host[sizeof(buffer)], char *text);
int sfc_scan(void);

int main(void) {
    getHost();
    
    // scan section start
    int sfc_state, counter=0; 
    logAdd(hostname, "SFC Scan gestartet ...");

    while (sfc_state != 0 && counter < scanLimiter) {
        sfc_state = sfc_scan();

        if (sfc_state != 0) {
            logAdd(hostname, "ETWAS IST SCHIEF GELAUFEN. SCAN STARTET ERNEUT");
            counter ++;
        } else if (sfc_state == 0) {
            logAdd(hostname, "Scan erfolgreich durchgelaufen");
            return sfc_state;
        }

        if (counter == scanLimiter) {
            logAdd(hostname, "LIMIT AN MAXIMALEN VERUSCHEN ERREICHT. SCAN ABGEBROCHEN");
            return sfc_state;
        }
    }

    logAdd(hostname, "ETWAS IST SCHIEF GELAUFEN. UNBEKANNTER FEHLER");
    return sfc_state;
}

void getHost(void) {
    // get client hostname
    system("hostname > t.txt");
    
    char fileText[sizeof(buffer)];
    FILE *fp;

    fp = fopen("t.txt","r");
    fgets(fileText,sizeof(buffer),fp);
    fclose(fp);

    system("del t.txt");;

    strcpy(hostname, fileText);
}

void logAdd(char host[sizeof(buffer)], char *message) {
    const int delay = 2; // set delay between next trys 
    const time_t t = time(NULL);
    const struct tm tm = *localtime(&t);
    
    FILE *fp;
    int openFile = 0;

    for (int i = 0; i<fileLimiter; i++) {
        fp = fopen(logfile, "a");
        if (fp != NULL) {
            openFile = 1;        
            break;
        }
        if (i==fileLimiter) break;
        sleep(delay);
    }

    if (openFile == 1) {
        fprintf(fp, "%d-%02d-%02d %02d:%02d - { %s } - { %s }\n", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, host, message);
        fclose(fp);
    } else {
        printf("Datei konnte nicht geöffnet werden. Kein Logeintrag erstellt.");
    }
}

int sfc_scan(void) {
    int scan_result = system("sfc /scannow");
    return scan_result;
}