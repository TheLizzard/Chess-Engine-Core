#pragma once
#include <iostream>
#include <string>
#include <bitset>

#include "linkedlist.cpp"
#include "move.cpp"


using namespace std;


typedef bitset<64> State;


//    -------- -------- 
//   | Type   | Number |
//    -------- -------- 
//   | Pawn   |   0    |
//   | Knight |   1    |
//   | Bishop |   2    |
//   | Rook   |   3    |
//   | Queen  |   4    |
//   | King   |   5    |
//    -------- -------- 


class Piece{
    public:
        bool colour;
        int type;
        int x;
        int y;

        Piece(){}

        Piece* create_new(){
            /*
            Creates a new Piece on the heap and returns
            a pointer to it.
            */
            return new Piece;
        }

        void deepcopy_to(Piece* _new){
            /*
            Deepcopies all of its attributes to `_new`.
            */
            _new->colour = this->colour;
            _new->type = this->type;
            _new->x = this->x;
            _new->y = this->y;
        }

        Piece(int type, bool colour, int x, int y){
            /*
            The constuctor
            */
            this->type = type;
            this->x = x;
            this->y = y;
            this->colour = colour;
        }

        State move_mask(State self, State other){
            /*
            This generates all of the legal moves that the piece can make.
            It takes in 2 board the bitset<64> representations of the pieces
            for white then black (assuming that (this->type == true)).
            */
            State output;
            int result;
            switch (this->type){
                // If the piece is a pawn
                case 0:
                    // 1 square forward
                    if (this->step_direction(self, other, 0, 1) == 3){
                        output |= this->pos_mask(0, 1);
                        if (((this->y == 1) and this->colour) or ((this->y == 6) and not this->colour)){
                            // 2 squares forward
                            if (this->step_direction(self, other, 0, 2) == 3){
                                output |= this->pos_mask(0, 2);
                            }
                        }
                    }
                    // diagonal right capture
                    if (this->step_direction(self, other, 1, 1) == 2){
                        output |= this->pos_mask(1,  1);
                    }
                    // diagonal left capture
                    if (this->step_direction(self, other, -1, 1) == 2){
                        output |= this->pos_mask(-1,  1);
                    }

                // If the piece is a knight
                case 1:
                    result = this->step_direction(self, other, 2, 1);
                    if ((result == 2) or (result == 3)){
                        output |= this->pos_mask(2,  1);
                    }
                    result = this->step_direction(self, other, -2, 1);
                    if ((result == 2) or (result == 3)){
                        output |= this->pos_mask(-2,  1);
                    }
                    result = this->step_direction(self, other, -2, -1);
                    if ((result == 2) or (result == 3)){
                        output |= this->pos_mask(-2,  -1);
                    }
                    result = this->step_direction(self, other, 2, -1);
                    if ((result == 2) or (result == 3)){
                        output |= this->pos_mask(2,  -1);
                    }
                    result = this->step_direction(self, other, 1, 2);
                    if ((result == 2) or (result == 3)){
                        output |= this->pos_mask(1,  2);
                    }
                    result = this->step_direction(self, other, -1, 2);
                    if ((result == 2) or (result == 3)){
                        output |= this->pos_mask(-1,  2);
                    }
                    result = this->step_direction(self, other, 1, -2);
                    if ((result == 2) or (result == 3)){
                        output |= this->pos_mask(1,  -2);
                    }
                    result = this->step_direction(self, other, -1, -2);
                    if ((result == 2) or (result == 3)){
                        output |= this->pos_mask(-1,  -2);
                    }

                // If the piece is a bishop
                case 2:
                    output |= this->diagonals(self, other);

                // If the piece is a root
                case 3:
                    output |= this->horizontal_vertical(self, other);

                // If the piece is a queen
                case 4:
                    output |= this->horizontal_vertical(self, other);
                    output |= this->diagonals(self, other);

                // if the piece is a king:
                case 5:
                    result = this->step_direction(self, other, 0, 1);
                    if ((result == 2) or (result == 3)){
                        output |= this->pos_mask(0, 1);
                    }
                    result = this->step_direction(self, other, 1, 0);
                    if ((result == 2) or (result == 3)){
                        output |= this->pos_mask(1, 0);
                    }
                    result = this->step_direction(self, other, 1, 1);
                    if ((result == 2) or (result == 3)){
                        output |= this->pos_mask(1, 1);
                    }
                    result = this->step_direction(self, other, 0, -1);
                    if ((result == 2) or (result == 3)){
                        output |= this->pos_mask(0, -1);
                    }
                    result = this->step_direction(self, other, -1, 0);
                    if ((result == 2) or (result == 3)){
                        output |= this->pos_mask(-1, 0);
                    }
                    result = this->step_direction(self, other, -1, -1);
                    if ((result == 2) or (result == 3)){
                        output |= this->pos_mask(-1, -1);
                    }
                    result = this->step_direction(self, other, -1, 1);
                    if ((result == 2) or (result == 3)){
                        output |= this->pos_mask(-1, 1);
                    }
                    result = this->step_direction(self, other, 1, -1);
                    if ((result == 2) or (result == 3)){
                        output |= this->pos_mask(1, -1);
                    }
            }
            return output;
        }

        int step_direction(State self, State other, int dx, int dy){
            /*
            Checks if the piece can move (dx, dy) steps from its current
            position. It returns an int signalling if the program should
            stop/continue in that direction. It also signals if the program
            should add that move to its legal moves list.
                 -------- ------- ------ 
                | Return | Legal | Stop |
                 -------- ------- ------ 
                |    0   |   0   |   0  |
                |    1   |   0   |   1  |
                |    2   |   1   |   1  |
                |    3   |   1   |   0  |
                 -------- ------- ------ 
            Important:
                It doesn't chech if it had hit any pieces whilest moving.
                It is the caller's responsibility to check that.
            */

            // `direction` holds the value for if the pieces should move
            // up or down the board. That depends on the colour of the piece.
            // It is only important for pawns as they can move in only
            // one direction
            int direction = this->colour*2-1;

            // First check if we going over the board bounds
            bool output_of_x_bounds = (this->x+dx > 7) or (this->x+dx < 0);
            bool output_of_y_bounds = (this->y+direction*dy > 7) or (this->y+direction*dy < 0);
            if (output_of_x_bounds or output_of_y_bounds){
                return 0;
            }

            // Check if we are actually moving:
            if ((dx == 0) and (dy == 0)){
                throw ValueError();
            }

            // Check if I am going to capture:
            State board = other;
            board &= this->pos_mask(dx, dy);
            if (board.any()){
                return 2;
            }

            // Check if I am going to hit myself:
            board = self;
            board &= this->pos_mask(dx, dy);
            if (board.any()){
                return 1;
            }

            return 3;
        }

        State diagonals(State self, State other){
            /*
            Checks for all possible diagonal moves. It does
            this by stepping in each direction 1 step at a
            time, checking if it is a legal move.
            Important:
                This doesn't have any piece type checks
                It is the caller's responsibility to do that
            */
            State output;
            // check this: "/" direction
            for (int d=1; d<8; d++){
                int result = this->step_direction(self, other, d, d);
                if ((result == 2) or (result == 3)){
                    output |= this->pos_mask(d,  d);
                }
                if ((result == 1) or (result == 2)){
                    break;
                }
            }
            for (int d=-1; d>-8; d--){
                int result = this->step_direction(self, other, d, d);
                if ((result == 2) or (result == 3)){
                    output |= this->pos_mask(d,  d);
                }
                if ((result == 1) or (result == 2)){
                    break;
                }
            }
            // check this: "\" direction
            for (int d=1; d<8; d++){
                int result = this->step_direction(self, other, d, -d);
                if ((result == 2) or (result == 3)){
                    output |= this->pos_mask(d,  -d);
                }
                if ((result == 1) or (result == 2)){
                    break;
                }
            }
            for (int d=-1; d>-8; d--){
                int result = this->step_direction(self, other, d, -d);
                if ((result == 2) or (result == 3)){
                    output |= this->pos_mask(d,  -d);
                }
                if ((result == 1) or (result == 2)){
                    break;
                }
            }
            return output;
        }

        State horizontal_vertical(State self, State other){
            /*
            Checks for all possible moves in the horizontal
            then vertical direction. It does this by stepping
            in each direction (horizontal then vertical)
            1 step at a time, checking if it is a legal move.
            Important:
                This doesn't have any piece type checks
                It is the caller's responsibility to do that
            */
            State output;
            // horizontal direction
            for (int dx=1; dx<8; dx++){
                int result = this->step_direction(self, other, dx, 0);
                if ((result == 2) or (result == 3)){
                    output |= this->pos_mask(dx,  0);
                }
                if ((result == 1) or (result == 2)){
                    break;
                }
            }
            for (int dx=-1; dx>-8; dx--){
                int result = this->step_direction(self, other, dx, 0);
                if ((result == 2) or (result == 3)){
                    output |= this->pos_mask(dx,  0);
                }
                if ((result == 1) or (result == 2)){
                    break;
                }
            }
            // vertical direction
            for (int dy=1; dy<8; dy++){
                int result = this->step_direction(self, other, 0, dy);
                if ((result == 2) or (result == 3)){
                    output |= this->pos_mask(0,  dy);
                }
                if ((result == 1) or (result == 2)){
                    break;
                }
            }
            for (int dy=-1; dy>-8; dy--){
                int result = this->step_direction(self, other, 0, dy);
                if ((result == 2) or (result == 3)){
                    output |= this->pos_mask(0,  dy);
                }
                if ((result == 1) or (result == 2)){
                    break;
                }
            }
            return output;
        }

        State pos_mask(int dx, int dy){
            /*
            Creates a bitset<64> mask of the position of the piece,
            if it was moved `dx` squares in the x direction and
            `dy` squares in the y direction
            */
            State output;
            int direction = this->colour*2-1;
            int pos = this->ints_to_idx(this->x+dx, this->y+direction*dy);
            output[pos] = 1;
            return output;
        }

        int ints_to_idx(int x, int y){
            /*
            Converts the (x, y) coordinate to a single int.
            It also takes in account that it is only used
            to create a bitset<64> mask of the position so
            it takes the inverse of the `y` coordinate
            */
            return this->pos(x, 7-y);
        }

        State pos_mask(){
            /*
            Creates a bitset<64> mask of the position of the piece.
            */
            State output;
            int pos = this->ints_to_idx(this->x, this->y);
            output[pos] = 1;
            return output;
        }

        int pos(int x, int y){
            /*
            Converts the (x, y) coordinate to a single int.
            Note:
                This is different from the `ints_to_idx` method
            */
            return (y<<3)+x;
        }

        char get_letter(){
            /*
            Returns the letter of the piece (pawn/knight/rook/...)
            */
            //    -------- -------- 
            //   | Type   | Number |
            //    -------- -------- 
            //   | Pawn   |   0    |
            //   | Knight |   1    |
            //   | Bishop |   2    |
            //   | Rook   |   3    |
            //   | Queen  |   4    |
            //   | King   |   5    |
            //    -------- -------- 
            char letter;
            switch (this->type){
                case 0:
                    letter = 'p';
                    break;
                case 1:
                    letter = 'n';
                    break;
                case 2:
                    letter = 'b';
                    break;
                case 3:
                    letter = 'r';
                    break;
                case 4:
                    letter = 'q';
                    break;
                case 5:
                    letter = 'k';
                    break;
            }
            if (this->colour){
                letter = toupper(letter);
            }
            return letter;
        }

        bool legal_moves(Moves* moves, State self_mask, State other_mask, State king_mask){
            /*
            Generates all of the legal moves and appends them to its first
            arg (a pointer to a List<Move>). It does that by:
                creating a bitset<64> of all of the possible moves of the piece
                for each possible move:
                    if we are going to capture the opponents king:
                        `return false` to signal an error
                    else:
                        if it is a pawn move that can be promoted:
                            for each possible promotion:
                                append to `moves` a `Move` object
                        else:
                            append to `moves` a `Move` object
                `return true` to signal that no errors have occured
            */

            // Get all of the possible moves for this piece
            State move_mask = this->move_mask(self_mask, other_mask);
            int from = (this->y<<3)+this->x;
            // Check if we can capture the king:
            State hit_king = move_mask&king_mask;
            if (hit_king.any()){
                return false;
            }
            // For each `1` in the `move_mask` (of type `bitset<64>`):
            for (int i=0; i<64; i++){
                if (move_mask[i]){
                    // Use some bit operations to figure out the position of the
                    // to square
                    int to_y = 7-(i>>3);  // lovely magic to speed up the code
                    int to = (to_y<<3)+(i&7);// lovely magic to speed up the code
                    // Oh God too many checks:
                    if ((this->type == 0) and (((this->colour == 1) and (to_y == 7)) or ((this->colour == 0) and (to_y == 0)))){
                        // For each possible promotion:
                        for (int prom=1; prom<5; prom++){
                            // Append the move to the list of moves
                            moves->append(Move(from, to, prom));
                        }
                    }else{
                        // Append the move to the list of moves
                        moves->append(Move(from, to));
                    }
                }
            }
            // No errors have occured so:
            return true;
        }

        friend ostream& operator<<(ostream& os, const Piece& piece){
            /*
            Display the object to stdout (for debug only)
            */
            string type;
            if (piece.type == 0){
                type = "pawn";
            }else if (piece.type == 1){
                type = "knight";
            }else if (piece.type == 2){
                type = "bishop";
            }else if (piece.type == 3){
                type = "rook";
            }else if (piece.type == 4){
                type = "queen";
            }else{
                type = "king";
            }
            os << "Piece(number_type=" << piece.type << ", x=";
            os << piece.x << ", y=" << piece.y << ", colour=";
            os << piece.colour << ", type=" << type << ")";
            return os;
        }
};

// Define some useful types that we are going to use later:
typedef List<Piece> Pieces;
typedef List<Piece*> PiecePointers;
