//
//  proc.c
//
//  Data processing
//
//  Created by Michael Pastushkov on 9/7/22.
//

#include "tracker.h"

#define CLIENT_LIST "CLIENT_LIST"
#define TIME_FORMAT "%Y-%m-%d %H:%M:%S"

int check_session(char *cn, char *tf) {

    char query[1024];
    int num;
    sprintf(query, "SELECT count(*) FROM sessions WHERE cn = '%s' AND stime = '%s'", cn, tf);
    
    db_query(query);

    MYSQL_RES *result = mysql_store_result(con);
    MYSQL_ROW row = mysql_fetch_row(result);
    num = atoi(row[0]);
    return num;
}

int record_session(char *cn, long bin, long bout, time_t stime, char *ip4, char *source) {
    
    char tf[32], tfn[32];
    char query[1024];
    time_t now;

    time(&now);
    strftime(tf, sizeof(tf), TIME_FORMAT, localtime(&stime));
    strftime(tfn, sizeof(tfn), TIME_FORMAT, localtime(&now));

    if (check_session(cn, tf)) {
        // Update session numbers
        sprintf(query, "UPDATE sessions SET bin=%ld, bout=%ld, \
                etime='%s', ip4='%s', source='%s' \
                WHERE cn = '%s' AND stime = '%s'",
                bin, bout, tfn, ip4, source, cn, tf);
        if (db_query(query) != 0)
            return -1;
    } else {
        // New session
        sprintf(query, "INSERT INTO sessions (cn, bin, bout, stime, etime, ip4, source) \
                VALUES ('%s', %ld, %ld, '%s', '%s', '%s', '%s')",
                cn, bin, bout, tf, tfn, ip4, source);
        if (db_query(query) != 0)
            return -1;
    }
    
    return 0;
}

int proc_line(char *line, char *source) {

    int i, j, e = 0;
    char *s = line;
    long bin = 0, bout = 0;
    time_t stime = 0;
    char *cn = NULL;
    char *ip4 = "";

    printf(" proc_line %s\n", line);

    if (strncmp(CLIENT_LIST, line, strlen(CLIENT_LIST)) != 0) {
        return -1;
    }
    
    int len = strlen(line);
    for (i=0; i<len; i++) {
        if (line[i] == ',') {
            line[i] = 0;
            switch (e) {
                case 1:
                    cn = s;
                    break;
                case 2:
                    ip4 = s;
                    for (j=0; ip4[j]; j++) { // filter out port
                        if (ip4[j] == ':') {
                            ip4[j] = 0;
                            break;
                        }
                    }
                    break;
                 case 5:
                    bin = atol(s);
                    break;
                case 6:
                    bout = atol(s);
                    break;
                case 8:
                    stime = (time_t)atol(s);
                    break;
            }
            s = line + i + 1;
            e++;
        }

    }

    if (cn) {
        if (record_session(cn, bin, bout, stime, ip4, source) != 0)
            return -1;
    }

    return 0;
}
