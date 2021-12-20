#pragma once

using U64 = unsigned long long;

namespace bitboard
{
    constexpr int kRankNum = 8;
    constexpr int kFileNum = 8;
    constexpr int kGridNum = kRankNum * kFileNum;

    //compiled time mask
    constexpr U64 kFirstRankMask = 0b11111111;
    constexpr U64 kLastRankMask = kFirstRankMask << 56;
    constexpr U64 kFirstFileMask = 0x101010101010101;
    constexpr U64 kLastFileMask = kFirstFileMask << 7; 
    constexpr U64 kMainDiagonalMask = 0x102040810204080;
    constexpr U64 kMainAntiDiagonalMask = 0x8040201008040201;
    

    //various mask tables
    extern U64 kRankMaskTable[kGridNum]; //horizontal
    extern U64 kFileMaskTable[kGridNum]; //vertical
    extern U64 kDiagonalMaskTable[kGridNum]; //diagonal 
    extern U64 kAntiDiagonalMaskTable[kGridNum]; //anti diagonal
    extern U64 kRFCrossMaskTable[kGridNum]; //horizontal vertical cross
    extern U64 kDCrossMaskTable[kGridNum]; //diagonal cross
    extern U64 kCrossMaskTable[kGridNum]; // combined cross



    //precalculated attack table
    extern U64 kRFAttackTable[kGridNum][1 << 16]; //rank/file attack table
    extern U64 kDAttackTable[kGridNum][1 << 16]; //anti/diagonal attack table



    void PrintBoard(U64 board);
    int SquareToRank(int square);
    int SquareToFile(int square);
    U64 SetBit(U64 board, int square);
    U64 ClearBit(U64 board, int square);

    U64 HashByRFCross(U64 board, int square);
    U64 HashByDCross(U64 board, int square);

    U64 GetAttackBoard(U64 board, int square);

    void InitMaskTable();
    void InitAttackTable();
}

