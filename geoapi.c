//
//  geoapi.c
//  
//
//  Created by Michael Pastushkov on 9/10/22.
//
// Service: https://ipinfo.io/account/home
//

#include "tracker.h"
#include <curl/curl.h>

#define MAX_ERRORS 20

char *api_host = "ipinfo.io";
char *api_token = "e0d4f45cec3323";


size_t write_data (void *ptr, size_t size, size_t nmemb, char *data) {
    
    //printf("received: %ld, %ld, %s\n", size, nmemb, ptr);
    
    memcpy (data, ptr, size * nmemb);
    return size * nmemb;
}


int get_ip4_info(char *ip4, char *info) {
    
    int i;
    char *p, *c;
    CURL *curl;
    CURLcode res;
    char url[128];
    char data[2048];
    char country[16], city[32];
    static int error_count = 0;
    
    if (error_count > MAX_ERRORS) {
        *info = 0;
        return 0;
    }

    sprintf(url, "%s/%s?token=%s", api_host, ip4, api_token);
    memset(data, 0, sizeof(data));
    memset(country, 0, sizeof(country));
    memset(city, 0, sizeof(city));

    curl = curl_easy_init();
    if (!curl) {
        printf("curl init error\n");
        return -1;
    }
    
    curl_easy_setopt(curl, CURLOPT_URL, url);
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &data);
    
    res = curl_easy_perform(curl);
    if (res != CURLE_OK) {
        fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
        return -1;
    }
    
    curl_easy_cleanup(curl);
    
    p = data;
    for (i=0; i<sizeof(data); i++) {
        if (data[i] == '\n') {
            data[i] = 0;

            remove_char(p, '"');
            remove_char(p, ',');
            remove_char(p, ' ');

            c = strchr(p, ':');
            if (c) {
                *c = 0;
                c++;
                if (strcmp("country", p) == 0) {
                    strncpy(country, c, sizeof(country));
                    error_count = 0;
                } else if (strcmp("city", p) == 0) {
                    strncpy(city, c, sizeof(city));
                } else if (strcmp("error", p) == 0) {
                    printf("error %i:\n", error_count++);
                    if (error_count > MAX_ERRORS) {
                        printf("geo api max error count, switching off until restart\n");
                        return 0;
                    }

                }
            }
            p = data + i + 1;
        }
    }

    sprintf(info, "%s, %s", country, city);
    
    return 0;
  }
