#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "encoder.h"

/*
 * H.261 uses a constant step size instead of a quantization matrix 
 */
void QUANT_intra(int A[8][8], const double B[8][8], const double step_size)
{
    for(int i = 0; i < 8; ++i)
        for(int j = 0; j < 8; ++j)
            A[i][j] = (int)(B[i][j] / step_size);
}

void QUANT_inter(int A[8][8], const double B[8][8], const double step_size)
{
    for(int i = 0; i < 8; ++i)
        for(int j = 0; j < 8; ++j)
            A[i][j] = round(B[i][j] / step_size);
}

void IQUANT(double A[8][8], const int B[8][8], const double step_size)
{
    for(int i = 0; i < 8; ++i)
        for(int j = 0; j < 8; ++j)
            A[i][j] = B[i][j] * step_size;
}

void zigzag_scan_8x8(int level[64], const int B[8][8])
{
    // copy first half
    int count = 0;
    for(int i = 0; i < 8; ++i)
    {
        for(int j = 0; j <= i; ++j)
        {
            if(i % 2 == 0)
                level[count] = B[i-j][j];
            else
                level[count] = B[j][i-j];
            ++count;
        }
    }

    // copy second half
    for(int i = 0; i < 7; ++i)
    {
        for(int j = 7; j > i; --j)
        {
            if(i % 2 == 0)
                level[count] = B[j][8 - j + i];
            else
                level[count] = B[8 - j + i][j];
            ++count;
        }
    }
}

void runlength(char rl[400], const int level[64])
{
    int nbytes;
    int nbits;
    int zeros = 0;
    int idx = 0;
    char symbol = 0x00;
    int amp;

    for(int i = 0; i < 64; ++i){
        if((level[i] == 0) && (zeros < 15))
            ++zeros;
        else if((level[i] == 0) && (zeros == 15)){
            symbol |= 0xf << 4;
            printf("symbol: (%d,%d)\n", (int)(symbol >> 4), (int)(symbol & 0x0f));
            rl[idx] = symbol;
            symbol = 0x00;
            zeros = 0;
        }
        else
        {
            // add the last four bits of the zero count
            symbol |= (zeros & 0xf) << 4; 
            zeros = 0;

            // add the size of the amplitude (in bytes for now)
            // TODO: in future, should be in bits ???
            amp = level[i];
            nbits = floor(log2(abs((double)level[i]))) + 2;
            nbytes = ceil(nbits / 8.0);
            symbol |= (nbits & 0xf);
            printf("symbol: (%d,%d)(%d)\n",
                   (int)(symbol >> 4),
                   (int)(symbol & 0x0f),
                   amp);
            rl[idx] = symbol;
            symbol = 0x00;
            ++idx;

            // add the amplitude one byte at a time
            for (int j = 0; j < nbytes; ++j)
                rl[idx+j] = ((unsigned char *)&amp)[j]; 
            idx += nbytes;
        }
    }

    // coalesce zeroes at the end
    while(rl[idx] == 0xf0)
        --idx;

    rl[idx] = 0x00;
    rl[idx+1] = '\0';
}
