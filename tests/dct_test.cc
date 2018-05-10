#include <cstdlib>
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
    class DctTest : public Test
    {
    protected:
        DctTest(){}
        ~DctTest(){}

        virtual void SetUp(){}
        virtual void TearDown(){}

        const short B[64] =
        {
          255, 0, 0, 10, 0, 0, 0, 0,
            0, 0, 0,  0, 0, 0, 0, 0,
            0, 0, 0,  0, 0, 0, 0, 0,
           10, 0, 0,  0, 0, 0, 0, 0,
            0, 0, 0,  0, 0, 0, 0, 0,
            0, 0, 0,  0, 0, 0, 0, 0,
            0, 0, 0,  0, 0, 0, 0, 0,
            0, 0, 0,  0, 0, 0, 0, 0
        };

        // computed DCT with Matlab, then converted to short using:
        // int16(floor(dct2(B)))
        const short Expected[64] =
        {
           34,  46,  41,  37,  34,  27,  17,   7,
           46,  62,  55,  51,  46,  37,  23,   9,
           41,  55,  50,  45,  41,  33,  20,   8,
           37,  51,  45,  41,  37,  30,  18,   8,
           34,  46,  41,  37,  34,  27,  17,   7,
           27,  37,  33,  30,  27,  21,  13,   5,
           17,  23,  20,  18,  17,  13,   8,   3,
            7,   9,   8,   8,   7,   5,   3,   1
        };

        const short threshold = 1;
    };


    TEST_F(DctTest, forward_transform)
    {
        short A[64]; 
        short e;

        memcpy(A, B, sizeof(short) * 64);
        dct(A);
        for(int i = 0; i < 8; ++i)
            for(int j = 0; j < 8; ++j)
                ASSERT_LE(abs(A[i*8 + j] - Expected[i*8 + j]), threshold);
    }

    TEST_F(DctTest, inverse_transform)
    {
        short C[64]; 
        short b;
        memcpy(C, Expected, sizeof(short) * 64);
        init_idct();
        idct(C);
        for(int i = 0; i < 8; ++i)
            for(int j = 0; j < 8; ++j)
            {
                if(i + j == 0) // inverse DC component is inaccurate
                    continue;
                ASSERT_LE(abs(C[i*8 + j] - B[i*8 + j]), threshold);
           }
    }
}
