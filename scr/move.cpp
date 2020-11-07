#pragma once
#include <iostream>
#include <string>

#include "linkedlist.cpp"


using namespace std;


class Move{
    public:
        int from = 0;
        int to = 0;
        int promotion = 0;

        Move(){}

        Move* create_new(){
            return new Move;
        }

        void deepcopy_to(Move* _new){
            /*
            Deepcopies all of its attributes to `_new`.
            */
            _new->from = this->from;
            _new->to = this->to;
            _new->promotion = this->promotion;
        }

        Move(int from, int to){
            /*
            The constuctor
            */
            this->from = from;
            this->to = to;
        }

        Move(int from, int to, int promotion){
            /*
            The constuctor
            */
            this->from = from;
            this->to = to;
            this->promotion = promotion;
        }

        int hash(){
            /*
            Hash the move
            It concatenates its attributes (from, to, and promotion) into
            one int that should be front padded with 0s before decoded.
            The default value for the promotion is 0 (which is also a pawn)

            Some sample inputs and outputs:
                 ----------------------- -------- 
                |        Inputs         | Output |
                | from | to | promotion |        |
                 ------ ---- ----------- -------- 
                |   0  |  1 |    None   |   0010 |
                |   0  | 10 |    None   |   0100 |
                |  50  |  1 |    None   | 500010 |
                |  50  | 60 |    None   | 500600 |
                |  50  | 60 |   queen   | 500604 |
                 ------ ---- ----------- -------- 
            */
            return this->from*1000+this->to*10+this->promotion;
        }

        friend ostream& operator<<(ostream& os, const Move& move){
            /*
            Display the object to stdout (for debug only)
            */
            os << "Move(from=" << move.from << ", to=" << move.to;
            os << ", promotion=" << move.promotion << ")";
            return os;
        }
};

typedef List<Move*> MovePointers;
typedef List<Move> Moves;
