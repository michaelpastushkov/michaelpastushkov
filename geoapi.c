//
//  geoapi.c
//  
//
//  Created by Michael Pastushkov on 9/10/22.
//
// Service: https://ipinfo.io/account/home
//

#include "tracker.h"

#include <GeoIP.h>
#include <GeoIPCity.h>

char geoip_data[256] = "../geoip-data/GeoIPCity.dat";
int geoip_on = 1;

static const char *str (const char *p) { return p ? p : ""; }

int get_ip4_info(session *ses) {
    
    static GeoIP *gi = 0;
    GeoIPRecord *gir;

    if (!gi)
        gi = GeoIP_open(geoip_data, GEOIP_INDEX_CACHE);

    if (gi == NULL) {
        log_printf("error opening geoip database: %s\n", geoip_data);
        log_printf("switching geoip off until restart (check config)\n");
        geoip_on = 0;
        return -1;
    }

    gir = GeoIP_record_by_name(gi, (const char *)ses->ip4);
    if (gir != NULL) {
        strncpy(ses->country, str(gir->country_code), sizeof(ses->country));
        strncpy(ses->city, str(gir->city), sizeof(ses->city));
        remove_char(ses->city, '\'');
        
        //log_printf("%s: country: %s, city: %s\n", ses->ip4, ses->country, ses->city);
    }
               
    //GeoIP_delete(gi);

    return 0;
}
