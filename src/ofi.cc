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
        
        ThrowException(Exception::TypeError(String::New("Wrong number of arguments")));
        return scope.Close(Undefined());
        
    }
    
    if (!args[0]->IsString()) {
        
        ThrowException(Exception::TypeError(String::New("Wrong arguments")));
        return scope.Close(Undefined());
        
    }
    
    // get the specified file path
    Local<String> path (args[0]->ToString());
    
    VTWORD wType;
    VTDWORD dwFlags = 0;
    VTBYTE pTypeName[4096] = "";
    
    // initialize file id engine
    SCCERR fileIdError = FIInit();
    
    if (fileIdError != SCCERR_OK)
        return scope.Close(Number::New((unsigned int)fileIdError));
    
    // identify the specified file's type
    fileIdError = FIIdFileEx(IOTYPE_UNIXPATH, *path, dwFlags, &wType, (char*)pTypeName, 4096);
    
    if (fileIdError != SCCERR_OK)
        return scope.Close(Number::New((unsigned int)fileIdError));
    
    // create the new object
    Local<Object> result = Object::New();
    
    // set properties
    result->Set(String::NewSymbol("id"), Number::New(wType));
    result->Set(String::NewSymbol("name"), String::New((char*)pTypeName));
    
    // deinitialize
    FIDeInit();
    
    return scope.Close(result);
    
}

void initialize (Handle<Object> target) {
    
    target->Set(String::NewSymbol("identify"), FunctionTemplate::New(identify)->GetFunction());
    
}

NODE_MODULE(ofi, initialize)
