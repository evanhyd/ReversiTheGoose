#include "Reversi.h"


Reversi::Reversi() : boards_(), turn_(kBlack), history_(), move_(0)
{
    boards_[kBlack] = bitboard::SetBit(boards_[kBlack], bitboard::D5);
    boards_[kBlack] = bitboard::SetBit(boards_[kBlack], bitboard::E4); 

    boards_[kWhite] = bitboard::SetBit(boards_[kWhite], bitboard::D4); 
    boards_[kWhite] = bitboard::SetBit(boards_[kWhite], bitboard::E5);

    boards_[kBoth] = boards_[kBlack] | boards_[kWhite];
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
        this->Print();
        std::cout << (turn_ == kBlack ? "Black" : "White") << "'s turn\n";

        if (player_mode[turn_] == 0)
        {
            bool has_good_move = this->Play();
            if (!has_good_move) break;
        }
        else
        {
            bool has_good_move = this->Search();
            if (!has_good_move) break;
        }
    }

    int black_score = bitboard::CountSetBit(boards_[kBlack]);
    int white_score = bitboard::CountSetBit(boards_[kWhite]);

    if (black_score > white_score) std::cout << "Black has won the game\a\n";
    else if (black_score < white_score) std::cout << "White has won the game\a\n";
    else std::cout << "Draw!\a\n";
}


bool Reversi::Play()
{
    bool attackable_square[bitboard::kGridNum] = { false };

    bool has_legal_move = false;
    for (U64 ally_board = boards_[turn_]; ally_board != 0; ally_board = bitboard::PopBit(ally_board))
    {
        int srce_square = bitboard::GetLSTSetBit(ally_board);

        for (U64 attack_board = bitboard::GetAttackBoard(boards_[!turn_], srce_square) & ~boards_[turn_]; attack_board; attack_board = bitboard::PopBit(attack_board))
        {
            int attack_square = bitboard::GetLSTSetBit(attack_board);
            attackable_square[attack_square] = true;
            has_legal_move = true;
        }
    }

    if (!has_legal_move) return false;

    while (true)
    {
        std::cout << "Coordinate: ";
        int rank, file, square;
        std::cin >> rank >> file;
        square = rank * bitboard::kRankLength + file;

        if (0 <= square && square < bitboard::kGridNum && attackable_square[square])
        {
            this->Flip(square);
            break;
        }
        else
        {
            std::cout << "Invalid attacking square\n";
        }
    }

    return true;
}


bool Reversi::Search()
{
    return false;
}



void Reversi::Flip(int square)
{
    //save the old history
    history_[move_++] = std::make_pair(boards_[kBlack], boards_[kWhite]);

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

    //update occupancy board and player turn
    boards_[kBoth] = boards_[kBlack] | boards_[kWhite];
    turn_ = !turn_;
}

void Reversi::TakeBack()
{
    --move_;
    boards_[kBlack] = history_[move_].first;
    boards_[kWhite] = history_[move_].second;

    boards_[kBoth] = boards_[kBlack] | boards_[kWhite];
    turn_ = !turn_;
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

