#include <string.h>

/*
 * blockify: Transform a Frame into an array of MacroBlocks
 *
 * Frame is padded with zeroes to ensure it's width
 * and height are multiples of 8.
 * */
void blockify(char blocks[][64], const char* frame,
              const int  height, const int width){
    int mb_width = width / 8;
    int mb_height = height / 8;
    for(int i = 0; i < mb_height; ++i)
    {
        for(int j = 0; j < mb_width; ++j)
        {
            for(int k = 0; k < 8; ++k)
                memcpy(&blocks[i*mb_width + j][k*8], frame, 8);
        }
    }
}

void doublify(double dest[64], const char src[64]){
    for(int i = 0; i < 64; ++i)
        dest[i] = ((int)src[i] / 255.0);
}
