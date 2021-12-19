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
    int square = 3 * 8 + 2;
    U64 a = U64(rand()) << 48 | U64(rand()) << 32 | U64(rand()) << 16 | rand();

    std::cout << "Original board:\n";
    bitboard::PrintBoard(a);

    U64 h1 = bitboard::HashByRFCross(a, square);
    std::cout << "Horizontal Vertical hash\n";
    std::cout <<"Array index: " << h1 << '\n';
    bitboard::PrintBoard(h1);


    U64 h2 = bitboard::HashByDCross(a, square);
    std::cout << "anti/diagonal hash\n";
    std::cout << "Array index: " << h2 << '\n';
    bitboard::PrintBoard(h2);


    bitboard::PrintBoard(bitboard::GetAttackBoard(a, square));


    /*bitboard::PrintBoard(bitboard::kFirstRankMask);
    bitboard::PrintBoard(bitboard::kLastRankMask);
    bitboard::PrintBoard(bitboard::kFirstFileMask);
    bitboard::PrintBoard(bitboard::kLastFileMask);
    bitboard::PrintBoard(bitboard::kMainDiagonalMask);
    bitboard::PrintBoard(bitboard::kMainAntiDiagonalMask);*/
}