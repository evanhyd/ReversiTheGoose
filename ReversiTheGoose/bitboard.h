#pragma once

using U64 = unsigned long long;

namespace bitboard
{
    constexpr int kRankNum = 8;
    constexpr int kFileNum = 8;
    constexpr int kGridNum = kRankNum * kFileNum;


    //square to rank/file conversion table
    extern U64 kSquareToRank[kGridNum];
    extern U64 kSquareToFile[kGridNum];
    

    //various mask tables
    extern U64 kRankMaskTable[kGridNum]; //horizontal
    extern U64 kFileMaskTable[kGridNum]; //vertical
    extern U64 kDiagonalMaskTable[kGridNum]; //diagonal 
    extern U64 kAntiDiagonalMaskTable[kGridNum]; //anti diagonal
    extern U64 kRFCrossMaskTable[kGridNum]; //horizontal vertical cross
    extern U64 kDCrossMaskTable[kGridNum]; //diagonal cross
    extern U64 kCrossMaskTable[kGridNum]; // combined cross



    //precalculate attack table
    extern U64 kRFAttackTable[kGridNum][1 << 16]; //rank/file attack table
    extern U64 kDAttackTable[kGridNum][1 << 16]; //anti/diagonal attack table



    void PrintBoard(U64 board);
    U64 SetBit(U64 board, int square);
    U64 ClearBit(U64 board, int square);

    U64 HashByRFCross(U64 board, int square);
    U64 HashByDCross(U64 board, int square);




    void InitTable();
}

