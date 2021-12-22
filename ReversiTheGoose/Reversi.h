#pragma once
#include "bitboard.h"
#include <iostream>

class Reversi
{
    enum Player : int
    {
        kBlack,
        kWhite
    };

    U64 boards_[2]; //black, white
    bool turn_;
    bool passed_;

public:

    Reversi();

    void Start();
    bool Human();
    bool Engine();


    U64 GetLegalMove();
    void Flip(int square);
    void Pass();
    double Evaluate();
    void Print();

    static long long Perft(int depth, Reversi& game);
    static double Search(const int kMaxDepth, int depth, double alpha, double beta, Reversi& game, int& best_move);
};

