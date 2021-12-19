#include "bitboard.h"
#include <iostream>


U64 bitboard::kRankMaskTable[kGridNum] = { 0 };
U64 bitboard::kFileMaskTable[kGridNum] = { 0 };
U64 bitboard::kDiagonalMaskTable[kGridNum] = { 0 };
U64 bitboard::kAntiDiagonalMaskTable[kGridNum] = { 0 };
U64 bitboard::kRFCrossMaskTable[kGridNum] = { 0 };
U64 bitboard::kDCrossMaskTable[kGridNum] = { 0 };
U64 bitboard::kCrossMaskTable[kGridNum] = { 0 };


U64 bitboard::kRFAttackTable[kGridNum][1 << 16] = { 0 };
U64 bitboard::kDAttackTable[kGridNum][1 << 16] = { 0 };


void bitboard::PrintBoard(U64 board)
{
    for (int rank = 0; rank < kRankNum; ++rank)
    {
        for (int file = 0; file < kFileNum; ++file)
        {
            std::cout << (board & 1) << ' ';
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



int bitboard::SquareToRank(int square)
{
    //square / 8
    return square >> 3;
}
int bitboard::SquareToFile(int square)
{
    //square % 8
    return square & 7;
}

U64 bitboard::HashByRFCross(U64 board, int square)
{
    return 
        (board >> (SquareToRank(square) * 8) & kRankMaskTable[0]) |
        (((board >> SquareToFile(square) & kFileMaskTable[0]) * kDiagonalMaskTable[7]) >> 56 << 8);
}

U64 bitboard::HashByDCross(U64 board, int square)
{
    return
        (((board & kDiagonalMaskTable[square]) * kFileMaskTable[0]) >> 56) |
        (((board & kAntiDiagonalMaskTable[square]) * kFileMaskTable[0]) >> 56 << 8);
}



void bitboard::InitMaskTable()
{

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
        PrintBoard(diagonals[level]);
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
            kAntiDiagonalMaskTable[rank * 8 + file] = anti_diagonals[kFileNum - 1 - rank + file];
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


U64 bitboard::GetAttackBoard(U64 board, int square)
{
    return kRFAttackTable[square][HashByRFCross(board, square)] | kDAttackTable[square][HashByDCross(board, square)];
}


void bitboard::InitAttackTable()
{
    for (int square = 0; square < kGridNum; ++square)
    {
        int rank = SquareToRank(square);
        int file = SquareToFile(square);

        for (U64 horizontal = 0; horizontal < (1ull << 8); ++horizontal)
        {
            for (U64 vertical = 0; vertical < (1ull << 8); ++vertical)
            {
                //0 is empty, 1 is occupied
                U64 enemy_board = (horizontal << (rank * 8)) | (((vertical * kDiagonalMaskTable[7]) & kFileMaskTable[7]) >> (7 - file));
                U64 attack_board = 0;


                //check attack square for up
                for (int x = rank - 1, dist = 0; x >= 0; --x, ++dist)
                {
                    int attack_square = x * 8 + file;

                    if ((enemy_board >> attack_square & 1) == 0)
                    {
                        if(dist > 0) attack_board |= 1ull << attack_square;
                        break;
                    }
                }

                //check attack square for down
                for (int x = rank + 1, dist = 0; x < kRankNum; ++x, ++dist)
                {
                    int attack_square = x * 8 + file;

                    if ((enemy_board >> attack_square & 1) == 0)
                    {
                        if (dist > 0) attack_board |= 1ull << attack_square;
                        break;
                    }
                }

                //check attack square for left
                for (int y = file - 1, dist = 0; y >= 0; --y, ++dist)
                {
                    int attack_square = rank * 8 + y;

                    if ((enemy_board >> attack_square & 1) == 0)
                    {
                        if (dist > 0) attack_board |= 1ull << attack_square;
                        break;
                    }
                }

                //check attack square for right
                for (int y = file + 1, dist = 0; y < kFileNum; ++y, ++dist)
                {
                    int attack_square = rank * 8 + y;

                    if ((enemy_board >> attack_square & 1) == 0)
                    {
                        if (dist > 0) attack_board |= 1ull << attack_square;
                        break;
                    }
                }

                kRFAttackTable[square][horizontal | vertical << 8] = attack_board;
            }
        }

        for (U64 diagonal = 0; diagonal < (1ull << 8); ++diagonal)
        {
            for (U64 anti_diagonal = 0; anti_diagonal < (1ull << 8); ++anti_diagonal)
            {
                U64 enemy_board = ((diagonal * bitboard::kFileMaskTable[0]) & bitboard::kDiagonalMaskTable[square]) | ((anti_diagonal * bitboard::kFileMaskTable[0]) & bitboard::kAntiDiagonalMaskTable[square]);
                U64 attack_board = 0;

                for (int x = rank - 1, y = file - 1, dist = 0; x >= 0 && y >= 0; --x, --y, ++dist)
                {
                    int attack_square = x * 8 + y;

                    if ((enemy_board >> attack_square & 1) == 0)
                    {
                        if (dist > 0) attack_board |= 1ull << attack_square;
                        break;
                    }
                }

                for (int x = rank - 1, y = file + 1, dist = 0; x >= 0 && y < kFileNum; --x, ++y, ++dist)
                {
                    int attack_square = x * 8 + y;

                    if ((enemy_board >> attack_square & 1) == 0)
                    {
                        if (dist > 0) attack_board |= 1ull << attack_square;
                        break;
                    }
                }

                for (int x = rank + 1, y = file - 1, dist = 0; x < kRankNum && y >= 0; ++x, --y, ++dist)
                {
                    int attack_square = x * 8 + y;

                    if ((enemy_board >> attack_square & 1) == 0)
                    {
                        if (dist > 0) attack_board |= 1ull << attack_square;
                        break;
                    }
                }

                for (int x = rank + 1, y = file + 1, dist = 0; x < kRankNum && y < kFileNum; ++x, ++y, ++dist)
                {
                    int attack_square = x * 8 + y;

                    if ((enemy_board >> attack_square & 1) == 0)
                    {
                        if (dist > 0) attack_board |= 1ull << attack_square;
                        break;
                    }
                }

                kDAttackTable[square][diagonal | anti_diagonal << 8] = attack_board;
            }
        }

    }
}