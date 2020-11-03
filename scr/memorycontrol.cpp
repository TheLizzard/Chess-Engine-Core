#pragma once
#include <iostream>


using namespace std;


long long int NEW_VAR_ID = 0;


//: public SuperClass
class SuperClass{
    public:
        void* operator new(size_t size){
            void* ptr = new char[size];
            NEW_VAR_ID++;
            cout << "new " << NEW_VAR_ID << " objects remaining\n";
            return ptr;
        }

        void operator delete(void* ptr, size_t size){
            NEW_VAR_ID--;
            cout << "delete " << NEW_VAR_ID << " objects remaining\n";
            //free(ptr);
        }
};


void is_on_heap(long long int variable_testing){
    int heap_variable;int* pointer = &heap_variable;
    bool on_heap = (long long int)pointer<variable_testing;
    cout << "On heap = " << (on_heap?"true":"false") << "\n";
}
