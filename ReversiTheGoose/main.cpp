#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#include <iostream>
#include <fstream>
#include <ctime>
#include <chrono>
#include <cassert>
#include <Windows.h>

#include "bitboard.h"
#include "Reversi.h"
#include "NeuralNetwork.h"


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

    NeuralNetwork network
    (
        { 64 + 64 + 1,
          (8 + 7 + 6 + 5 + 4 + 3 + 2 + 1) * 8 * 2 + (1 + 3 + 6 + 10 + 15 + 21 + 28 + 36 + 1 + 3 + 6 + 10 + 15 + 21 + 28) * 2,
          8 + 8 + 15 * 2,
          64
        }
    );

    if (network.LoadNeuralNetwork("brain.txt"))
    {
        std::cout << "Loaded neural network data\n";
        //network.PrintLayerInfo();
    }

    constexpr int kSimulGames = 100;
    int round = 0;
    Reversi game;

    for (int i = 0; i < kSimulGames; ++i)
    {
        std::thread thd(Reversi::Train, std::ref(round), game, std::ref(network));
        thd.detach();
    }

    while (true)
    {
        Sleep(1000);

        Reversi::mut.lock();

        std::cout <<"Rounds: " << round << '\n';
        network.SaveNeuralNetwork("brain.txt");

        Reversi::mut.unlock();
    }



    //int depth;
    //std::cin >> depth;

    //long long nodes = 0; 

    //auto begin = std::chrono::high_resolution_clock::now();
    //Reversi::MultiPerft(depth, depth, game, nodes);
    ////nodes = Reversi::Perft(depth, game);
    //auto end = std::chrono::high_resolution_clock::now();
    //auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin);

    //std::cout << "Perft depth " << depth << ": " << nodes << " nodes in " << elapsed.count() << " ms (" << nodes / elapsed.count() << " knode/s)\n";
}

