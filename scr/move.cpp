#pragma once
#include <iostream>
#include <string>

#include "linkedlist.cpp"
#include "memorycontrol.cpp"

using namespace std;


class Move{
    public:
        int from = NULL;
        int to = NULL;
        int promotion = NULL;

        Move(){}

        Move* create_new(){
            return new Move;
        }

        void deepcopy_to(Move* _new){
            _new->from = this->from;
            _new->to = this->to;
            _new->promotion = this->promotion;
        }

        Move(int from, int to){
            this->from = from;
            this->to = to;
        }

        Move(int from, int to, int promotion){
            this->from = from;
            this->to = to;
            this->promotion = promotion;
        }

        friend ostream& operator<<(ostream& os, const Move& move){
            os << "Move(from=" << move.from << ", to=" << move.to;
            os << ", promotion=" << move.promotion << ")";
            return os;
        }
};

typedef List<Move*> MovePointers;
typedef List<Move> Moves;

