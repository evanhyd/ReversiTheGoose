#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#include <iostream>
#include <ctime>
#include <chrono>
#include <cassert>
#include "bitboard.h"
#include "Reversi.h"


int main()
{
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

    std::cout << "Initializing Mask Table\n";
    bitboard::InitMaskTable();

    std::cout << "Initializing Attack Table\n";
    bitboard::InitAttackTable();

    std::cout << "Initializing Flip Table\n";
    bitboard::InitFlipTable();

    std::cout << "All the tables have been loaded"<<std::endl;



    srand(time(NULL));
    U64 board = U64(rand()) << 48 | U64(rand()) << 32 | U64(rand()) << 16 | U64(rand());


    Reversi game;
    //game.Start();

    int depth;
    std::cin >> depth;

    auto begin = std::chrono::high_resolution_clock::now();
    int node = game.Perft(depth, false);
    auto end = std::chrono::high_resolution_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin);

    std::cout << "Perft depth " << depth << ": " << node << " nodes in " << elapsed.count() << " ms (" << node / elapsed.count() << "knode/s)\n";
}

