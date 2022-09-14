//
//  alerts.c
//  
//
//  Created by Michael Pastushkov on 9/13/22.
//

#include "tracker.h"

#define MIB_DIV (1024 * 1024)

int daily_limit_mib = 1024;

int check_alerts() {
    
    char query[1024];
    char query2[1024];
    char details[256];
    MYSQL_RES *result;
    MYSQL_ROW row;
    char *cn;
    long sbout;
    double mib;
    int alert_count = 0;
    
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
        mib = sbout / MIB_DIV;

        //printf("mib: %.2f, %ld\n", mib, sbout);

        if (mib > daily_limit_mib) {

            //printf("alert: mib: %.2f, %i\n", mib, daily_limit_mib);

            sprintf(details, "Daily traffic limit %i MiB exceeded (%.2f)",
                    daily_limit_mib, mib);
            sprintf(query2, "INSERT INTO alerts (cn, details) VALUES ('%s', '%s')",
                    cn, details);
            db_query(query2);
            alert_count++;

        }
        
    }
    mysql_free_result(result);
    
    printf("alerts: %i\n", alert_count);
    
    return 0;
}
