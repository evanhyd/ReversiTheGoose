#pragma once

using U64 = unsigned long long;

namespace bitboard
{
    enum Coordinate : int
    {
        A1, B1, C1, D1, E1, F1, G1, H1,
        A2, B2, C2, D2, E2, F2, G2, H2,
        A3, B3, C3, D3, E3, F3, G3, H3,
        A4, B4, C4, D4, E4, F4, G4, H4,
        A5, B5, C5, D5, E5, F5, G5, H5,
        A6, B6, C6, D6, E6, F6, G6, H6,
        A7, B7, C7, D7, E7, F7, G7, H7,
        A8, B8, C8, D8, E8, F8, G8, H8
    };

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


    //precalculated flip table
    extern U64 kFlipMaskTable[kGridNum][kGridNum]; //[srce][dest]


    int SquareToRank(int square);
    int SquareToFile(int square);
    bool IsSetBit(U64 board, int square);
    U64 SetBit(U64 board, int square);
    U64 ClearBit(U64 board, int square);
    int CountSetBit(U64 board);
    void PrintBoard(U64 board);

    U64 HashByRFCross(U64 board, int square);
    U64 UnhashByRFCross(U64 hash, int square);
    U64 HashByDCross(U64 board, int square);
    U64 UnhashByDCross(U64 hash, int square);

    U64 GetAttackBoard(U64 board, int square);

    void InitMaskTable();
    void InitAttackTable();
    void InitFlipTable();
}

