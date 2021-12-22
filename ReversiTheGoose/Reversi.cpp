#include "Reversi.h"
#include <cassert>
#include <thread>
#include <vector>

Reversi::Reversi() : boards_(), turn_(kBlack), passed_(false)
{
    boards_[kBlack] = bitboard::SetBit(boards_[kBlack], bitboard::D5);
    boards_[kBlack] = bitboard::SetBit(boards_[kBlack], bitboard::E4); 

    boards_[kWhite] = bitboard::SetBit(boards_[kWhite], bitboard::D4); 
    boards_[kWhite] = bitboard::SetBit(boards_[kWhite], bitboard::E5);
}


void Reversi::Start()
{
    bool player_mode[2];
    std::cout << "Player Black (0. human   1. A.I.): ";
    std::cin >> player_mode[0];
    std::cout << "Player White (0. human   1. A.I.): ";
    std::cin >> player_mode[1];


    while (true)
    {
        //check board corruption
        assert((boards_[kBlack] & boards_[kWhite]) == 0);

        U64 attackable_square = this->GetLegalMove();
        if (attackable_square == 0) break;

        this->Print();
        std::cout << (turn_ == kBlack ? "Black" : "White") << "'s turn\n";

        if (player_mode[turn_] == 0) this->Human();
        else this->Engine();
    }

    int black_score = bitboard::CountSetBit(boards_[kBlack]);
    int white_score = bitboard::CountSetBit(boards_[kWhite]);

    if (black_score > white_score) std::cout << "Black has won the game\a\n";
    else if (black_score < white_score) std::cout << "White has won the game\a\n";
    else std::cout << "Draw!\a\n";
}



bool Reversi::Human()
{
    U64 attackable_square = this->GetLegalMove();
    bitboard::PrintBoard(attackable_square);
    
    while (true)
    {
        std::cout << "Move: ";
        
        int rank, file;
 
        if (std::cin >> rank >> file)
        {
            int square = rank * bitboard::kRankLength + file;

            if (0 <= square && square < bitboard::kGridNum)
            {
                if (bitboard::IsSetBit(attackable_square, square))
                {
                    this->Flip(square);
                    break;
                }
            }
        }

        std::cout << "Invalid attacking square\n";
    }

    return true;
}


bool Reversi::Engine()
{
    U64 attackable_square = this->GetLegalMove();
    if (attackable_square == 0) return false;

    int best_move = -1;
    Search(8, 8, -99999999.9, 99999999.9, *this, best_move);

    this->Flip(best_move);

    std::cout << "Move: " << bitboard::SquareToRank(best_move) << ' ' << bitboard::SquareToFile(best_move) << '\n';
}






U64 Reversi::GetLegalMove()
{
    U64 legal_moves = 0;
    for (U64 ally_board = boards_[turn_]; ally_board != 0; ally_board = bitboard::PopBit(ally_board))
    {
        int srce_square = bitboard::GetLSTSetBit(ally_board);

        //get the attack mask + filter out ally occupied square
        legal_moves |= bitboard::GetAttackBoard(boards_[!turn_], srce_square);
    }

    //filter out ally occupied square
    legal_moves &= ~boards_[turn_];

    return legal_moves;
}


void Reversi::Flip(int square)
{
    //flip the srce square
    boards_[turn_] = bitboard::FlipBit(boards_[turn_], square);

    //loop through all the attackable square
    for (U64 attack_board = bitboard::GetAttackBoard(boards_[!turn_], square) & boards_[turn_]; attack_board != 0; attack_board = bitboard::PopBit(attack_board))
    {
        //get the attacking square
        int attack_square = bitboard::GetLSTSetBit(attack_board);

        //update boards
        boards_[turn_] = bitboard::FlipBoard(boards_[turn_], square, attack_square);
        boards_[!turn_] = bitboard::FlipBoard(boards_[!turn_], square, attack_square);
    }

    turn_ = !turn_;
}

void Reversi::Pass()
{
    turn_ = !turn_;
    passed_ = true;
}

double Reversi::Evaluate()
{
    //we will replace this will neural network eval later
    //this is just a simple evaluation functiono to prove the concept

    double score = bitboard::CountSetBit(boards_[kBlack]) - bitboard::CountSetBit(boards_[kWhite]);
    return (turn_ == kBlack ? score : -score);
}

void Reversi::Print()
{
    std::cout << "     0 1 2 3 4 5 6 7\n";
    std::cout << "    -----------------\n";

    for (int rank = 0; rank < bitboard::kRankLength; ++rank)
    {
        std::cout <<' ' << rank << " |";
        for (int file = 0; file < bitboard::kFileLength; ++file)
        {
            int square = rank * 8 + file;
            if (bitboard::IsSetBit(boards_[kBlack], square)) std::cout << ' ' << 'X';
            else if (bitboard::IsSetBit(boards_[kWhite], square)) std::cout << ' ' << 'O';
            else std::cout << " .";
        }
        std::cout << " |\n";
    }

    std::cout << "    -----------------\n";

    std::cout << "Black: " << bitboard::CountSetBit(boards_[kBlack]) << '\n';
    std::cout << "White: " << bitboard::CountSetBit(boards_[kWhite]) << "\n\n";
}




/*
    DEPTH   #LEAF NODES
========================
     1               4
     2              12
     3              56
     4             244
     5            1396
     6            8200
     7           55092
     8          390216
     9         3005288
    10        24571284
    11       212258800
    12      1939886636

    nearly 20 millions nodes/s
*/
long long Reversi::Perft(int depth, Reversi& game)
{
    if (depth == 0) return 1;

    long long node = 0;

    //get legal moves
    U64 attackable_square = game.GetLegalMove();
    if (attackable_square == 0)
    {
        //continue if opponent did not pass before
        if (!game.passed_)
        {
            Reversi child = game;
            child.Pass();
            node += Perft(depth - 1, child);
        }

        //game over
        else return 1;
    }


    while (attackable_square != 0)
    {
        int square = bitboard::GetLSTSetBit(attackable_square);

        Reversi child = game;
        child.Flip(square);
        node += Perft(depth - 1, child);

        attackable_square = bitboard::PopBit(attackable_square);
    }

    return node;
}

double Reversi::Search(const int kMaxDepth, int depth, double alpha, double beta, Reversi& game, int& best_move)
{
    if (depth == 0) return game.Evaluate();

    U64 attackable_square = game.GetLegalMove();

    double score = -99999999.9;

    //run out of move
    if (attackable_square == 0)
    {
        //continue if opponent did not pass before
        if (!game.passed_)
        {
            Reversi child = game;
            child.Pass();
            score = std::max(score, -Search(kMaxDepth, depth - 1, -beta, -alpha, child, best_move));

            if (score > beta) return beta;
            if (score > alpha) alpha = score;
        }

        //game over
        else return game.Evaluate();
    }


    while (attackable_square != 0)
    {
        int square = bitboard::GetLSTSetBit(attackable_square);

        Reversi child = game;
        child.Flip(square);
        score = std::max(score, -Search(kMaxDepth, depth - 1, -beta, -alpha, child, best_move));

        //update alpha beta
        if (score > beta) return beta;
        if (score > alpha)
        {
            alpha = score;
            if (depth == kMaxDepth) best_move = square;
        }
        
        attackable_square = bitboard::PopBit(attackable_square);
    }

    return alpha;
}