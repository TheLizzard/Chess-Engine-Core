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
            if (this->state != NULL){
                delete this->state;
            }
            this->states.free();
            this->stack.free();
            this->state = new BoardState(fen);
        }

        Moves* legal_moves(){
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
            this->states.append(this->state);
            this->state = this->state->push(*move);
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

        PiecePointers* get_pieces_to_play(){
            return this->state->get_pieces_to_play();
        }

        PiecePointers* get_pieces_not_to_play(){
            return this->state->get_pieces_not_to_play();
        }

        bool is_in_check(){
            return this->state->is_in_check();
        }
};
