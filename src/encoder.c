//#include <opencv2/opencv.hpp>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include "encoder.h"

int main(int argc, char** argv){
    int A[64];
    double B[64] =
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
    int level[64];
    char symbol;
    char runlen[400];
    int amp;
    int nbytes;
    char SIGN_MASK = 0x80;
    char sign;
    DCT2(B);
    QUANT_intra(A, B, 1.5);
    zigzag_scan_8x8(level, A);
    runlength(runlen, level);
    for(int i = 0; i < 8; ++i)
    {
        for(int j = 0; j < 8; ++j)
        {
             printf("%d ", level[i*8 + j]);
        }
        printf("\n");
    }
    printf("\n");

    int i = 0;
    while(runlen[i] != '\0')
    {
        symbol = runlen[i];
        if(symbol == 0x0f){
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
        printf("(%d)\n", amp);
    }
    return 0;
}
