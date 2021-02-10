#include <iostream>
#include <fstream>
#include <string>

#include "board.cpp"
#include "move.cpp"
#include "boardstate.cpp"


using namespace std;


float PAWN_EVAL = 1;
float KNIGHT_EVAL = 3;
float BISHOP_EVAL = 3;
float ROOK_EVAL = 5;
float QUEEN_EVAL = 9;
float KING_EVAL = 999999;
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


double eval_pieces(PiecePointers list){
    double eval = 0;
    list.start_iter();
    while (list.has_next()){
        Piece* piece = list.next_item();
        double multiplier = 1.0;

        int rank;
        if (piece->colour){
            rank = piece->y;
        }else{
            rank = 7-piece->y;
        }

        if ((piece->type == 1) or (piece->type == 2)){
            // Stop underdevelopment only applies to knights and bishops
            if ((piece->y == 0) or (piece->y == 7)){
                multiplier *= 0.9;
            }
        }
        switch (piece->type){
            case 0:// Encourage pawn pushing
                switch (rank){
                    case 1:
                        break;
                    case 2:
                        multiplier *= 1.05;
                        break;
                    case 3:
                        multiplier *= 1.1;
                        break;
                    case 4:
                        multiplier *= 1.2;
                        break;
                    case 5:
                        multiplier *= 2;
                        break;
                    case 6:
                        multiplier *= 2.5;
                        break;
                }
                eval += PAWN_EVAL*multiplier;
                break;
            case 1:
                // As a popular saying goes:
                //     "Knights on the rim are dim"
                // Some people disagree with that but still:
                if ((piece->x == 0) or (piece->x == 7)){
                    multiplier = 0.95;
                }
                eval += KNIGHT_EVAL*multiplier;
                break;
            case 2:
                eval += BISHOP_EVAL*multiplier;
                break;
            case 3:
                eval += ROOK_EVAL*multiplier;
                break;
            case 4:
                eval += QUEEN_EVAL*multiplier;
                break;
            case 5:// Later: encourage king safety (castling)?
                eval += KING_EVAL*multiplier;
                break;
        }
    }
    return eval;
}


double eval_board(Board* board){
    return eval_pieces(board->state->white) - eval_pieces(board->state->black);
}


double alphabeta(Board* board, int depth, double alpha=-9999999, double beta=9999999){
    NODES_VISITED++;
    if ((NODES_VISITED&511) == 0){
        cout << "Nodes visited = " << NODES_VISITED << "\n";
    }
    if (board->is_game_over()){
        return (9999999*((not board->player())*2-1));
    }
    if (depth == 0){
        return eval_board(board);
    }
    Moves* possible_moves = board->legal_moves();
    possible_moves->start_iter();
    double new_value;
    if (board->player()){
        double value = -9999999;
        while (possible_moves->has_next()){
            Move move = possible_moves->next_item();
            board->push(move);
            new_value = alphabeta(board, depth-1, alpha, beta);
            board->pop();
            value = max(value, new_value);
            alpha = max(alpha, value);
            if (alpha >= beta){
                break;
            }
        }
        return value;
    }else{
        double value = 9999999;
        while (possible_moves->has_next()){
            Move move = possible_moves->next_item();
            board->push(move);
            new_value = alphabeta(board, depth-1, alpha, beta);
            board->pop();
            value = min(value, new_value);
            beta = min(beta, value);
            if (alpha >= beta){
                break;
            }
        }
        return value;
    }
}


Move alphabeta_move(Board* board, int depth, double alpha=-9999999, double beta=9999999){
    NODES_VISITED++;
    Move best_move;
    Moves* possible_moves = board->legal_moves();
    possible_moves->start_iter();
    double value;
    double new_value;
    if (board->player()){
        value = -9999999;
        while (possible_moves->has_next()){
            Move move = possible_moves->next_item();
            board->push(move);
            new_value = alphabeta(board, depth-1, alpha, beta);
            board->pop();
            if (new_value > value){
                best_move = move;
                value = new_value;
            }
            alpha = max(alpha, value);
            if (alpha >= beta){
                break;
            }
        }
    }else{
        value = 9999999;
        while (possible_moves->has_next()){
            Move move = possible_moves->next_item();
            board->push(move);
            new_value = alphabeta(board, depth-1, alpha, beta);
            board->pop();
            if (new_value < value){
                best_move = move;
                value = new_value;
            }
            beta = min(beta, value);
            if (alpha >= beta){
                break;
            }
        }
    }
    return best_move;
    
}


int main(int argc, char* argv[]){
    if (argc != 4){
        return 30000001;
    }

    // Arg 0 must be the location of the data in the
    // format <Drive:\Folder\...\Folder\>
    string folder = argv[1];

    // Arg 1 must be a valid fen
    string fen = argv[2];

    // Args 2 must be an int that is >0
    int depth = stoi(argv[3]);

    Board* board = new Board();
    set_vars(folder);
    board->set_fen(fen);
    Move move = alphabeta_move(board, depth);

    delete board;
    cout << "Nodes visited = " << NODES_VISITED << "\n";
    cout << "Suceess\n";
    cout << move << "\n";
    return move.hash();
}
