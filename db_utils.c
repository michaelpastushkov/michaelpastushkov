//
//  db_utils.c
//  
//  Working with database
//  Created by Michael Pastushkov on 9/6/22.
//

#include "tracker.h"

MYSQL *con;
char db_host[32] = "localhost";
char db_user[32] = "root";
char db_pass[32] = "";
char db_schema[32] = "p4pn";

int db_open() {
    
    log_printf(1, "connecting MySQL: %s %s\n", db_host, db_schema);
        
    con = mysql_init(NULL);
    if (con == NULL) {
        log_printf(0, "%s\n", mysql_error(con));
        return -1;
    }

    if (mysql_real_connect(con, db_host, db_user, db_pass, db_schema, 0, NULL, 0) == NULL) {
        log_printf(0, "%s\n", mysql_error(con));
        mysql_close(con);
        return -1;
    }

    return 0;
}

void db_close() {
    mysql_close(con);
}

int db_query(char *query) {
    
    log_printf(3, "SQL: %s\n", query);
    
    if (mysql_query(con, query)) {
        log_printf(0, "MySQL error: %s\n%s\n", mysql_error(con), query);
        return -1;
    }

    return 0;
}

int db_cleanup() {

    int ret;
    char query[1024];

    sprintf(query, "DELETE FROM sessions WHERE etime < DATE_SUB(NOW(), INTERVAL 7 DAY)");
    ret = db_query(query);
    
    sprintf(query, "DELETE FROM alerts WHERE date < DATE_SUB(NOW(), INTERVAL 7 DAY)");
    ret += db_query(query);

    return ret;
}


