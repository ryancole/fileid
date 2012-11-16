#ifndef _FILEID_H_
#define _FILEID_H_

#include <v8.h>

using namespace v8;

struct fileid_identify_baton {
    
    uv_work_t req;
    
    bool success;
    char* path;
    char* name;
    unsigned short id;
    Persistent<Function> callback;
        
};

#endif
