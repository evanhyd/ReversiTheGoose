#include "bitboard.h"
#include <iostream>
#include <bitset>

U64 bitboard::kRankMaskTable[kGridNum] = { 0 };
U64 bitboard::kFileMaskTable[kGridNum] = { 0 };
U64 bitboard::kDiagonalMaskTable[kGridNum] = { 0 };
U64 bitboard::kAntiDiagonalMaskTable[kGridNum] = { 0 };
U64 bitboard::kRFCrossMaskTable[kGridNum] = { 0 };
U64 bitboard::kDCrossMaskTable[kGridNum] = { 0 };
U64 bitboard::kCrossMaskTable[kGridNum] = { 0 };

U64 bitboard::kRFAttackTable[kGridNum][1 << 16] = { 0 };
U64 bitboard::kDAttackTable[kGridNum][1 << 16] = { 0 };

U64 bitboard::kFlipMaskTable[kGridNum][kGridNum] = { 0 };


int bitboard::SquareToRank(int square)
{
    //divided by 8
    return square >> 3;
}
int bitboard::SquareToFile(int square)
{
    //modulo 8
    return square & 7;
}

bool bitboard::IsSetBit(U64 board, int square)
{
    return board >> square & 1;
}

U64 bitboard::SetBit(U64 board, int square)
{
    return board | 1ull << square;
}

U64 bitboard::FlipBit(U64 board, int square)
{
    return board ^ 1ull << square;
}

U64 bitboard::ClearBit(U64 board, int square)
{
    return board & ~(1ull << square);
}

U64 bitboard::PopBit(U64 board)
{
    return board & (board - 1);
}

int bitboard::CountSetBit(U64 board)
{
    return std::popcount(board);
}




/*
Benchmark

20 millions
MSVC: 1160 1166 1175 1176 1184 1211 1227
STD:  1154 1159 1184 1217 1222 1229 1247
AMD:  1160 1184 1201 1219 1228 1229 1230

50 millions
MSVC: 2873 2878 3018 3076 3133 3155 3168
STD:  2869 3034 3040 3084 3093 3129 3155
AMD:  2917 2926 3069 3092 3105 3106 3178

100 millions
MSVC: 5746
STD: 5770
AMD: 5733
*/

int bitboard::GetLSTSetBit(U64 board)
{
    return (board != 0 ? (std::popcount(board ^ (board - 1)) - 1) : -1);
}

int bitboard::GetLSTSetBit_STD(U64 board)
{
    return (board != 0 ? (std::popcount(board ^ (board - 1)) - 1) : -1);
}

int bitboard::GetLSTSetBit_MSVC(U64 board)
{
    return (board != 0 ? static_cast<int>(__popcnt64(board ^ (board - 1)) - 1) : -1);
}

void bitboard::GetLSTSetBit_AMD(unsigned long* res, U64 board)
{
    auto is_non_zero = _BitScanForward64(res, board);
    if (!is_non_zero) *res = 64;
}


void bitboard::PrintBoard(U64 board)
{
    for (int rank = 0; rank < kRankLength; ++rank)
    {
        for (int file = 0; file < kFileLength; ++file)
        {
            std::cout << (board & 1) << ' ';
            board >>= 1;
        }

        std::cout << '\n';
    }
    std::cout << '\n';
}



U64 bitboard::HashByRFCross(U64 board, int square)
{
    int rank = SquareToRank(square);
    int file = SquareToFile(square);

    U64 first_rank = board >> (rank * 8) & kFirstRankMask;
    U64 second_rank = ((board >> file & kFirstFileMask) * kMainDiagonalMask) >> 56 << 8;

    return first_rank | second_rank;
}

U64 bitboard::HashByDCross(U64 board, int square)
{
    U64 first_rank = ((board & kDiagonalMaskTable[square]) * kFirstFileMask) >> 56;
    U64 second_rank = ((board & kAntiDiagonalMaskTable[square]) * kFirstFileMask) >> 56 << 8;

    return first_rank | second_rank;
}

U64 bitboard::UnhashByRFCross(U64 hash, int square)
{
    int rank = SquareToRank(square);
    int file = SquareToFile(square);

    U64 horizontal = (hash & kFirstRankMask) << (rank * 8);
    U64 vertical = (((hash >> 8) * kMainDiagonalMask) & kLastFileMask) >> (7 - file);

    return horizontal | vertical;
}

U64 bitboard::UnhashByDCross(U64 hash, int square)
{
    U64 diagonal = ((hash & kFirstRankMask) * kFirstFileMask) & kDiagonalMaskTable[square];
    U64 anti_diagonal = ((hash >> 8) * kFirstFileMask) & kAntiDiagonalMaskTable[square];

    return diagonal | anti_diagonal;
}

void bitboard::InitMaskTable()
{

    //initialize rank table
    U64 rank_mask = 0b11111111;
    for (int rank = 0; rank < kRankLength; ++rank)
    {
        for (int file = 0; file < kFileLength; ++file)
        {
            kRankMaskTable[rank * 8 + file] = rank_mask;
        }

        rank_mask <<= 8;
    }



    //initialize file table
    U64 file_mask = 1ull | 1ull << 8 | 1ull << 16 | 1ull << 24 | 1ull << 32 | 1ull << 40 | 1ull << 48 | 1ull << 56;
    for (int file = 0; file < kFileLength; ++file)
    {
        for (int rank = 0; rank < kRankLength; ++rank)
        {
            kFileMaskTable[rank * 8 + file] = file_mask;
        }

        file_mask <<= 1;
    }



    //initialize diagonal table
    constexpr int kMaxLevel = kRankLength + kFileLength - 1;
    U64 diagonals[kMaxLevel] = { 1 };
    for (int level = 1; level < kMaxLevel; ++level)
    {
        diagonals[level] = diagonals[level - 1] << 1 & ~kFirstFileMask | diagonals[level - 1] << 8;
    }

    for (int rank = 0; rank < kRankLength; ++rank)
    {
        for (int file = 0; file < kFileLength; ++file)
        {
            kDiagonalMaskTable[rank * 8 + file] = diagonals[rank + file];
        }
    }


    
    //initialize anti diagonal table
    U64 anti_diagonals[kMaxLevel] = { 1ull << 56 };
    for (int level = 1; level < kMaxLevel; ++level)
    {
        anti_diagonals[level] = anti_diagonals[level - 1] << 1 & ~kFirstFileMask | anti_diagonals[level - 1] >> 8;
    }

    for (int rank = 0; rank < kRankLength; ++rank)
    {
        for (int file = 0; file < kFileLength; ++file)
        {
            kAntiDiagonalMaskTable[rank * 8 + file] = anti_diagonals[kFileLength - 1 - rank + file];
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


//returns a board with all attackable squares in it
U64 bitboard::GetAttackBoard(U64 enemy_board, int square)
{
    return kRFAttackTable[square][HashByRFCross(enemy_board, square)] | kDAttackTable[square][HashByDCross(enemy_board, square)];
}


//return a board after the flips
U64 bitboard::FlipBoard(U64 board, int srce, int dest)
{
    return board ^ kFlipMaskTable[srce][dest];
}


//initializes the two attack tables (Rank+File and Diagonals), where the index is the 16 bit hash and the value is the 8x8 board with the attack pieces in them
void bitboard::InitAttackTable()
{
    for (int square = 0; square < kGridNum; ++square) 
    {
        int rank = SquareToRank(square);
        int file = SquareToFile(square);

        //right now there are 4 times as many tables as needed because the horizontal and vertical values at the target square's position don't matter.
        for (U64 horizontal = 0; horizontal < (1ull << 8); ++horizontal) 
        {
            for (U64 vertical = 0; vertical < (1ull << 8); ++vertical)
            {
                //0 is empty, 1 is occupied
                U64 hash = horizontal | vertical << 8;
                U64 enemy_board = UnhashByRFCross(hash, square);
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
                for (int x = rank + 1, dist = 0; x < kRankLength; ++x, ++dist)
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
                for (int y = file + 1, dist = 0; y < kFileLength; ++y, ++dist)
                {
                    int attack_square = rank * 8 + y;

                    if ((enemy_board >> attack_square & 1) == 0)
                    {
                        if (dist > 0) attack_board |= 1ull << attack_square;
                        break;
                    }
                }

                kRFAttackTable[square][hash] = attack_board;
            }
        }

        for (U64 diagonal = 0; diagonal < (1ull << 8); ++diagonal)
        {
            for (U64 anti_diagonal = 0; anti_diagonal < (1ull << 8); ++anti_diagonal)
            {
                U64 hash = diagonal | anti_diagonal << 8;
                U64 enemy_board = UnhashByDCross(hash, square);
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

                for (int x = rank - 1, y = file + 1, dist = 0; x >= 0 && y < kFileLength; --x, ++y, ++dist)
                {
                    int attack_square = x * 8 + y;

                    if ((enemy_board >> attack_square & 1) == 0)
                    {
                        if (dist > 0) attack_board |= 1ull << attack_square;
                        break;
                    }
                }

                for (int x = rank + 1, y = file - 1, dist = 0; x < kRankLength && y >= 0; ++x, --y, ++dist)
                {
                    int attack_square = x * 8 + y;

                    if ((enemy_board >> attack_square & 1) == 0)
                    {
                        if (dist > 0) attack_board |= 1ull << attack_square;
                        break;
                    }
                }

                for (int x = rank + 1, y = file + 1, dist = 0; x < kRankLength && y < kFileLength; ++x, ++y, ++dist)
                {
                    int attack_square = x * 8 + y;

                    if ((enemy_board >> attack_square & 1) == 0)
                    {
                        if (dist > 0) attack_board |= 1ull << attack_square;
                        break;
                    }
                }

                kDAttackTable[square][hash] = attack_board;
            }
        }

    }
}

void bitboard::InitFlipTable()
{
    for (int srce = 0; srce < kGridNum; ++srce) {
        for (int dest = 0; dest < kGridNum; ++dest) {

            U64 flip_board = 0;

            //save time for duplicate values
            if (dest < srce) {
                kFlipMaskTable[srce][dest] = kFlipMaskTable[dest][srce];
            }
            else {

                //get ranks and files
                int srce_rank = bitboard::SquareToRank(srce);
                int srce_file = bitboard::SquareToFile(srce);
                int dest_rank = bitboard::SquareToRank(dest);
                int dest_file = bitboard::SquareToFile(dest);

                //mask for when they are vertically aligned (file is the same)
                if (srce_file == dest_file && dest_rank - srce_rank >= 2) {

                    //loop through the positions inbetween srce and dest and add them to the map
                    for (int rank = srce_rank + 1; rank < dest_rank; ++rank) {
                        flip_board <<= 8;
                        flip_board |= 1ull;
                    }

                    //shift the map to the correct starting file, then the correct starting rank
                    flip_board <<= srce_file;
                    flip_board <<= 8 * (srce_rank + 1);

                //mask for when they are horizontally aligned (rank is the same)
                } else if (srce_rank == dest_rank && dest_file - srce_file >= 2) {

                    //loop through the positions inbetween srce and dest and add them to the map
                    for (int file = srce_file + 1; file < dest_file; ++file) {
                        flip_board <<= 1;
                        flip_board |= 1ull;
                    }

                    //shift the map to the correct starting file, then the correct starting rank
                    flip_board <<= srce_file + 1;
                    flip_board <<= 8 * srce_rank;


                //mask for main diagonal
                } else if (dest_rank - srce_rank == dest_file - srce_file) {

                    //loop through the positions between srce and dest and add them to the map
                    for (int pos = srce_file + 1; pos < dest_file; ++pos) {
                        flip_board <<= 9;
                        flip_board |= 1ull;
                    }

                    //shift the map to the correct starting file, then the correct starting rank
                    flip_board <<= srce_file + 1;
                    flip_board <<= 8 * (srce_rank + 1);

                //mask for anti-diagonal
                } else if (dest_rank + dest_file == srce_rank + srce_file) {

                    //loop through the positions between srce and dest and add them to the map
                    for (int pos = dest_file + 1; pos < srce_file; ++pos) {
                        flip_board |= 1ull; 
                        flip_board <<= 7;
                    }

                    //shift the map to the correct starting file, then the correct starting rank
                    flip_board >>= kFileLength - srce_file;
                    flip_board <<= 8 * (srce_rank + 1);
                }

                kFlipMaskTable[srce][dest] = flip_board;

            }
        }
    }
}