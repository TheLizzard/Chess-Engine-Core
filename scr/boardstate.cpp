#pragma once
#include <iostream>
#include <string>
#include <bitset>

#include "linkedlist.cpp"
#include "move.cpp"
#include "piece.cpp"
#include "errors.cpp"


using namespace std;


typedef bitset<64> State;


struct PosAsInts{
    int x;
    int y;
};


//                            -------- -------- 
//                           | Type   | Number |
//                            -------- -------- 
//                           | Pawn   |   0    |
//                           | Knight |   1    |
//                           | Bishop |   2    |
//                           | Rook   |   3    |
//                           | Queen  |   4    |
//                           | King   |   5    |
//                            -------- -------- 


class BoardState{
    public:
        Moves* _legal_moves = NULL;
        PiecePointers white;
        PiecePointers black;
        bool player;
        bool gameover = false;

        BoardState(){}

        ~BoardState(){
            this->white.free();
            this->black.free();
            delete _legal_moves;
        }

        BoardState* create_new(){
            return new BoardState;
        }

        void deepcopy_to(BoardState* _new){
            _new->player = this->player;
            _new->gameover = this->gameover;
            this->white.deepcopy_to_p(&_new->white);
            this->black.deepcopy_to_p(&_new->black);
            // Not going to copy _legal_moves that because it is
            // mainly used in push and it will have to be reset
            //this->_legal_moves->deepcopy_to(_new->_legal_moves);
        }

        BoardState(string fen){
            // get the colour to play next from fen: "xxxxxxxxx w|b xxxxxx"
            for (int i=0; i<fen.size(); i++){
                if (fen[i] == ' '){
                    this->player = (fen[i+1] == 'w');
                    break;
                }
            }
            // get 2 lists of all pieces (white and black)
            int pos = 0;
            bool endloop = false;
            for (int i=0; i<fen.size(); i++){
                if (fen[i] == ' '){
                    break;
                }
                if (pos > 63){
                    cout << "Fen too long.\n";
                    throw ValueError();
                }
                PosAsInts pos_as_ints = this->pos_to_ints(pos);
                int x = pos_as_ints.x;
                int y = 7-pos_as_ints.y;
                char chr = fen[i];
                switch (tolower(chr)){
                    case 'p':{
                            if (chr == 'p'){
                                Piece* piece = new Piece(0, false, x, y);
                                this->black.append(piece);
                            }else{
                                Piece* piece = new Piece(0, true, x, y);
                                this->white.append(piece);
                            }
                            break;
                        }
                    case 'n':{
                            if (chr == 'n'){
                                Piece* piece = new Piece(1, false, x, y);
                                this->black.append(piece);
                            }else{
                                Piece* piece = new Piece(1, true, x, y);
                                this->white.append(piece);
                            }
                            break;
                        }
                    case 'b':{
                            if (chr == 'b'){
                                Piece* piece = new Piece(2, false, x, y);
                                this->black.append(piece);
                            }else{
                                Piece* piece = new Piece(2, true, x, y);
                                this->white.append(piece);
                            }
                            break;
                        }
                    case 'r':{
                            if (chr == 'r'){
                                Piece* piece = new Piece(3, false, x, y);
                                this->black.append(piece);
                            }else{
                                Piece* piece = new Piece(3, true, x, y);
                                this->white.append(piece);
                            }
                            break;
                        }
                    case 'q':{
                            if (chr == 'q'){
                                Piece* piece = new Piece(4, false, x, y);
                                this->black.append(piece);
                            }else{
                                Piece* piece = new Piece(4, true, x, y);
                                this->white.append(piece);
                            }
                            break;
                        }
                    case 'k':{
                            if (chr == 'k'){
                                Piece* piece = new Piece(5, false, x, y);
                                this->black.append(piece);
                            }else{
                                Piece* piece = new Piece(5, true, x, y);
                                this->white.append(piece);
                            }
                            break;
                        }
                    case '1':
                        break;
                    case '2':
                        pos += 1;
                        break;
                    case '3':
                        pos += 2;
                        break;
                    case '4':
                        pos += 3;
                        break;
                    case '5':
                        pos += 4;
                        break;
                    case '6':
                        pos += 5;
                        break;
                    case '7':
                        pos += 6;
                        break;
                    case '8':
                        pos += 7;
                        break;
                    case '/':
                        pos -= 1;
                        break;
                    default:
                        cout << "Unkown character: " << fen[i] << "\n";
                        endloop = true;
                        break;
                }
                pos += 1;
                if (endloop){
                    break;
                }
            }
        }

        PiecePointers* get_pieces_to_play(){
            if (this->player){
                return &this->white;
            }else{
                return &this->black;
            }
        }

        PiecePointers* get_pieces_not_to_play(){
            if (this->player){
                return &this->black;
            }else{
                return &this->white;
            }
        }

        bool is_game_over(){
            return this->gameover = (this->legal_moves()->length == 0);
        }

        Moves* legal_moves(){
            if (this->_legal_moves == NULL){
                PiecePointers* self = this->get_pieces_to_play();
                PiecePointers* other = this->get_pieces_not_to_play();
                State self_mask = this->pieces_to_bitset(self);
                State other_mask = this->pieces_to_bitset(other);
                Piece* king = this->get_king(other);

                Moves* moves = this->pseudolegal_moves(self, self_mask, other_mask, king);
                this->remove_illegal_moves(moves);
                if (moves->length == 0){
                    this->gameover = true;
                }
                this->_legal_moves = moves;
                return moves;
            }else{
                return this->_legal_moves;
            }
        }

        Moves* pseudolegal_moves(PiecePointers* self, State self_mask, State other_mask, Piece* king){
            self->start_iter();
            Moves* output = new Moves();
            int king_pos = king->pos();
            while (self->has_next()){
                Piece* piece = self->next_item();
                State move_mask = piece->move_mask(self_mask, other_mask);
                int starting_pos = piece->pos();
                for (int i=0; i<64; i++){
                    if (move_mask[i] == 1){
                        auto temp = this->pos_to_ints(i);
                        int from = piece->pos();
                        int to_y = 7-temp.y;
                        int to = this->pos(temp.x, to_y);
                        int type = piece->type;
                        if ((type == 0) and (((piece->colour == 1) and (to_y == 7)) or ((piece->colour == 0) and (to_y == 0)))){
                            for (int prom=1; prom<5; prom++){
                                Move move(from, to, prom);
                                output->append(move);
                                if (to == king_pos){
                                    self->stop_iter();
                                    delete output;
                                    throw CapturedKingError();
                                }
                            }
                        }else{
                            Move move(from, to);
                            output->append(move);
                            if (to == king_pos){
                                self->stop_iter();
                                delete output;
                                throw CapturedKingError();
                            }
                        }
                    }
                }
            }
            return output;
        }

        void remove_illegal_moves(Moves* moves){
            int i = 0;
            List<int> moves_to_remove;
            moves->start_iter();
            while (moves->has_next()){
                Move move = moves->next_item();

                BoardState* new_state = this->push(move);

                // Set up vars:
                PiecePointers* self = new_state->get_pieces_to_play();
                PiecePointers* other = new_state->get_pieces_not_to_play();
                State self_mask = new_state->pieces_to_bitset(self);
                State other_mask = new_state->pieces_to_bitset(other);
                Piece* king = new_state->get_king(other);
                // Done

                try{
                    Moves* moves = new_state->pseudolegal_moves(self, self_mask, other_mask, king);
                    delete moves;
                }catch (CapturedKingError exception){
                    moves_to_remove.append(i);
                }
                i += 1;
                delete new_state;
            }
            moves_to_remove.reverse();
            moves_to_remove.start_iter();
            while (moves_to_remove.has_next()){
                int idx = moves_to_remove.next_item();
                moves->remove(idx);
            }
        }

        BoardState* push(Move move){
            BoardState* other = new BoardState;
            this->deepcopy_to(other);

            auto ints = other->pos_to_ints(move.from);
            int from_x = ints.x;
            int from_y = ints.y;
            ints = other->pos_to_ints(move.to);
            int to_x = ints.x;
            int to_y = ints.y;

            Piece* piece_to_move;
            Piece* piece_captured;
            int idx = 0;
            bool found = false;

            PiecePointers* pieces = other->get_pieces_to_play();
            pieces->start_iter();
            while (pieces->has_next()){
                Piece* piece = pieces->next_item();
                if ((piece->x == from_x) and (piece->y == from_y)){
                    piece_to_move = piece;
                    found = true;
                    pieces->stop_iter();
                    if ((piece->type == 0) and (((to_y == 7) and piece->colour) or ((to_y == 0) and not piece->colour)) and (move.promotion == NULL)){
                        throw MoveNeedsPromotionError();
                    }
                    break;
                }
            }
            if (not found){
                throw IllegalMoveError();
            }

            pieces = other->get_pieces_not_to_play();
            pieces->start_iter();
            while (pieces->has_next()){
                Piece* piece = pieces->next_item();
                if ((piece->x == to_x) and (piece->y == to_y)){
                    piece_captured = piece;
                    pieces->stop_iter();
                    pieces->remove(idx);
                    delete piece_captured;
                    break;
                }
                idx += 1;
            }

            piece_to_move->x = to_x;
            piece_to_move->y = to_y;

            if ((piece_to_move->type == 0) and (to_y == 7)){
                piece_to_move->type = move.promotion;
            }

            other->player = not other->player;
            return other;
        }

        Piece* get_king(PiecePointers* pieces){
            Piece* output;
            bool found = false;
            pieces->start_iter();
            while (pieces->has_next()){
                Piece* piece = pieces->next_item();
                if (piece->type == 5){
                    output = piece;
                    found = true;
                    break;
                }
            }
            pieces->stop_iter();
            if (not found){
                throw ValueError();
            }
            return output;
        }

        PosAsInts pos_to_ints(int pos){
            PosAsInts result;
            result.x = pos%8;
            result.y = (int)pos/8;
            return result;
        }

        void print(){
            string data;
            string temp;
            for (int i=0; i<64; i++){
                temp += ".";
            }
            data = temp;
            string output =  " ---------- \n";
            // put the white pieces on the board:
            this->white.start_iter();
            while (this->white.has_next()){
                Piece* piece = this->white.next_item();
                data[this->pos(piece->x, 7-piece->y)] = piece->get_letter();
            }
            this->white.stop_iter();

            // add black pieces:
            this->black.start_iter();
            while (this->black.has_next()){
                Piece* piece = this->black.next_item();
                data[this->pos(piece->x, 7-piece->y)] = piece->get_letter();
            }
            this->black.stop_iter();

            // add the new lines
            for (int i=0; i<64; i++){
                if (i%8 == 0){
                    output += "| "+data.substr(i, 8)+" |"+"\n";
                }
            }
            output += " ---------- \n";
            cout << output;
        }

    private:
        State pieces_to_bitset(PiecePointers* list){
            State board;
            list->start_iter();
            while (list->has_next()){
                Piece* piece = list->next_item();
                board |= piece->pos_mask();
            }
            return board;
        }

        State pos_mask(int pos){
            bitset<64> output;
            output[this->pos_to_mask_idx(pos)] = 1;
            return output;
        }

        int pos_to_mask_idx(int pos){
            auto ints = this->pos_to_ints(pos);
            return this->pos(ints.x, 7-ints.y);
        }

        int pos(int x, int y){
            return 8*y+x;
        }
};


typedef List<BoardState> BoardStates;
typedef List<BoardState*> BoardStatePointers;
