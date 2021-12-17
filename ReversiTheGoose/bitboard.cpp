#include "bitboard.h"
#include <iostream>


U64 bitboard::kSquareToRank[kGridNum];
U64 bitboard::kSquareToFile[kGridNum];

U64 bitboard::kRankMaskTable[kGridNum];
U64 bitboard::kFileMaskTable[kGridNum];
U64 bitboard::kDiagonalMaskTable[kGridNum];
U64 bitboard::kAntiDiagonalMaskTable[kGridNum];

U64 bitboard::kRFCrossMaskTable[kGridNum];
U64 bitboard::kDCrossMaskTable[kGridNum];
U64 bitboard::kCrossMaskTable[kGridNum];


void bitboard::PrintBoard(U64 board)
{
    for (int rank = 0; rank < kRankNum; ++rank)
    {
        for (int file = 0; file < kFileNum; ++file)
        {
            std::cout << (board & 1);
            board >>= 1;
        }

        std::cout << '\n';
    }
    std::cout << '\n';
}

U64 bitboard::SetBit(U64 board, int square)
{
    return board | 1ull << square;
}

U64 bitboard::ClearBit(U64 board, int square)
{
    return board & ~(1ull << square);
}




U64 bitboard::HashByRFCross(U64 board, int square)
{
    return (board >> (kSquareToRank[square] * 8) & kRankMaskTable[0]) |
           (((board >> kSquareToFile[square] & kFileMaskTable[0]) * kDiagonalMaskTable[7]) >> 56 << 8);
}

U64 bitboard::HashByDCross(U64 board, int square)
{
    return 0;
}



void bitboard::InitTable()
{
    //initialize rank file look up table
    for (int rank = 0; rank < kRankNum; ++rank)
    {
        for (int file = 0; file < kFileNum; ++file)
        {
            kSquareToRank[rank * 8 + file] = rank;
            kSquareToFile[rank * 8 + file] = file;
        }
    }


    //initialize rank table
    U64 rank_mask = 0b11111111;
    for (int rank = 0; rank < kRankNum; ++rank)
    {
        for (int file = 0; file < kFileNum; ++file)
        {
            kRankMaskTable[rank * 8 + file] = rank_mask;
        }

        rank_mask <<= 8;
    }



    //initialize file table
    U64 file_mask = 1ull | 1ull << 8 | 1ull << 16 | 1ull << 24 | 1ull << 32 | 1ull << 40 | 1ull << 48 | 1ull << 56;
    for (int file = 0; file < kFileNum; ++file)
    {
        for (int rank = 0; rank < kRankNum; ++rank)
        {
            kFileMaskTable[rank * 8 + file] = file_mask;
        }

        file_mask <<= 1;
    }



    //initialize diagonal table
    constexpr int kMaxLevel = kRankNum + kFileNum - 1;
    U64 diagonals[kMaxLevel] = { 1 };
    for (int level = 1; level < kMaxLevel; ++level)
    {
        diagonals[level] = diagonals[level - 1] << 1 & ~kFileMaskTable[0] | diagonals[level - 1] << 8;
    }

    for (int rank = 0; rank < kRankNum; ++rank)
    {
        for (int file = 0; file < kFileNum; ++file)
        {
            kDiagonalMaskTable[rank * 8 + file] = diagonals[rank + file];
        }
    }


    
    //initialize anti diagonal table
    U64 anti_diagonals[kMaxLevel] = { 1ull << 56 };
    for (int level = 1; level < kMaxLevel; ++level)
    {
        anti_diagonals[level] = anti_diagonals[level - 1] << 1 & ~kFileMaskTable[0] | anti_diagonals[level - 1] >> 8;
    }

    for (int rank = 0; rank < kRankNum; ++rank)
    {
        for (int file = 0; file < kFileNum; ++file)
        {
            kAntiDiagonalMaskTable[rank * 8 + file] = anti_diagonals[rank + kFileNum - 1 - file];
        }
    }



    //initialize cross tables
    for (int square = 0; square < kGridNum; ++square)
    {
        kRFCrossMaskTable[square] = kRankMaskTable[square] | kFileMaskTable[square];
        kDCrossMaskTable[square] = kDiagonalMaskTable[square] | kAntiDiagonalMaskTable[square];
        kCrossMaskTable[square] = kRFCrossMaskTable[square] | kDCrossMaskTable[square];
    }
}