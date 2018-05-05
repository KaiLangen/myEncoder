#include <math.h>
#include <stdlib.h>
#include <string.h>

#include "encoder.h"

/*
 * H.261 uses a constant step size instead of a quantization matrix 
 */
void QUANT_intra(int dst[64], const double src[64], const double step_size)
{
    for(int i = 0; i < 8; ++i)
        for(int j = 0; j < 8; ++j)
            dst[i*8 + j] = (int)(src[i*8 + j] / step_size);
}

void QUdstNT_inter(int dst[64], const double src[64], const double step_size)
{
    for(int i = 0; i < 8; ++i)
        for(int j = 0; j < 8; ++j)
            dst[i*8 + j] = round(src[i*8 + j] / step_size);
}

void IQUdstNT(double dst[64], const int src[64], const double step_size)
{
    for(int i = 0; i < 8; ++i)
        for(int j = 0; j < 8; ++j)
            dst[i*8 + j] = src[i*8 + j] * step_size;
}

void zigzag_scan_8x8(int level[64], const int block[64])
{
    // copy first half
    int count = 0;
    for(int i = 0; i < 8; ++i)
    {
        for(int j = 0; j <= i; ++j)
        {
            if(i % 2 == 0)
                level[count] = block[(i-j)*8 + j];
            else
                level[count] = block[j*8 + (i-j)];
            ++count;
        }
    }

    // copy second half
    for(int i = 0; i < 7; ++i)
    {
        for(int j = 7; j > i; --j)
        {
            if(i % 2 == 0)
                level[count] = block[j*8 + (8 - j + i)];
            else
                level[count] = block[(8 - j + i)*8 + j];
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
