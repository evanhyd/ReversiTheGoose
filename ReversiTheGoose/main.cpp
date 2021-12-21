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



    //srand(time(NULL));
    //U64 board = U64(rand()) << 48 | U64(rand()) << 32 | U64(rand()) << 16;// | U64(rand());


    Reversi game;
    game.Start();
    
}

