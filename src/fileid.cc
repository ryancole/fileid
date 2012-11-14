#include <node.h>
#include <v8.h>

#ifndef UNIX
    #define UNIX
    #define UNIX_64
#endif

#include <sccfi.h>
#include <sccerr.h>
#include "fileid.h"

using namespace v8;

void fileid_identify (uv_work_t* req) {
    
    // init baton pointer
    fileid_identify_baton* baton = (fileid_identify_baton*) req->data;
    
    // identify the specified file's type
    if (FIIdFileEx(IOTYPE_UNIXPATH, baton->path, FIFLAG_NORMAL, &baton->id, baton->name, 256) == 0)
        baton->success = true;
    
}

void fileid_identify_end (uv_work_t* req) {
    
    fileid_identify_baton* baton = (fileid_identify_baton*) req->data;
    
    // init callback params
    Local<Value> argv[2];
    
    if (baton->success == true) {
        
        // create details result object
        Local<Object> details = Object::New();
        
        // set detail properties
        details->Set(String::NewSymbol("id"), Number::New(baton->id));
        details->Set(String::NewSymbol("name"), String::New(baton->name));
        
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

void fileid_init (uv_work_t* req) {
    
    // init the baton pointer
    fileid_init_baton* baton = (fileid_init_baton*) req->data;
    
    // initialize file id engine
    if (FIInit() == SCCERR_OK)
        baton->success = true;
    
}

void fileid_init_end (uv_work_t* req) {
    
    // init the baton pointer
    fileid_init_baton* baton = (fileid_init_baton*) req->data;
    
    // set global variable making note of a successful load
    if (baton->success == true)
        hasInitialized = true;
    
    // callback arguments
    Local<Value> argv[2];
    
    if (baton->success == true) {
        
        argv[0] = Local<Value>::New(Null());
        argv[1] = Local<Value>::New(Boolean::New(true));
        
    } else {
        
        argv[0] = Exception::Error(String::New("failed to initialize file id engine"));
        argv[1] = Local<Value>::New(Boolean::New(false));
        
    }
    
    // execute callback function
    node::MakeCallback(Context::GetCurrent()->Global(), baton->callback, 2, argv);
    
    delete baton;
    
}

Handle<Value> identify (const Arguments& args) {
    
    HandleScope scope;
    
    if (args.Length() != 2)
        ThrowException(Exception::SyntaxError(String::New("expected two parameters")));
    
    if (!args[0]->IsString() || !args[1]->IsFunction())
        ThrowException(Exception::TypeError(String::New("expected str, func parameters")));
    
    if (hasInitialized == true) {
        
        String::Utf8Value path (args[0]->ToString());
            
        // instanciate data baton
        fileid_identify_baton* baton = new fileid_identify_baton;
        
        // set async work data thing
        baton->req.data = (void*) baton;
        
        // init string memory locations
        baton->name = new char[256];
        baton->path = new char[path.length() + 1];
        
        // set baton properties
        baton->id = 0;
        baton->success = false;
        baton->callback = Persistent<Function>::New(Local<Function>::Cast(args[1]));
        
        // copy in string paths
        memset(baton->name, '\0', 256);
        strncpy((char*)memset(baton->path, '\0', path.length() + 1), *path, path.length());
        
        // initiate async work on thread pool
        uv_queue_work(uv_default_loop(), &baton->req, fileid_identify, fileid_identify_end);
        
    } else {
        
        Local<Value> argv[2] = {
                
            Exception::Error(String::New("the file id engine has not been initialized")),
            Local<Value>::New(Boolean::New(false))
            
        };
        
        node::MakeCallback(Context::GetCurrent()->Global(), Local<Function>::Cast(args[1]), 2, argv);
        
    }
    
    return scope.Close(Undefined());
    
}

Handle<Value> init (const Arguments& args) {
    
    HandleScope scope;
    
    if (args.Length() != 1)
        ThrowException(Exception::SyntaxError(String::New("expected one parameter")));
    
    if (!args[0]->IsFunction())
        ThrowException(Exception::TypeError(String::New("expected callback parameter")));
    
    if (hasInitialized == true) {
        
        Local<Value> argv[2] = {
                
            Local<Value>::New(Null()),
            Local<Value>::New(Boolean::New(true))
            
        };
        
        node::MakeCallback(Context::GetCurrent()->Global(), Local<Function>::Cast(args[0]), 2, argv);
        
    } else {
        
        // instanciate data baton
        fileid_init_baton* baton = new fileid_init_baton;
        
        // set async work data thing
        baton->req.data = (void*)baton;
        
        // initialize baton properties
        baton->success = false;
        baton->callback = Persistent<Function>::New(Local<Function>::Cast(args[0]));
        
        // queue work on thread pool
        uv_queue_work(uv_default_loop(), &baton->req, fileid_init, fileid_init_end);
        
    }
    
    return scope.Close(Undefined());
    
}

void initialize (Handle<Object> target) {
    
    target->Set(String::NewSymbol("initialize"), FunctionTemplate::New(init)->GetFunction());
    target->Set(String::NewSymbol("identify"), FunctionTemplate::New(identify)->GetFunction());
    
}

NODE_MODULE(fileid, initialize)
