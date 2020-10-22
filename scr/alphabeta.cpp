#include <iostream>
#include <string>

#include "board.cpp"
#include "move.cpp"
#include "boardstate.cpp"

using namespace std;


int PAWN_EVAL = 1;
int KNIGHT_EVAL = 3;
int BISHOP_EVAL = 3;
int ROOK_EVAL = 5;
int QUEEN_EVAL = 9;
int KING_EVAL = 999999;

int min(int a, int b){
    if (a > b){
        return b;
    }else{
        return a;
    }
}

int max(int a, int b){
    if (a > b){
        return a;
    }else{
        return b;
    }
}


int eval_board(Board* board){
    int eval = 0;
    board->state->white.start_iter();
    while (board->state->white.has_next()){
        Piece* piece = board->state->white.next_item();
        switch (piece->type){
            case 0:
                eval += PAWN_EVAL;
                break;
            case 1:
                eval += KNIGHT_EVAL;
                break;
            case 2:
                eval += BISHOP_EVAL;
                break;
            case 3:
                eval += ROOK_EVAL;
                break;
            case 4:
                eval += QUEEN_EVAL;
                break;
            case 5:
                eval += KING_EVAL;
                break;
        }
    }
    board->state->black.start_iter();
    while (board->state->black.has_next()){
        Piece* piece = board->state->black.next_item();
        switch (piece->type){
            case 0:
                eval -= PAWN_EVAL;
                break;
            case 1:
                eval -= KNIGHT_EVAL;
                break;
            case 2:
                eval -= BISHOP_EVAL;
                break;
            case 3:
                eval -= ROOK_EVAL;
                break;
            case 4:
                eval -= QUEEN_EVAL;
                break;
            case 5:
                eval -= KING_EVAL;
                break;
        }
    }
    return eval;
}


int alphabeta(Board* board, int depth, int alpha, int beta){
    if ((depth == 0) or board->is_game_over()){
        return eval_board(board);
    }
    if (board->player()){
        int value = -9999999;
        Moves possible_moves = board->legal_moves();
        possible_moves.start_iter();
        while (possible_moves.has_next()){
            Move move = possible_moves.next_item();
            Board* child = new Board();
            board->deepcopy_to(child);
            child->push(move);
            value = max(value, alphabeta(child, depth-1, alpha, beta));
            delete child;
            alpha = max(alpha, value);
            if (alpha >= beta){
                break;
            }
        }
        return value;
    }else{
        int value = 9999999;
        Moves possible_moves = board->legal_moves();
        possible_moves.start_iter();
        while (possible_moves.has_next()){
            Move move = possible_moves.next_item();
            Board* child = new Board();
            board->deepcopy_to(child);
            child->push(move);
            value = min(value, alphabeta(child, depth-1, alpha, beta));
            delete child;
            beta = min(beta, value);
            if (alpha >= beta){
                break;
            }
        }
        return value;
    }
    cout << "Unreachable\n";
}


int alphabeta(Board* board, int depth){
    return alphabeta(board, depth, -9999999, 9999999);
}


int alphabeta(string fen, int depth){
    Board* board = new Board();
    board->set_fen(fen);
    int result = alphabeta(board, depth, -9999999, 9999999);
    delete board;
    return result;
}


int main(int argc, char* argv[]){
    Board* board = new Board();
    board->set_fen("rnbqkbnr/pPpppppp/8/8/8/8/1PPPPPPP/RNBQKBNR w KQkq - 0 1");

    int result = alphabeta(board, 5);
    cout << result << "\n";
    board->print();
    delete board;
    return 0;
}


//void __main(int argc, char* argv[]){
//    for (int i = 0; i < argc; i++){
//        string x = argv[i];
//        cout << "Argument " << i << " = " << x << "\n";
//    }
//}

