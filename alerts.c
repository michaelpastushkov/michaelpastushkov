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
int check_cn = 1;
int check_undef = 1;

int kill_cn(char *kill_cmd, char *source) {

    int sockfd;
    char line[256];
    remote_host *rh;
    
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

    if (!strstr(line, "SUCCESS:")) {
        log_printf(3, "%s\n", line);
        return -1;
    }
    
    return 0;
}


int record_alert(char *details) {
    char query[1024];
    log_printf(1, details);
    sprintf(query, "INSERT INTO alerts (details) VALUES ('%s')", details);
    return db_query(query);
}

int check_alerts_cn() {
    
    char query[1024];
    char details[256];
    char kill_cmd[256];
    MYSQL_RES *result;
    MYSQL_ROW row;
    char *cn;
    long sbout;
    int mib;
    int alert_count = 0;
    char *source;
    
    sprintf(query,
        "SELECT cn, source, sum(bout) as sbout\
            FROM sessions\
            WHERE etime > DATE_SUB(NOW(), INTERVAL 1 DAY)\
            AND cn != 'UNDEF'\
            GROUP BY cn, source");
    
    if (db_query(query) != 0) {
        return -1;
    }

    result = mysql_store_result(con);
    if (!result) {
        log_printf(0, "SQL results error\n");
        return -1;
    }
    
    while ((row = mysql_fetch_row(result))) {
        
        cn = row[0];
        source = row[1];
        sbout = atol(row[2]);
        mib = sbout / MIB_DIV;

        if (mib > daily_limit_mib) {
            sprintf(kill_cmd, "kill %s\n", cn);
            if (kill_cn(kill_cmd, source) == 0) {
                sprintf(details, "connection killed: %s (%i/%i MB)\n",
                    cn, mib, daily_limit_mib);
                record_alert(details);
                alert_count++;
            }
        }
        
    }
    mysql_free_result(result);
    
    return alert_count;
}

int kill_undefs(char *ip, char *source, int mib) {
    
    char query[1024];
    char details[256];
    MYSQL_RES *result;
    MYSQL_ROW row;
    char *port;
    int alert_count = 0;
    char kill_cmd[256];

    /* Get only most recent UNDEF sessions from the given IP / source,
       Others should have been killed earlier */
    sprintf(query,
        "SELECT port\
            FROM sessions\
            WHERE etime > DATE_SUB(NOW(), INTERVAL 10 MINUTE)\
            AND cn = 'UNDEF'\
            AND ip4 = '%s' \
            AND source = '%s'",
            ip, source);

    //printf("query: %s\n", query);
    
    if (db_query(query) != 0) {
        return -1;
    }

    result = mysql_store_result(con);
    if (!result) {
        log_printf(0, "SQL results error\n");
        return -1;
    }
    
    while ((row = mysql_fetch_row(result))) {
        port = row[0];
        sprintf(kill_cmd, "kill %s:%s\n", ip, port);
        if (kill_cn(kill_cmd, source) == 0) {
            sprintf(details, "connection killed: %s:%s (%i/%i MB)\n",
                ip, port, mib, daily_limit_mib);
            record_alert(details);
            alert_count++;
        }
    }
    mysql_free_result(result);
    return alert_count;
}

int check_alerts_undef() {
    
    char query[1024];
    MYSQL_RES *result;
    MYSQL_ROW row;
    char *ip;
    long sbout;
    int mib;
    int alert_count = 0;
    char *source;

    sprintf(query,
        "SELECT ip4, source, sum(bout) as sbout\
            FROM sessions\
            WHERE etime > DATE_SUB(NOW(), INTERVAL 1 DAY)\
            AND cn = 'UNDEF'\
            AND bout > 1024\
            GROUP BY ip4, source"
        );
    
    if (db_query(query) != 0) {
        return -1;
    }

    result = mysql_store_result(con);
    if (!result) {
        log_printf(0, "SQL results error\n");
        return -1;
    }
    
    while ((row = mysql_fetch_row(result))) {
        
        ip = row[0];
        source = row[1];
        sbout = atol(row[2]);
        mib = sbout / MIB_DIV;

        if (mib > daily_limit_mib) {
            int ret = kill_undefs(ip, source, mib);
            if (ret < 0)
                continue;
            alert_count += ret;
        }
        
    }
    mysql_free_result(result);
    
    return alert_count;
}


int check_alerts() {

    int ret = 0;
    
    if (check_cn) {
        ret += check_alerts_cn();
        if (ret < 0)
            return ret;
    }
    
    if (check_undef) {
        ret += check_alerts_undef();
        if (ret < 0)
            return ret;
    }
    
    log_printf(2, "alerts: %i\n", ret);
    
    return 0;
}
