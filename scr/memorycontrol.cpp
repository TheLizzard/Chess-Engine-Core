#pragma once
#include <iostream>
//: public SuperClass
using namespace std;

long long int NEW_VAR_ID = 0;

class SuperClass{
    public:
        void operator delete(void* self, size_t size){
            char* _self = (char*)self;
            //int code = _self[size];
            //cout << "delete (code = " << code << ")\n";
            NEW_VAR_ID--;
            cout << "delete " << NEW_VAR_ID << " objects remaining\n";
            delete _self;
        }
    
        void* operator new(size_t size){
            void* ptr = new char[size+4];
            //char* as_str = (char*)ptr;
            //as_str[size] = NEW_VAR_ID;
            //cout << "new (code = " << NEW_VAR_ID << ")\n";
            NEW_VAR_ID++;
            cout << "new " << NEW_VAR_ID << " objects remaining\n";
            return ptr;
        }
};

