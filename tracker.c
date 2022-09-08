//  tracker.c
//
//  Open VPN Server tracker
//  Capture usage statistics aand recording sessions
//  in MySQL database for future analysis
//
//  Created by Michael Pastushkov on 9/5/22.
//

#include "tracker.h"

int main( int argc, char *argv[] ) {

    time_t now;
 
    time(&now);
    printf("ovpn-tracker: %s", ctime(&now));
    
    if (db_open() != 0) {
        printf("can't connect to database");
        return -1;
    }
    
    /*
    if (read_local() != 0) {
        printf("can't read %s\n", status_log);
        return -1;
    }
    */
     
    read_remote();
     
    db_close();
    
    return 0;
}
