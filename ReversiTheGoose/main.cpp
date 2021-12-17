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

    bitboard::InitTable();

    srand(time(NULL));
    U64 a = U64(rand()) << 48 | U64(rand()) << 32 | U64(rand()) << 16 | rand();
    bitboard::PrintBoard(a);

    a = bitboard::HashByRFCross(a, 3 * 8 + 5);



    bitboard::PrintBoard(a);
    
}