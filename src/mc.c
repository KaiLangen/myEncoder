#include <string.h>

void motion_compensation(double blocks[][64], double comp_image[][64],
                         int vectors[][2], int mb_high, int mb_wide)
{
    int ref_block;
    for(int i = 0; i < mb_high * mb_wide)
    {
        // offset the reference block by the motion vector
        ref_block = i + vectors[i][0] * mb_wide + vectors[i][1];

        // copy the contents of ref into current block
        memcpy(comp_image[i], blocks[ref_block], sizeof(double) * 64);
    }
}
