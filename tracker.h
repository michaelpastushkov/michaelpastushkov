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
#include <unistd.h>
#include <mysql/mysql.h>

#define MAX_REMOTE_HOSTS 4

extern MYSQL *con;
extern char db_host[32];
extern char db_user[32];
extern char db_pass[32];
extern char db_schema[32];

extern char *status_log;
extern char *mhost;
extern int port;

typedef struct {
    char host[32];
    int port;
} remote_host;
extern remote_host remote_hosts[];

int db_open();
void db_close();
int db_query(char *query);

int read_local();
int read_remote();
int proc_line(char *line, char *source);
int read_config();
int get_ip4_info(char *ip4, char *info);

void remove_char(char* s, char c);

#endif /* tracker_h */
