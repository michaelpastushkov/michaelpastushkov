//
//  alerts.c
//  
//
//  Created by Michael Pastushkov on 9/13/22.
//

#include "tracker.h"

long daily_limit = 1024 * 1024 * 1024;

int check_alerts() {
    
    char query[1024];
    char query2[1024];
    char details[256];
    MYSQL_RES *result;
    MYSQL_ROW row;
    char *cn;
    long sbout;
    
    sprintf(query,
            "SELECT cn, sum(bout) as sbout FROM sessions\
            where now() - etime < 60*60*24\
            GROUP BY cn ORDER BY sbout DESC"
            );
    
    if (db_query(query) != 0) {
        return -1;
    }

    result = mysql_store_result(con);
    if (!result) {
        printf("SQL results error");
        return -1;
    }
    
    while ((row = mysql_fetch_row(result))) {
        cn = row[0];
        sbout = atol(row[1]);
        
        //printf("cn: %s, bytes: %ld\n", cn, sbout);
        
        if (sbout > daily_limit) {
            sprintf(details, "Daily traffic linmit %ld exceeded (%ld)", daily_limit, sbout);
            sprintf(query2, "INSERT INTO alerts (cn, details) VALUES ('%s', '%s')", cn, details);
            db_query(query2);
        }
        
    }
    mysql_free_result(result);
    
    return 0;
}
