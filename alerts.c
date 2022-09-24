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

int kill_cn(char *cn, char *ip, char *port, char *source) {

    int sockfd;
    char kill_cmd[256];
    char line[256];
    remote_host *rh;
    
    if (strcmp("UNDEF", cn) == 0) {
        sprintf(kill_cmd, "kill %s:%s\n", ip, port);
    } else {
        sprintf(kill_cmd, "kill %s\n", cn);
    }
    
    rh = get_host_by_source(source);
    if (!rh)
        return -1;
    
    sockfd = connect_remote(rh->host, rh->port);
    if (sockfd <= 0) {
        log_printf(0, "error connecting to %s:%i\n", rh->host, rh->port);
        return -1;
    }
    
    write (sockfd, kill_cmd, strlen(kill_cmd));

    memset(line, 0, sizeof(line));
    read_line(sockfd, line, sizeof(line)); // telnet greeting
    memset(line, 0, sizeof(line));
    read_line(sockfd, line, sizeof(line)); // actual status line

    close_remote(sockfd);

    if (strstr(line, "SUCCESS:") &&
        (strstr(line, cn) || strstr(line, ip))) {
        return 0;
    } else {
        log_printf(3, "%s\n", line);
        return -1;
    }
}

int check_alerts() {
    
    char query[1024];
    char query2[1024];
    char details[256];
    MYSQL_RES *result;
    MYSQL_ROW row;
    char *cn;
    char *ip;
    char *port;
    long sbout;
    double mib;
    int alert_count = 0;
    char *source;
    
    sprintf(query,
        "SELECT max(cn), ip4, port, source, sum(bout) as sbout\
            FROM sessions\
            WHERE now() - etime < 60*60*24\
            GROUP BY ip4, port, source"
        );
    
    if (db_query(query) != 0) {
        return -1;
    }

    result = mysql_store_result(con);
    if (!result) {
        log_printf(0, "SQL results error");
        return -1;
    }
    
    while ((row = mysql_fetch_row(result))) {
        
        cn = row[0];
        ip = row[1];
        port = row[2];
        source = row[3];
        sbout = atol(row[4]);

        mib = sbout / MIB_DIV;

        if (mib > daily_limit_mib) {
            if (kill_cn(cn, ip, port, source) == 0) {
                sprintf(details, "connection killed: %s, %s (mib: %.2f)\n", cn, ip, mib);
                log_printf(1, details);
                sprintf(query2, "INSERT INTO alerts (details) VALUES ('%s')", details);
                db_query(query2);
                alert_count++;
            }
        }
        
    }
    mysql_free_result(result);
    
    log_printf(2, "alerts: %i\n", alert_count);
    
    return 0;
}

