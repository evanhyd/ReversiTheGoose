#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#include <iostream>
#include <ctime>
#include "bitboard.h"


using namespace std;

int main()
{
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

    std::cout << "Initializing Mask Table\n";
    bitboard::InitMaskTable();


    std::cout << "Initializing Attack Table\n";
    bitboard::InitAttackTable();

    std::cout << "All the tables have been loaded\n";

    srand(time(NULL));
    U64 a = U64(rand()) << 48 | U64(rand()) << 32 | U64(rand()) << 16 | rand();

    bitboard::PrintBoard(a);
    int square = 6 * 8 + 4;
    a = bitboard::kRFAttackTable[square][bitboard::HashByRFCross(a, square)];
    bitboard::PrintBoard(a);

}