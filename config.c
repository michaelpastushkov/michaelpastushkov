//
//  config.c
//  
//
//  Created by Michael Pastushkov on 9/8/22.
//

#include "tracker.h"

static char *config_file = "ovpn-tracker.config";

int rh_index = 0; // non-kosher but easy way to handle multiple hosts

int parse_config_line(char *line) {
    
    char *n, *v, *p;
    
    remove_char(line, ' ');
    remove_char(line, '\t');
    remove_char(line, '\n');
    remove_char(line, '\r');
    
    if (line[0] == '#')
        return 0;
    
    n = line;
    v = strchr(line, ':');
    if (!v)
        return 0;
    
    *v++ = 0;
    if (strcmp(n, "db_host") == 0) {
        strncpy(db_host, v, sizeof(db_host));
    } else if (strcmp(n, "db_user") == 0) {
        strncpy(db_user, v, sizeof(db_user));
    } else if (strcmp(n, "db_pass") == 0) {
        strncpy(db_pass, v, sizeof(db_pass));
    } else if (strcmp(n, "db_schema") == 0) {
        strncpy(db_schema, v, sizeof(db_schema));
    } else if (strcmp(n, "status_log") == 0) {
        strncpy(status_log, v, sizeof(status_log));
    } else if (strcmp(n, "mode") == 0) {
        mode = strcmp(v, "local") == 0 ? MODE_LOCAL : MODE_REMOTE;
    } else if (strcmp(n, "repeat_time") == 0) {
        repeat_time = atoi(v);
    } else if (strcmp(n, "log_level") == 0) {
        log_level = atoi(v);
    } else if (strcmp(n, "daily_limit_mib") == 0) {
        daily_limit_mib = atoi(v);
    } else if (strcmp(n, "geoip") == 0) {
        if (strcmp(v, "on") == 0) {
            geoip_on = 1;
        }
    } else if (strcmp(n, "geoip_data") == 0) {
        strncpy(geoip_data, v, sizeof(geoip_data));
    } else if (strcmp(n, "remote_host") == 0) {
        if (rh_index >= MAX_REMOTE_HOSTS) {
            log_printf(0, "can't handle more than %i remote hosts\n", MAX_REMOTE_HOSTS);
            return -1;
        }
        p = strchr(v, ':');
        if (!p) {
            log_printf(0, "%s must be in format host:port\n", line);
            return -1;
        }
        *p++ = 0;

        strncpy(remote_hosts[rh_index].host, v, sizeof(remote_hosts[rh_index].host));
        remote_hosts[rh_index].port = atol(p);
        rh_index++;
    }

    return 0;
}

int read_config() {
    
    FILE *fp = fopen(config_file, "r");
    char line[1024];
    
    if (!fp) {
        log_printf(0, "can't read config file %s\n", config_file);
        return -1;
    }

    while (fgets (line, sizeof(line), fp) != NULL ) {
        if (parse_config_line(line) != 0) {
            return -1;
        }
    }

    return 0;
}


