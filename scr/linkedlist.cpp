#pragma once
#include <iostream>

#include "errors.cpp"

using namespace std;


// The Node class:
// Mostly the same as the `List` class
template <class Type>
    class Node{
        public:
            Type data;
            Node<Type>* next = NULL;

            Node(){}

            Node(Type data){
                this->set(data);
            }

            void set(Type data){
                this->data = data;
            }

            Type get(){
                return this->data;
            }

            Type get(int key){
                if (key == 0){
                    return this->get();
                }else{
                    if (this->next == NULL){
                        throw IndexError();
                    }else{
                        return this->next->get(key-1);
                    }
                }
            }

            void append(Type data){
                if (this->next == NULL){
                    this->next = new Node<Type>(data);
                }else{
                    this->next->append(data);
                }
            }

            void remove(int idx){
                if (this->next == NULL){
                    throw IndexError();
                }
                if (idx == 0){
                    Node<Type>* temp = this->next;
                    this->next = temp->next;
                    delete temp;
                }else{
                    this->next->remove(idx-1);
                }
            }

            Type pop(){
                if (this->next->next == NULL){
                    Node<Type>* temp = this->next;
                    Type data = temp->get();
                    this->next = temp->next;
                    delete temp;
                    return data;
                }else{
                    return this->next->pop();
                }
            }
    };


template <class Type>
    class List{
        public:
            Node<Type>* next = NULL;
            int length = 0;
            Node<Type>* iter_node = NULL;

            ~List(){
                /*
                Destructor
                */
                this->flush();
            }

            void deepcopy_to(List<Type>* _new){
                /*
                Deepcopies all of its attributes to `_new`.
                Only works if list elements aren't on the heap.
                */
                _new->flush();
                this->start_iter();
                while (this->has_next()){
                    _new->append(Type(this->next_item()));
                }
            }

            void deepcopy_to_p(List<Type>* _new){
                /*
                Deepcopies all of its attributes to `_new`.
                Only works if list elements are on the heap.
                */
                _new->flush_p(); // Remove everything from the other list
                // Iterate over this list:
                this->start_iter();
                while (this->has_next()){
                    Type old_item = this->next_item();
                    // The object must have the `create_new` method. It should
                    // return a pointer to a new object created on the heap.
                    Type item = old_item->create_new();
                    // The object must have the `deepcopy_to` method too.
                    old_item->deepcopy_to(item);
                    _new->append(item);
                }
            }

            void free(){
                /*
                Frees all of the list's objects
                */
                this->flush_p();
            }

            void flush(){
                /*
                Removes all objects from the list. Only works if
                list elements aren't on the heap.
                */
                this->stop_iter();
                while (this->next != NULL){
                    Type value = this->pop();
                }
            }

            void flush_p(){
                /*
                Removes all objects from the list. Only works if
                list elements are on the heap.
                */
                this->stop_iter();
                while (this->next != NULL){
                    Type item = this->pop();
                    delete item;
                }
            }

            void append(Type data){
                /*
                Appends an object to the list
                */
                this->length += 1; // add 1 to the length
                if (this->next == NULL){
                    // Create the head node:
                    this->next = new Node<Type>(data);
                }else{
                    this->next->append(data);
                }
            }

            Type operator[](int key){
                /*
                Gets an object from the list
                */
                if (this->next == NULL){
                    throw IndexError();
                }
                return this->next->get(key);
            }

            int len(){
                /*
                Returns the length of the list
                */
                return this->length;
            }

            void remove(int idx){
                /*
                Removes an object from the list (specified by idx)
                Important:
                    Must not be in the middle of iterating over the list
                    if so use `list.stop_iter()`
                */
                if (this->iter_node != NULL){
                    throw ListSizeChangedError();
                }
                if (this->next == NULL){
                    throw IndexError();
                }
                this->length -= 1;
                if (idx == 0){
                    Node<Type>* temp = this->next;
                    this->next = temp->next;
                    delete temp; // Delete the node from the heap
                }else{
                    this->next->remove(idx-1);
                }
            }

            Type pop(){
                /*
                Pops and returns the last item from the list.
                Important:
                    Must not be in the middle of iterating over the list
                    if so use `list.stop_iter()`
                */
                if (this->iter_node != NULL){
                    throw ListSizeChangedError();
                }
                if (this->next == NULL){
                    throw IndexError();
                }
                this->length -= 1;
                if (this->next->next == NULL){
                    Node<Type>* temp = this->next;
                    Type data = temp->get();
                    this->next = temp->next;
                    delete temp;
                    return data;
                }else{
                    return this->next->pop();
                }
            }

            void start_iter(){
                /*
                Starts iterating over the list.
                */
                this->iter_node = this->next;
            }

            void stop_iter(){
                /*
                Stops iterating over the list.
                */
                this->iter_node = NULL;
            }

            Type next_item(){
                /*
                Gets the next item in the list
                Note:
                    first use `list.start_iter()` to begin the iteration of the list
                */
                if (this->iter_node == NULL){
                    throw IterationError();
                }
                Type item = this->iter_node->data;
                this->iter_node = this->iter_node->next;
                return item;
            }

            bool has_next(){
                /*
                Checks if the iteration is finished.
                */
                return (not (this->iter_node == NULL));
            }

            void reverse(){
                /*
                Reverses the list. Partly taken from:
                https://www.geeksforgeeks.org/reverse-a-linked-list/
                */
                Node<Type>* current = this->next;
                Node<Type>* prev = NULL;
                Node<Type>* next = NULL;

                while (current != NULL){
                    next = current->next;
                    current->next = prev;
                    prev = current;
                    current = next;
                }
                this->next = prev;
            }

            void print(){
                /*
                Prints the list to stdout.
                */
                this->start_iter();
                cout << "[";
                while (this->has_next()){
                    Type item = this->next_item();
                    if (this->has_next()){
                        cout << item << ", ";
                    }else{
                        cout << item << "]\n";
                    }
                }
            }

            void print_p(){
                /*
                Prints the list to stdout.
                This is for when `Type` is a pointer.
                */
                this->start_iter();
                cout << "[";
                while (this->has_next()){
                    Type item = this->next_item();
                    if (this->has_next()){
                        cout << *item << ", ";
                    }else{
                        cout << *item << "]\n";
                    }
                }
            }
    };
