//
//  utils.c
//  
//
//  Created by Michael Pastushkov on 9/8/22.
//

#include "tracker.h"

char *log_file = "/var/log/ovpn-tracker.log";

void remove_char(char* s, char c) {
    char* d = s;
    do {
        while (*d == c) {
            d++;
        }
    } while ((*s++ = *d++));
}

int log_printf (const char *format, ...)
{
    va_list arg;
    int done;
    time_t now;
    char log_format[1024];
    char rec[1024];
    struct tm * timeinfo;
    char log_time [80];
    FILE *fp;
    
    time(&now);
    timeinfo = localtime(&now);
    strftime (log_time, sizeof(log_time), "%x %X", timeinfo);
    
    sprintf(log_format, "%s: %s", log_time, format);
    
    va_start (arg, format);
    done = vsprintf (rec, log_format, arg);
    va_end (arg);

    printf ("%s", rec);
    
    fp = fopen(log_file, "a+");
    if (fp) {
        fprintf (fp, "%s", rec);
        fclose (fp);
    } else {
        fprintf (stderr, "can't open log file %s\n", log_file);
        return -1;
    }
    
    return done;
}
