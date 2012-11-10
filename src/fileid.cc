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
    
    bool success;
    
    VTWORD id;
    VTBYTE name[128];
    
    String::Utf8Value* path;
    Persistent<Function> callback;
    
    uv_work_t req;
        
};

void fileid_identify (uv_work_t* req) {
    
    fileid_baton* baton = (fileid_baton*) req->data;
    
    // identify the specified file's type
    if (FIIdFileEx(IOTYPE_UNIXPATH, **(baton->path), FIFLAG_NORMAL, &baton->id, (char*)baton->name, 128) == 0)
        baton->success = true;
    
}

void fileid_cleanup (uv_work_t* req) {
    
    HandleScope scope;
    
    fileid_baton* baton = (fileid_baton*) req->data;
    
    // init callback params
    Local<Value> argv[2];
    
    if (baton->success == true) {
        
        // create details result object
        Local<Object> details = Object::New();
        
        // set detail properties
        details->Set(String::NewSymbol("id"), Number::New(baton->id));
        details->Set(String::NewSymbol("name"), String::New((char*)baton->name));
        
        // set callback params
        argv[0] = Local<Value>::New(Null());
        argv[1] = Local<Value>::New(details);
        
    } else {
        
        // set callback params
        argv[0] = Exception::Error(String::New("failed to identify the given file"));
        argv[1] = Local<Value>::New(Null());
        
    }
    
    // execute callback function
    node::MakeCallback(Context::GetCurrent()->Global(), baton->callback, 2, argv);
    
    delete baton;
    
}

Handle<Value> identify (const Arguments& args) {
    
    HandleScope scope;
    
    // instanciate data baton
    fileid_baton* baton = new fileid_baton;
    
    // initialize baton properties
    baton->id = 0;
    baton->success = false;
    baton->req.data = (void*) baton;
    baton->path = new String::Utf8Value(args[0]->ToString());
    baton->callback = Persistent<Function>::New(Local<Function>::Cast(args[1]));
    
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
