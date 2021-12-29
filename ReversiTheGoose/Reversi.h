#pragma once
#include "bitboard.h"
#include <iostream>
#include <mutex>

class NeuralNetwork;

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

    explicit operator std::vector<double>() const;

    
    static long long Perft(int depth, Reversi& game);
    static double Search(const int kMaxDepth, int depth, double alpha, double beta, Reversi& game, int& best_move);
    static void MultiPerft(int kMaxDepth, int depth, Reversi game, long long& nodes);
    static void MultiSearch(const int kMaxDepth, int depth, double &ret_value, double alpha, double beta, Reversi game, int& best_move);

    static std::mutex mut;
    static void Train(int& round, Reversi game, NeuralNetwork& network);
};

