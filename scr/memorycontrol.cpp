#pragma once
#include <iostream>


using namespace std;


long long int NEW_VAR_ID = 0;


//: public SuperClass
class SuperClass{
    public:
        void* operator new(size_t size){
            /*
            Creates a new object and prints to stdout the
            number of objects remaining (on the heap only)
            */

            // Reserve memory:
            void* ptr = new char[size];
            // Increment counter
            NEW_VAR_ID++;
            cout << "new " << NEW_VAR_ID << " objects remaining\n";
            return ptr;
        }

        void operator delete(void* ptr, size_t size){
            /*
            Deletes the object from the heap and prints to stdout
            the number of objects remaining (on the heap only)
            */

            // Decrement counter
            NEW_VAR_ID--;
            cout << "delete " << NEW_VAR_ID << " objects remaining\n";
            free(ptr);
        }
};


bool is_on_heap(long long int variable_testing){
    /*
    Checks if the pointer given to it (as a long long int) is
    on the heap or on the stack. It does that by createing a
    new object that is on the stack and comparing its location
    to the pointer passed in.
    */

    // Create the stack variable
    int stack_variable;
    // Gets the pointer to the stack variable
    long long int stack_pointer = (long long int)&stack_variable;
    // Check if it is on the heap
     return (stack_pointer<variable_testing);
}
