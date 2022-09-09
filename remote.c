//
//  sock.c
//  
//  Working with management interface via sockets
//  Created by Michael Pastushkov on 9/6/22.
//

#include "tracker.h"

#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>
#include <arpa/inet.h>

#define END_MARKER "END"

remote_host remote_hosts[MAX_REMOTE_HOSTS];

static char *status_cmd = "status\n";
char line[1024];

int read_line(int sockfd) {

    int n = 0;
    char b;

    while (read(sockfd, &b, 1) == 1) {
        if (n >= sizeof(line)) {
            line[n] = 0;
            break;
        } else if (b == '\r') {
            line[n] = 0;
        } else if (b == '\n') {
            break;
        } else {
            line[n++] = b;
        }
    }
    return n;
}

int connect_remote(char *mhost, int port) {
    
    int sockfd = 0;
    struct sockaddr_in serv_addr;
    struct addrinfo hints, *res;
    int line_count = 0;
    char addrstr[100];
    void *ptr;
    
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("error: could not create socket \n");
        return -1;
    }

    memset (&serv_addr, '0', sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(port);

    memset (&hints, 0, sizeof (hints));
    hints.ai_family = PF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags |= AI_CANONNAME;
    
    if (getaddrinfo(mhost, NULL, &hints, &res) != 0) {
        printf("getaddrinfo error\n");
        return -1;
    }
    
    inet_ntop (res->ai_family, res->ai_addr->sa_data, addrstr, sizeof(addrstr));
    ptr = &((struct sockaddr_in *) res->ai_addr)->sin_addr;
    inet_ntop (res->ai_family, ptr, addrstr, sizeof(addrstr));
    
    printf ("connecting to ovpn remote management: %s:%i %s\n",
            addrstr, port, res->ai_canonname);
    
    if (inet_pton(AF_INET, addrstr, &serv_addr.sin_addr)<=0) {
        printf("inet_pton error occured\n");
        return -1;
    }

    if (connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        printf("error: connect failed \n");
        return -1;
    }

    write (sockfd, status_cmd, strlen(status_cmd));
    
    while (read_line(sockfd)) {
        
        if (strcmp(line, END_MARKER) == 0) {
            break;
        }
        
        if (proc_line(line, mhost) == 0) {
            line_count++;
        }
    }
    close(sockfd);
    
    printf("lines processed: %i\n", line_count);
    
    return 0;
}

int read_remote() {
    
    int i;
    int conn_count = 0;
    
    for (i=0; i<MAX_REMOTE_HOSTS; i++) {
        if (remote_hosts[i].host[0] == 0)
            break;
        if (connect_remote(remote_hosts[i].host, remote_hosts[i].port) == 0) {
            conn_count++;
        }
    }

    printf("connections processed: %i\n", conn_count);
    
    return 0;
}
