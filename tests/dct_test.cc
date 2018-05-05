#include <stdlib.h>
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

        const double B[64] =
        {
            1, 1, 1, 1, 0, 0, 0, 0,
            1, 1, 1, 0, 0, 0, 0, 0,
            1, 1, 0, 0, 0, 0, 0, 0,
            1, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0
        };

        const double Expected[64] =
        {
            1.2500, 1.3654, 0.5576, 0.0395,      0, 0.0887, 0.0396,-0.0361,
            1.3654, 1.4202, 0.4186,-0.1633,-0.1125, 0.0676, 0.0230,-0.0676,
            0.5576, 0.4186,-0.2500,-0.4937,-0.2310, 0.0292,      0,-0.0833,
            0.0395,-0.1633,-0.4937,-0.4418,-0.0637, 0.1633, 0.0609,-0.0676,
                 0,-0.1125,-0.2310,-0.0637, 0.2500, 0.3204, 0.0957,-0.0752,
            0.0887, 0.0676, 0.0292, 0.1633, 0.3204, 0.2347,-0.0407,-0.1633,
            0.0396, 0.0230,-0.0000, 0.0609, 0.0957,-0.0407,-0.2500,-0.2595,
           -0.0361,-0.0676,-0.0833,-0.0676,-0.0752,-0.1633,-0.2595,-0.2130
        };

        const double threshold = 0.00001;
    };


    TEST_F(DctTest, forward_transform)
    {
        double A[64]; 

        memcpy(A, B, sizeof(double) * 64);
        DCT2(A);
        for(int i = 0; i < 8; ++i)
            for(int j = 0; j < 8; ++j)
                ASSERT_LT(abs(A[i*8 + j] - Expected[i*8 + j]), threshold);
    }

    TEST_F(DctTest, inverse_transform)
    {
        double C[64]; 
        memcpy(C, Expected, sizeof(double) * 64);
        IDCT2(C);
        for(int i = 0; i < 8; ++i)
            for(int j = 0; j < 8; ++j)
                ASSERT_LT(abs(C[i*8 + j] - B[i*8 + j]), threshold);
    }
}
