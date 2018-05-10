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

        const short B[64] =
        {
             0,  1,  2,  3,  4,  5,  6,  7,
             8,  9, 10, 11, 12, 13, 14, 15,
            16, 17, 18, 19, 20, 21, 22, 23,
            24, 25, 26, 27, 28, 29, 30, 31,
            32, 33, 34, 35, 36, 37, 38, 39,
            40, 41, 42, 43, 44, 45, 46, 47,
            48, 49, 50, 51, 52, 53, 54, 55,
            56, 57, 58, 59, 60, 61, 62, 63
        };

        const short Expected[64] =
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

        const unsigned char symbol_table[5] =
        {0x08, 0x05, 0x08, 0x64, 0xA3};

        const int amp_table[5] =
        {126, -9, -72, -7, -2};

        const unsigned char SIGN_MASK = 0x80;
    };

    TEST_F(VlcTest, test_quant)
    {
        short A[64]; 
        memcpy(A, B, sizeof(short) * 64);

        QUANT(A, 32); 
        for(int i = 0; i < 4; ++i)
            for(int j = 0; j < 8; ++j)
                ASSERT_EQ(A[i*8 + j], 0);

        for(int i = 4; i < 8; ++i)
            for(int j = 0; j < 8; ++j)
                ASSERT_GT(A[i*8 + j], 0);
    }

    TEST_F(VlcTest, test_zigzag8)
    {
        short level[64]; 
        zigzag_scan_8x8(level, B); 
        for(int i = 0; i < 64; ++i)
            ASSERT_EQ(level[i], Expected[i]);
    }

    TEST_F(VlcTest, test_rle)
    {
        short A[64]; 
        short level[64]; 
        unsigned char runlen[400];
        int amp, nbytes;
        unsigned char sign, symbol;
        int i = 0;
        int count = 0;

        memcpy(A, B, sizeof(short) * 64);
        dct(A);
        QUANT(A, 2);
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
            printf("symbol: (%d,%d)",
                   (int)(symbol >> 4),
                   (int)(symbol & 0x0f));
            ASSERT_EQ(symbol, symbol_table[count]);
            ++i;

            // extract the amplitude
            amp = 0;
            for(int j = 0; j < nbytes; ++j)
                ((unsigned char *)&amp)[j] = runlen[i+j];
            i+=nbytes;

            // add the sign of the amp to the integer
            sign = ((unsigned char *)&amp)[nbytes - 1] & SIGN_MASK;
            for(int j = nbytes; j < 4; ++j)
                if(sign)
                    ((unsigned char *)&amp)[j] = 0xff;
                else
                    ((unsigned char *)&amp)[j] = 0x00;
            printf("(%d)\n", amp);
            ASSERT_EQ(amp, amp_table[count]);
            ++count;
        }

    }

}
