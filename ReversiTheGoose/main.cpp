#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#include <iostream>
#include <ctime>
#include "bitboard.h"
#include "Reversi.h"

int main()
{
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

    std::cout << "Initializing Mask Table\n";
    bitboard::InitMaskTable();


    std::cout << "Initializing Attack Table\n";
    bitboard::InitAttackTable();

    std::cout << "All the tables have been loaded\n";

    for (unsigned char a = 0; a < 255; ++a)
    {
        std::cout << a << '\n';
    }

    Reversi reversi;
    reversi.PrintBoard();
}