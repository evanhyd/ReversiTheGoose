#include "Reversi.h"



Reversi::Reversi() : boards_(), turn_(kBlack), history_(), move_(0)
{
    this->boards_[kBlack] = bitboard::SetBit(boards_[kBlack], bitboard::D5);
    this->boards_[kBlack] = bitboard::SetBit(boards_[kBlack], bitboard::E4); 

    this->boards_[kWhite] = bitboard::SetBit(boards_[kWhite], bitboard::D4); 
    this->boards_[kWhite] = bitboard::SetBit(boards_[kWhite], bitboard::E5);

    this->boards_[kBoth] = boards_[kBlack] | boards_[kWhite];

    this->history_[this->move_] = { this->boards_[kBlack], this->boards_[kWhite] };
}


void Reversi::Start()
{

}

void Reversi::Flip(int square)
{
    //check replacable squares

    //apply flip masks

    //add to the history and increment the counter
}

void Reversi::Print()
{
    std::cout << "-------------------\n";

    for (int rank = 0; rank < bitboard::kRankNum; ++rank)
    {
        std::cout << '|';
        for (int file = 0; file < bitboard::kFileNum; ++file)
        {
            int square = rank * 8 + file;
            if (bitboard::IsSetBit(this->boards_[kBlack], square)) std::cout << ' ' <<'O';
            else if (bitboard::IsSetBit(this->boards_[kWhite], square)) std::cout << ' ' << char(254);
            else std::cout << " .";
        }
        std::cout << " |\n";
    }

    std::cout << "-------------------\n";

    std::cout << "Black: " << bitboard::CountSetBit(this->boards_[kBlack]) << '\n';
    std::cout << "White: " << bitboard::CountSetBit(this->boards_[kWhite]) << "\n\n";
}

