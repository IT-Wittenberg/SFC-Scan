counter 0

start 
sfc.exe /scannow

chkdsk /r /f -y + Reboot -> set boot flag 1

if boot flag 1
    set flag 0
    sfc.exe /scannow
    exit success
else
    if counter != 2
        goto start
        set counter 1
    else
        breake
        return error

