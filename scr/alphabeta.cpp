#include <iostream>
#include <fstream>
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
int NODES_VISITED = 0;


int read_file(string filename){
    ifstream file(filename);
    string text((istreambuf_iterator<char>(file)),
                istreambuf_iterator<char>());
    return atof(text.c_str());
}


void set_vars(string folder){
    PAWN_EVAL = read_file(folder+"Data\\PAWN_EVAL.txt");
    KNIGHT_EVAL = read_file(folder+"Data\\KNIGHT_EVAL.txt");
    BISHOP_EVAL = read_file(folder+"Data\\BISHOP_EVAL.txt");
    ROOK_EVAL = read_file(folder+"Data\\ROOK_EVAL.txt");
    QUEEN_EVAL = read_file(folder+"Data\\QUEEN_EVAL.txt");
}


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


bool check_quiet_move(Board* before, Board* after){
    if (after->is_in_check() or before->is_in_check()){
        return false;
    }else{
        int pieces_before = before->get_pieces_not_to_play()->length;
        int pieces_after = after->get_pieces_to_play()->length;
        return (pieces_before == pieces_after);
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


int alphabeta(Board* board, int depth, int quietness_w, int quietness_b, int alpha=-9999999, int beta=9999999){
    NODES_VISITED++;
    if (NODES_VISITED%256 == 0){
        cout << "Nodes visited = " << NODES_VISITED << "\n";
    }
    if (board->is_game_over()){
        return (9999999*((not board->player())*2-1));
    }
    bool quiet = ((quietness_w <= 0) and (quietness_b <= 0));
    if ((depth == 0) or quiet){
        return eval_board(board);
    }
    if (board->player()){
        int value = -9999999;
        Moves* possible_moves = board->legal_moves();
        possible_moves->start_iter();
        while (possible_moves->has_next()){
            Move move = possible_moves->next_item();
            Board* child = new Board();
            board->deepcopy_to(child);
            child->push(move);
            int new_value;
            if (check_quiet_move(board, child)){
                new_value = alphabeta(child, depth-1, quietness_w, quietness_b-1, alpha, beta);
            }else{
                new_value = alphabeta(child, depth-1, quietness_w, quietness_b, alpha, beta);
            }
            value = max(value, new_value);
            delete child;
            alpha = max(alpha, value);
            if (alpha >= beta){
                break;
            }
        }
        return value;
    }else{
        int value = 9999999;
        Moves* possible_moves = board->legal_moves();
        possible_moves->start_iter();
        while (possible_moves->has_next()){
            Move move = possible_moves->next_item();
            Board* child = new Board();
            board->deepcopy_to(child);
            child->push(move);
            int new_value;
            if (check_quiet_move(board, child)){
                new_value = alphabeta(child, depth-1, quietness_w-1, quietness_b, alpha, beta);
            }else{
                new_value = alphabeta(child, depth-1, quietness_w, quietness_b, alpha, beta);
            }
            value = min(value, new_value);
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


int main(int argc, char* argv[]){
    if (argc != 4){
        return 30000001;
    }

    string filename = argv[0];
    string folder = filename.substr(0, filename.size()-16);
    string fen = argv[1];
    int depth = stoi(argv[2]);
    int quietness = stoi(argv[3])+1;
    Board* board = new Board();

    set_vars(folder);
    board->set_fen(fen);

    int result = alphabeta(board, depth, quietness, quietness);
    delete board;

    cout << "Nodes visited = " << NODES_VISITED << "\n";
    cout << "Suceess\n";
    cout << result << "\n";
    return result+9999999;
}
