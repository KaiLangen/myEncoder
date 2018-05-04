#include <cstdlib>
#include <iostream>
#include <string>

#include <gmock/gmock.h>
  using ::testing::Eq;
#include <gtest/gtest.h>
  using ::testing::Test;

// declare "C" functions
extern "C"
{
    #include "encoder.h"
}

namespace testing
{
    class VlcTest : public Test
    {
    protected:
        VlcTest(){}
        ~VlcTest(){}

        virtual void SetUp(){}
        virtual void TearDown(){}

        const int B[8][8] =
        {
            { 0,  1,  2,  3,  4,  5,  6,  7},
            { 8,  9, 10, 11, 12, 13, 14, 15},
            {16, 17, 18, 19, 20, 21, 22, 23},
            {24, 25, 26, 27, 28, 29, 30, 31},
            {32, 33, 34, 35, 36, 37, 38, 39},
            {40, 41, 42, 43, 44, 45, 46, 47},
            {48, 49, 50, 51, 52, 53, 54, 55},
            {56, 57, 58, 59, 60, 61, 62, 63}
        };

        double C[8][8] =
        {
            { 0,  1,  2,  3,  4,  5,  6,  7},
            { 8,  9, 10, 11, 12, 13, 14, 15},
            {16, 17, 18, 19, 20, 21, 22, 23},
            {24, 25, 26, 27, 28, 29, 30, 31},
            {32, 33, 34, 35, 36, 37, 38, 39},
            {40, 41, 42, 43, 44, 45, 46, 47},
            {48, 49, 50, 51, 52, 53, 54, 55},
            {56, 57, 58, 59, 60, 61, 62, 63}
        };

        const int Expected[64] =
        {
           0,  1,  8, 16,  9,  2,  3, 10, 
          17, 24, 32, 25, 18, 11,  4,  5, 
					12, 19, 26, 33, 40, 48, 41, 34, 
					27, 20, 13,  6,  7, 14, 21, 28, 
					35, 42, 49, 56, 57, 50, 43, 36, 
					29, 22, 15, 23, 30, 37, 44, 51, 
					58, 59, 52, 45, 38, 31, 39, 46, 
					53, 60, 61, 54, 47, 55, 62, 63
        };

        const char symbol_table[6] =
        {0x09, 0x08, 0x05, 0x35, 0x22, 0x53};

        const int amp_table[6] =
        {168, -97, -12, -10, -1, -3};

        const char SIGN_MASK = 0x80;
    };

    TEST_F(VlcTest, test_quant)
    {
        int A[8][8]; 
        for(int i = 0; i < 8; ++i)
            for(int j = 0; j < 8; ++j)
                C[i][j] = (double)B[i][j];

        QUANT_intra(A, C, 32.0); 
        for(int i = 0; i < 4; ++i)
            for(int j = 0; j < 8; ++j)
                ASSERT_EQ(A[i][j], 0);

        for(int i = 4; i < 8; ++i)
            for(int j = 0; j < 8; ++j)
                ASSERT_GT(A[i][j], 0);
    }

    TEST_F(VlcTest, test_zigzag8)
    {
        int level[64]; 
        zigzag_scan_8x8(level, B); 
        for(int i = 0; i < 64; ++i)
            ASSERT_EQ(level[i], Expected[i]);
    }

    TEST_F(VlcTest, test_rle)
    {
        int A[8][8]; 
        double D[8][8];
        int level[64]; 
        char runlen[400];
        int amp, nbytes;
        char sign, symbol;
        int i = 0;
        int count = 0;

        memcpy(D, C, sizeof(double) * 64);
        DCT2(D);
        QUANT_intra(A, D, 1.5);
        zigzag_scan_8x8(level, A); 
        runlength(runlen, level);

        while(runlen[i] != '\0')
        {
            symbol = runlen[i];
            if(symbol == 0x0f){
                ++count;
                ++i;
                continue;
            }

            if(symbol == 0x00){
                break;
            }

            nbytes = ceil((int)(symbol & 0x0f) / 8.0);
            ASSERT_EQ(symbol, symbol_table[count]);
            ++i;

            // extract the amplitude
            amp = 0;
            for(int j = 0; j < nbytes; ++j)
                ((unsigned char *)&amp)[j] = runlen[i+j];
            i+=nbytes;

            sign = ((unsigned char *)&amp)[nbytes - 1] & SIGN_MASK;
            for(int j = nbytes; j < 4; ++j)
                if(sign)
                    ((unsigned char *)&amp)[j] = 0xff;
                else
                    ((unsigned char *)&amp)[j] = 0x00;
            ASSERT_EQ(amp, amp_table[count]);
            ++count;
        }

    }

}
