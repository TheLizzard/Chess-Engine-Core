#pragma once
#include <iostream>
#include <string>

#include "linkedlist.cpp"
#include "move.cpp"
#include "boardstate.cpp"

using namespace std;


string STARTING_FEN = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq";


class Board{
    public:
        BoardState* state;
        BoardStatePointers states;
        MovePointers stack;

        Board(){
            this->state = new BoardState(STARTING_FEN);
        }

        ~Board(){
            delete this->state;
            this->states.free();
            this->stack.free();
        }

        void deepcopy_to(Board* _new){
            this->state->deepcopy_to(_new->state);
            this->stack.deepcopy_to_p(&_new->stack);
            this->states.deepcopy_to_p(&_new->states);
        }

        void set_fen(string fen){
            delete this->state;
            this->states.free();
            this->stack.free();
            this->state = new BoardState(fen);
        }

        Moves legal_moves(){
            return this->state->legal_moves();
        }

        bool player(){
            return this->state->player;
        }

        void push(Move move){
            Move* _move = new Move(move);
            this->push(_move);
        }

        void push(Move* move){
            this->stack.append(move);

            BoardState* new_state = new BoardState;
            this->state->deepcopy_to(new_state);

            new_state->push(*move);
            this->states.append(this->state);
            this->state = new_state;
        }

        void pop(){
            Move* move = this->stack.pop();
            delete move;
            delete this->state;
            this->state = this->states.pop();
        }

        bool is_game_over(){
            return this->state->is_game_over();
        }

        void print(){
            this->state->print();
        }
};


//int main(){
//    Board* board = new Board();
//    board->print();
//    delete board;
//    return 0;
//}

