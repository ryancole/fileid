#include <node.h>
#include <v8.h>

#ifndef UNIX
    #define UNIX
    #define UNIX_64
#endif

#include <sccfi.h>
#include <sccerr.h>

using namespace v8;

Handle<Value> identify (const Arguments& args) {
    
    HandleScope scope;
    
    if (args.Length() != 1) {
        
        ThrowException(Exception::TypeError(String::New("expected one argument")));
        return scope.Close(Undefined());
        
    }
    
    if (!args[0]->IsString()) {
        
        ThrowException(Exception::TypeError(String::New("expected string argument")));
        return scope.Close(Undefined());
        
    }
    
    VTWORD wType;
    VTBYTE pTypeName[128] = "";
    
    // get the specified file path
    String::Utf8Value path (args[0]->ToString());
    
    // identify the specified file's type
    if (FIIdFileEx(IOTYPE_UNIXPATH, *path, FIFLAG_NORMAL, &wType, (char*)pTypeName, 128) != 0)
        return scope.Close(Undefined());
    
    // create the new object
    Local<Object> result = Object::New();
    
    // set properties
    result->Set(String::NewSymbol("id"), Number::New(wType));
    result->Set(String::NewSymbol("name"), String::New((char*)pTypeName));
    
    return scope.Close(result);
    
}

void initialize (Handle<Object> target) {
    
    // initialize file id engine
    if (FIInit() != SCCERR_OK)
        ThrowException(Exception::TypeError(String::New("Failed to initialize file id engine")));
    
    // export identify function
    target->Set(String::NewSymbol("identify"), FunctionTemplate::New(identify)->GetFunction());
    
}

NODE_MODULE(fileid, initialize)
