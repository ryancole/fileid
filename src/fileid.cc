#include <stdlib.h>
#include <node.h>
#include <v8.h>

#ifndef UNIX
    #define UNIX
    #define UNIX_64
#endif

#include <sccfi.h>
#include <sccerr.h>

using namespace v8;

struct fileid_baton {
    
    Local<Function> callback;
    VTBYTE path[1024];
    bool success;
    VTWORD id;
    VTBYTE name[128];
    uv_work_t req;
        
};

void fileid_identify (uv_work_t* req) {
    
    fileid_baton* baton = (fileid_baton*) req->data;
    
    strncpy((char*)baton->path, "/home/ryan/.bashrc", 18);
    
    // identify the specified file's type
    if (FIIdFileEx(IOTYPE_UNIXPATH, baton->path, FIFLAG_NORMAL, &baton->id, (char*)baton->name, 128) == 0)
        baton->success = true;
    
}

void fileid_cleanup (uv_work_t* req) {
    
    fileid_baton* baton = (fileid_baton*) req->data;
    
    // create details result object
    Local<Object> details = Object::New();
    
    // set detail properties
    details->Set(String::NewSymbol("id"), Number::New(baton->id));
    details->Set(String::NewSymbol("name"), String::New((char*)baton->name));
    
    // create callback params
    Local<Value> argv[2] = {
        
        Local<Value>::New(Null()),
        Local<Value>::New(details)
        
    };
    
    // execute callback function
    baton->callback->Call(Context::GetCurrent()->Global(), 2, argv);
    
}

Handle<Value> identify (const Arguments& args) {
    
    HandleScope scope;
    
    // instanciate data baton
    fileid_baton* baton = (fileid_baton*) malloc(sizeof(fileid_baton));
    
    // initialize baton properties
    baton->req.data = (void*) baton;
    baton->success = false;
    baton->callback = Local<Function>::Cast(args[1]);
    
    // initiate async work on thread pool
    uv_queue_work(uv_default_loop(), &baton->req, fileid_identify, fileid_cleanup);
    
    return scope.Close(Undefined());
    
}

void initialize (Handle<Object> target) {
    
    // initialize file id engine
    if (FIInit() != SCCERR_OK)
        ThrowException(Exception::TypeError(String::New("Failed to initialize file id engine")));
    
    // export identify function
    target->Set(String::NewSymbol("identify"), FunctionTemplate::New(identify)->GetFunction());
    
}

NODE_MODULE(fileid, initialize)
