//
//  local.c
//  
//  Working with local log files
//
//  Created by Michael Pastushkov on 9/6/22.
//

#include "tracker.h"

char status_log[256] = "sample-data/status.log";

int read_local() {

    char str[1024];
    int count = 0;

    FILE *fp = fopen(status_log, "r");
    if (!fp) {
        printf("can't read status file %s\n", status_log);
        return -1;
    }

    while (fgets (str, sizeof(str), fp) != NULL ) {
        if (proc_line(str, status_log) == 0)
            count ++;
    }

    printf("sessions recorded: %i\n", count);

    fclose(fp);
 
    return 0;
}

