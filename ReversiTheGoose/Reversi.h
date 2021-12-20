#pragma once
#include "bitboard.h"
#include <iostream>

class Reversi
{
    enum Player : int
    {
        kBlack,
        kWhite,
        kBoth
    };

    U64 boards_[3]; //black, white, both
    bool turn_;

    std::pair<U64, U64> history_[64];
    int move_;

public:

    Reversi();

    void Start();
    void Flip(int square);
    void PrintBoard();

};

