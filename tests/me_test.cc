#include <cstdlib>
#include <cstdio>
#include <string>

#include <gmock/gmock.h>
  using ::testing::Eq;
#include <gtest/gtest.h>
  using ::testing::Test;

// declare "C" functions
extern "C"
{
    #include "encoder.h"
    #include "config.h"
}

namespace testing
{
    class IOTest : public Test
    {
    protected:
        IOTest(){}
        ~IOTest(){}

        virtual void SetUp(){}
        virtual void TearDown(){}

        char const* foreman = "tests/data/foreman_qcif.y4m";
    };


    TEST_F(IOTest, read_header)
    {
        int result, width, height, fr, inl;
        int ar = 0;
        FILE* fp;
        char str[BUFF_SIZE];
        sprintf(str, "%s/%s", TOP_LVL_DIR, foreman);
        fp = fopen(str, "r");
        ASSERT_NE(fp, nullptr);

        result = readHeader(fp, &width, &height, &fr, &inl, &ar);
        ASSERT_FALSE(result);
        ASSERT_EQ(width, 176);
        ASSERT_EQ(height, 144);
        ASSERT_EQ(framerate[fr], 29.97);
        ASSERT_EQ(inl, IP);
        //TODO pixel aspect ratio at a later time
        ASSERT_EQ(ar, 0);

    }
    TEST_F(IOTest, read_frame)
    {
        unsigned char buffer1[QCIF_SIZE + 1];
        unsigned char buffer2[QCIF_SIZE + 1];
        FILE* fp;
        int result, width, height, fr, inl, ar;
        bool isDistinct = false;
        char str[BUFF_SIZE];

        // Check that there are no problems reading the files
        sprintf(str, "%s/%s", TOP_LVL_DIR, foreman);
        fp = fopen(str, "rb");

        result = readHeader(fp, &width, &height, &fr, &inl, &ar);

        result = (fp, buffer1, QCIF);
        ASSERT_FALSE(result);
        result = readFrame(fp, buffer2, QCIF);
        ASSERT_FALSE(result);

        // Check that consecutive calls to imread on the same video sequence
        // returns different frames.
        for (int i = 0; i < QCIF_SIZE; ++i)
        {
            if(buffer1[i] != buffer2[i])
            {
                isDistinct = true;
                break;
            }
        }
        ASSERT_TRUE(isDistinct);
    }

//    TEST_F(IOTest, imwrite_qcif{}
}
