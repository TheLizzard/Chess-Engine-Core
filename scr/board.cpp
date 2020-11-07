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
            /*
            Constructor
            */
            this->state = new BoardState(STARTING_FEN);
        }

        ~Board(){
            /*
            Destructor
            */
            delete this->state;
            this->states.free();
            this->stack.free();
        }

        void deepcopy_to(Board* _new){
            /*
            Deepcopies all of its attributes to `_new`.
            */
            this->state->deepcopy_to(_new->state);
            this->stack.deepcopy_to_p(&_new->stack);
            this->states.deepcopy_to_p(&_new->states);
        }

        void set_fen(string fen){
            /*
            Sets the fen to the string provided
            Note:
                it clears the move stack
            */
            if (this->state != NULL){
                delete this->state;
            }
            this->states.free();
            this->stack.free();
            this->state = new BoardState(fen);
        }

        Moves* legal_moves(){
            /*
            Returns a pointer to the legal moves.
            Note:
                The moves should not be deleted. That is managed by the `BoardState`
            */
            return this->state->legal_moves();
        }

        bool player(){
            /*
            Returns the player to move as a bool.
            white => true
            black => false
            */
            return this->state->player;
        }

        void push(Move move){
            /*
            Pushes a move to the board.
            */
            Move* _move = new Move(move);
            this->push(_move);
        }

        void push(Move* move){
            /*
            Pushes a move to the board.
            */
            this->stack.append(move);
            this->states.append(this->state);
            this->state = this->state->push(*move);
        }

        void pop(){
            /*
            Pops a move from the board.
            Note:
                it doesn't return the move
            */
            Move* move = this->stack.pop();
            delete move;
            delete this->state;
            this->state = this->states.pop();
        }

        bool is_game_over(){
            /*
            Checks and returns if the game is over.
            */
            return this->state->is_game_over();
        }

        void print(){
            /*
            Prints the board to stdout (only for debug)
            */
            this->state->print();
        }

        PiecePointers* get_pieces_to_play(){
            /*
            Gets the pieces to play.
            Note:
                The pieces list should not be deleted. That is managed by the `BoardState`
            */
            return this->state->get_pieces_to_play();
        }

        PiecePointers* get_pieces_not_to_play(){
            /*
            Gets the oponent's pieces.
            Note:
                The pieces list should not be deleted. That is managed by the `BoardState`
            */
            return this->state->get_pieces_not_to_play();
        }

        bool is_in_check(){
            /*
            Checks and returns if there is a check on the board.
            */
            return this->state->is_in_check();
        }
};
