//
//  alerts.c
//  
//
//  Created by Michael Pastushkov on 9/13/22.
//

#include "tracker.h"
#include <sys/socket.h>

#define MIB_DIV (1024 * 1024)

int daily_limit_mib = 1024;

int kill_cn(char *cn, char *source) {

    int sockfd;
    char kill_cmd[256];
    char line[256];
    remote_host *rh;
    
    if (strcmp("UNDEF", cn) == 0) {
        return -2;
    }
    
    sprintf(kill_cmd, "kill %s\n", cn);
    
    rh = get_host_by_source(source);
    if (!rh)
        return -1;
    
    sockfd = connect_remote(rh->host, rh->port);
    if (sockfd <= 0) {
        log_printf("error connecting to %s:%i\n", rh->host, rh->port);
        return -1;
    }
    
    write (sockfd, kill_cmd, strlen(kill_cmd));

    memset(line, 0, sizeof(line));
    read_line(sockfd, line, sizeof(line)); // telnet greeting
    memset(line, 0, sizeof(line));
    read_line(sockfd, line, sizeof(line)); // actual status line

    if (strstr(line, "SUCCESS:") && strstr(line, cn)) {
        log_printf("connection %s on %s killed\n", cn, source);
    } else {
        log_printf("connection %s on %s NOT killed\n", cn, source);
        log_printf("%s\n", line);
    }
    
    close_remote(sockfd);
    
    return 0;
}

int check_alerts() {
    
    char query[1024];
    char query2[1024];
    char details[256];
    char killed[128] = "";
    MYSQL_RES *result;
    MYSQL_ROW row;
    char *cn;
    long sbout;
    double mib;
    int alert_count = 0;
    char *source;

    sprintf(query,
            "SELECT cn, sum(bout), source as sbout FROM sessions\
            where now() - etime < 60*60*24\
            GROUP BY cn, source ORDER BY sbout DESC"
            );
    
    if (db_query(query) != 0) {
        return -1;
    }

    result = mysql_store_result(con);
    if (!result) {
        log_printf("SQL results error");
        return -1;
    }
    
    while ((row = mysql_fetch_row(result))) {
        
        cn = row[0];
        sbout = atol(row[1]);
        source = row[2];
        mib = sbout / MIB_DIV;
        killed[0] = 0;

        //log_printf("mib: %.2f, %ld\n", mib, sbout);
        
        if (mib > daily_limit_mib) {

            //log_printf("alert: mib: %.2f, %i\n", mib, daily_limit_mib);
            
            if (kill_cn(cn, source) == 0) {
                sprintf (killed, "connection on %s killed", source);
            }
            
            sprintf(details,
                    "Daily traffic limit %i MiB exceeded (%.2f) %s",
                    daily_limit_mib, mib, killed);
            sprintf(query2, "INSERT INTO alerts (cn, details) VALUES ('%s', '%s')",
                    cn, details);
            db_query(query2);
            
            alert_count++;

        }
        
    }
    mysql_free_result(result);
    
    log_printf("alerts: %i\n", alert_count);
    
    return 0;
}
