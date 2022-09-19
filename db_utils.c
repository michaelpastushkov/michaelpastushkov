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
    
    printf("connecting MySQL: %s %s\n", db_host, db_schema);
        
    con = mysql_init(NULL);
    if (con == NULL) {
        fprintf(stderr, "%s\n", mysql_error(con));
        return -1;
    }

    if (mysql_real_connect(con, db_host, db_user, db_pass, db_schema, 0, NULL, 0) == NULL) {
        fprintf(stderr, "%s\n", mysql_error(con));
        mysql_close(con);
        return -1;
    }

    return 0;
}

void db_close() {
    mysql_close(con);
}

int db_query(char *query) {
    
    // printf("SQL: %s\n", query);
    
    if (mysql_query(con, query)) {
        fprintf(stderr, "%s\n", mysql_error(con));
        printf("SQL: %s\n", query);
        return -1;
    }

    return 0;
}
