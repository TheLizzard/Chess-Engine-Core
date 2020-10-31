#pragma once
#include <iostream>
#include <string>
#include <bitset>

#include "linkedlist.cpp"
#include "move.cpp"
#include "memorycontrol.cpp"

using namespace std;


class Piece{
    public:
        bool colour;
        int type;
        int x;
        int y;

        Piece(){}

        Piece* create_new(){
            return new Piece;
        }

        void deepcopy_to(Piece* _new){
            _new->colour = this->colour;
            _new->type = this->type;
            _new->x = this->x;
            _new->y = this->y;
        }

        Piece(int type, bool colour, int x, int y){
            this->type = type;
            this->x = x;
            this->y = y;
            this->colour = colour;
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
        }

        bitset<64> move_mask(bitset<64> self, bitset<64> other){
            int x = this->x;
            int y = this->y;
            bitset<64> output;
            if (this->type == 5){
                int result = this->step_direction(self, other, 0, 1, x, y);
                if ((result == 2) or (result == 3)){
                    output |= this->pos_mask(0, 1);
                }
                result = this->step_direction(self, other, 1, 0, x, y);
                if ((result == 2) or (result == 3)){
                    output |= this->pos_mask(1, 0);
                }
                result = this->step_direction(self, other, 1, 1, x, y);
                if ((result == 2) or (result == 3)){
                    output |= this->pos_mask(1, 1);
                }
                result = this->step_direction(self, other, 0, -1, x, y);
                if ((result == 2) or (result == 3)){
                    output |= this->pos_mask(0, -1);
                }
                result = this->step_direction(self, other, -1, 0, x, y);
                if ((result == 2) or (result == 3)){
                    output |= this->pos_mask(-1, 0);
                }
                result = this->step_direction(self, other, -1, -1, x, y);
                if ((result == 2) or (result == 3)){
                    output |= this->pos_mask(-1, -1);
                }
                result = this->step_direction(self, other, -1, 1, x, y);
                if ((result == 2) or (result == 3)){
                    output |= this->pos_mask(-1, 1);
                }
                result = this->step_direction(self, other, 1, -1, x, y);
                if ((result == 2) or (result == 3)){
                    output |= this->pos_mask(1, -1);
                }
            }else if (this->type == 3){
                output |= this->horizontal_vertical(output, self, other, x, y);
            }else if (this->type == 2){
                output |= this->diagonals(output, self, other, x, y);
            }else if (this->type == 4){
                output |= this->horizontal_vertical(output, self, other, x, y);
                output |= this->diagonals(output, self, other, x, y);
            }else if (this->type == 0){ //1 square forward
                if (this->step_direction(self, other, 0, 1, x, y) == 3){
                    output |= this->pos_mask(0, 1);
                    if (((this->y == 1) and this->colour) or ((this->y == 6) and not this->colour)){
                        if (this->step_direction(self, other, 0, 2, x, y) == 3){
                            output |= this->pos_mask(0, 2);
                        }
                    }
                } //diagonal right capture
                if (this->step_direction(self, other, 1, 1, x, y) == 2){
                    output |= this->pos_mask(1,  1);
                } //diagonal left capture
                if (this->step_direction(self, other, -1, 1, x, y) == 2){
                    output |= this->pos_mask(-1,  1);
                }
            }else if (this->type == 1){
                int result = this->step_direction(self, other, 2, 1, x, y);
                if ((result == 2) or (result == 3)){
                    output |= this->pos_mask(2,  1);
                }
                result = this->step_direction(self, other, -2, 1, x, y);
                if ((result == 2) or (result == 3)){
                    output |= this->pos_mask(-2,  1);
                }
                result = this->step_direction(self, other, -2, -1, x, y);
                if ((result == 2) or (result == 3)){
                    output |= this->pos_mask(-2,  -1);
                }
                result = this->step_direction(self, other, 2, -1, x, y);
                if ((result == 2) or (result == 3)){
                    output |= this->pos_mask(2,  -1);
                }
                result = this->step_direction(self, other, 1, 2, x, y);
                if ((result == 2) or (result == 3)){
                    output |= this->pos_mask(1,  2);
                }
                result = this->step_direction(self, other, -1, 2, x, y);
                if ((result == 2) or (result == 3)){
                    output |= this->pos_mask(-1,  2);
                }
                result = this->step_direction(self, other, 1, -2, x, y);
                if ((result == 2) or (result == 3)){
                    output |= this->pos_mask(1,  -2);
                }
                result = this->step_direction(self, other, -1, -2, x, y);
                if ((result == 2) or (result == 3)){
                    output |= this->pos_mask(-1,  -2);
                }
            }

            return output;
        }

        int step_direction(bitset<64> self, bitset<64> other, int dx, int dy, int x, int y){
            /*self, other, dx, dy, x, y*/
    
            //     -------- ------- ------ 
            //    | Return | Legal | Stop |
            //     -------- ------- ------ 
            //    |    0   |   0   |   0  |
            //    |    1   |   0   |   1  |
            //    |    2   |   1   |   1  |
            //    |    3   |   1   |   0  |
            //     -------- ------- ------ 
            int direction = this->colour*2-1;
            if ((x+dx > 7) or (y+direction*dy > 7) or (x+dx < 0) or (y+direction*dy < 0)){
                return 0;
            }
            if ((dx == 0) and (dy == 0)){
                return 0;
            }
            int result = 3;

            // Check if I am going to capture:
            bitset<64> board;
            board = other;
            board &= this->pos_mask(dx, dy);
            if (board.any()){
                result = 2;
            }

            // Check if I am going to hit myself:
            board = self;
            board &= this->pos_mask(dx, dy);
            if (board.any()){
                result = 1;
            }

            return result;
        }

        bitset<64> diagonals(bitset<64> output, bitset<64> self, bitset<64> other, int x, int y){
            /*output, self, other, dx, dy, x, y*/
            /*this: "/" direction*/
            for (int d=0; d<8; d++){
                int result = this->step_direction(self, other, d, d, x, y);
                if ((result == 2) or (result == 3)){
                    output |= this->pos_mask(d,  d);
                }
                if ((result == 1) or (result == 2)){
                    break;
                }
            }
            for (int d=0; d>-8; d--){
                int result = this->step_direction(self, other, d, d, x, y);
                if ((result == 2) or (result == 3)){
                    output |= this->pos_mask(d,  d);
                }
                if ((result == 1) or (result == 2)){
                    break;
                }
            }
            /*this: "\" direction*/
            for (int d=0; d<8; d++){
                int result = this->step_direction(self, other, d, -d, x, y);
                if ((result == 2) or (result == 3)){
                    output |= this->pos_mask(d,  -d);
                }
                if ((result == 1) or (result == 2)){
                    break;
                }
            }
            for (int d=0; d>-8; d--){
                int result = this->step_direction(self, other, d, -d, x, y);
                if ((result == 2) or (result == 3)){
                    output |= this->pos_mask(d,  -d);
                }
                if ((result == 1) or (result == 2)){
                    break;
                }
            }
            return output;
        }

        bitset<64> horizontal_vertical(bitset<64> output, bitset<64> self, bitset<64> other, int x, int y){
            /*output, self, other, dx, dy, x, y*/
            /*x direction*/
            for (int dx=0; dx<8; dx++){
                int result = this->step_direction(self, other, dx, 0, x, y);
                if ((result == 2) or (result == 3)){
                    output |= this->pos_mask(dx,  0);
                }
                if ((result == 1) or (result == 2)){
                    break;
                }
            }
            for (int dx=0; dx>-8; dx--){
                int result = this->step_direction(self, other, dx, 0, x, y);
                if ((result == 2) or (result == 3)){
                    output |= this->pos_mask(dx,  0);
                }
                if ((result == 1) or (result == 2)){
                    break;
                }
            }
            /*y direction*/
            for (int dy=0; dy<8; dy++){
                int result = this->step_direction(self, other, 0, dy, x, y);
                if ((result == 2) or (result == 3)){
                    output |= this->pos_mask(0,  dy);
                }
                if ((result == 1) or (result == 2)){
                    break;
                }
            }
            for (int dy=0; dy>-8; dy--){
                int result = this->step_direction(self, other, 0, dy, x, y);
                if ((result == 2) or (result == 3)){
                    output |= this->pos_mask(0,  dy);
                }
                if ((result == 1) or (result == 2)){
                    break;
                }
            }
            return output;
        }

        bitset<64> pos_mask(int dx, int dy){
            bitset<64> output;
            int direction = this->colour*2-1;
            int pos = this->ints_to_pos(this->x+dx, this->y+direction*dy);
            output[pos] = 1;
            return output;
        }

        int ints_to_pos(int x, int y){
            return this->pos(x, 7-y);
        }

        bitset<64> pos_mask(){
            return this->pos_mask(0, 0);
        }

        int pos(int x, int y){
            return 8*y+x;
        }

        int pos(){
            return this->pos(this->x, this->y);
        }

        char get_letter(){
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
                default:
                    letter = 'a';
                    break;
            }
            if (this->colour){
                letter = toupper(letter);
            }
            return letter;
        }

        friend ostream& operator<<(ostream& os, const Piece& piece){
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

typedef List<Piece> Pieces;
typedef List<Piece*> PiecePointers;
