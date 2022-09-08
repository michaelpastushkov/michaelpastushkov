//
//  tracker.h
//  
//
//  Created by Michael Pastushkov on 9/5/22.
//

#ifndef tracker_h
#define tracker_h

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include <mysql/mysql.h>

extern MYSQL *con;
extern char *host;
extern char *user;
extern char *pass;
extern char *db;

extern char *status_log;
extern char *mhost;
extern int port;

int db_open();
void db_close();
int db_query(char *query);

int read_local();
int read_remote();
int proc_line(char *line, char *source);

#endif /* tracker_h */
