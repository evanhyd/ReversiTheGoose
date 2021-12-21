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

    U64 GetLegalMove();
    bool Human();
    bool Engine();
    double Search(int max_depth, double alpha, double beta);
    long long Perft(int max_depth, bool passed);
    double Evaluate();



    void Flip(int square);
    void Pass();
    void TakeBack();
    void Print();

};

