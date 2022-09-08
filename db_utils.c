//
//  db_utils.c
//  
//  Working with database
//  Created by Michael Pastushkov on 9/6/22.
//

#include "tracker.h"

MYSQL *con;
char *host = "localhost";
char *user = "root";
char *pass = "";
char *db = "p4pn";

int db_open() {
    con = mysql_init(NULL);
    if (con == NULL) {
        fprintf(stderr, "%s\n", mysql_error(con));
        return -1;
    }

    if (mysql_real_connect(con, host, user, pass, db, 0, NULL, 0) == NULL) {
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
    
    //puts(query);
    
    if (mysql_query(con, query)) {
        fprintf(stderr, "%s\n", mysql_error(con));
        return -1;
    }

    return 0;
}
