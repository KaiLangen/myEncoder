#include <string.h>

/*
 * blockify: Transform a Frame into an array of MacroBlocks
 *
 * Frame is padded with zeroes to ensure it's width
 * and height are multiples of 8.
 * */
void blockify(char blocks[][64], const char* frame,
              int mb_high, int mb_wide)
{
    for(int i = 0; i < mb_high; ++i)
        for(int j = 0; j < mb_wide; ++j)
            for(int k = 0; k < 8; ++k)
                memcpy(&blocks[i*mb_wide + j][k*8],
                       frame[(i*8 + k)*mb_wide + j*8], 8);
}

void unblockify(char* frame, const char blocks[][64],
                int mb_high, int width)
{
    for(int i = 0; i < mb_high; ++i)
        for(int j = 0; j < mb_wide; ++j)
            for(int k = 0; k < 8; ++k)
                memcpy(frame[(i*8 + k)*mb_wide + j*8],
                       &blocks[i*mb_wide + j][k*8], 8);
}

void shortify(short dest[64], const char src[64]){
    for(int i = 0; i < 64; ++i)
        dest[i] = src[i];
}
