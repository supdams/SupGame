#ifndef MANGOSSERVER_PROGRESSBAR_H
#define MANGOSSERVER_PROGRESSBAR_H

#include <stdio.h>

class barGoLink
{
    char const * empty;
    char const * full;

    int rec_no;
    int rec_pos;
    int num_rec;
    int indic_len;
    bool fast_update;

    public:

        void step( void );
        barGoLink( int, bool );
        ~barGoLink();
};
#endif
