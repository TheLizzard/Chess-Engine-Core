// https://www.geeksforgeeks.org/reverse-a-linked-list/
#pragma once
#include <iostream>
//#include <type_traits>
#include "memorycontrol.cpp"

#include "errors.cpp"

using namespace std;


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

            int len(){
                if (this->next == NULL){
                    return 1;
                }
                return this->next->len()+1;
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
                    this->next->pop();
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
                this->stop_iter();
                while (this->next != NULL){
                    Type _ = this->pop();
                }
            }

            void deepcopy_to(List<Type>* _new){
                _new->flush();
                this->start_iter();
                while (this->has_next()){
                    _new->append(Type(this->next_item()));
                }
            }

            void deepcopy_to_p(List<Type>* _new){
                _new->flush_p();
                this->start_iter();
                while (this->has_next()){
                    Type old_item = this->next_item();
                    Type item = old_item->create_new();
                    old_item->deepcopy_to(item);
                    _new->append(item);
                }
            }

            void free(){
                this->flush_p();
            }

            void flush(){
                this->stop_iter();
                while (this->next != NULL){
                    this->pop();
                }
            }

            void flush_p(){
                this->stop_iter();
                while (this->next != NULL){
                    Type item = this->pop();
                    delete item;
                }
            }

            void append(Type data){
                this->length += 1;
                if (this->next == NULL){
                    this->next = new Node<Type>(data);
                }else{
                    this->next->append(data);
                }
            }

            Type operator[](int key){
                if (this->next == NULL){
                    throw IndexError();
                }
                return this->next->get(key);
            }

            int reset_len(){
                return this->next->len();
            }

            int len(){
                return this->length;
            }

            void remove(int idx){
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
                    delete temp;
                }else{
                    this->next->remove(idx-1);
                }
            }

            Type pop(){
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
                    this->next->pop();
                }
            }

            void start_iter(){
                this->iter_node = this->next;
            }

            void stop_iter(){
                this->iter_node = NULL;
            }

            Type next_item(){
                if (this->iter_node == NULL){
                    throw IterationError();
                }
                Type item = this->iter_node->data;
                this->iter_node = this->iter_node->next;
                return item;
            }

            bool has_next(){
                return (not (this->iter_node == NULL));
            }

            void reverse(){
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


//class Object{
//    public:
//        int value;
//        Object(){}
//        Object(int x){
//            this->value = x;
//        }
//        Object(Object* _new){
//            _new->value = this->value;
//        }
//        friend ostream& operator<<(ostream& os, const Object& obj){
//            return os << obj.value;
//        }
//        void deepcopy_to(Object* _new){
//            _new->value = this->value;
//        }
//        Object* create_new(){
//            return new Object;
//        }
//};
//
//
//int main(){
//    List<Object*> list;
//    list.append(new Object(10));
//    list.append(new Object(20));
//    list.append(new Object(30));
//    list.append(new Object(40));
//    //list.remove(0);
//    return 0;
//}

