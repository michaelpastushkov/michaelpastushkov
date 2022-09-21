//
//  utils.c
//  
//
//  Created by Michael Pastushkov on 9/8/22.
//

#include "tracker.h"

char *log_file = "/var/log/ovpn-tracker.log";
int log_level = 2;

void remove_char(char* s, char c) {
    char* d = s;
    do {
        while (*d == c) {
            d++;
        }
    } while ((*s++ = *d++));
}

int log_printf (int level, const char *format, ...)
{
    va_list arg;
    int done;

    if (level > log_level) {
        return 0;
    }
   
    va_start (arg, format);
    done = vprintf (format, arg);
    va_end (arg);

    return done;
}
