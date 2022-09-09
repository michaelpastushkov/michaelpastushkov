//
//  utils.c
//  
//
//  Created by Michael Pastushkov on 9/8/22.
//

#include "tracker.h"

void remove_char(char* s, char c) {
    char* d = s;
    do {
        while (*d == c) {
            ++d;
        }
    } while ((*s++ = *d++));
}
