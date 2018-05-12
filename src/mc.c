#include "encoder.h"

void create_pred_image(struct pblock_t pred_blks[], struct pblock_t trg[],
                       int vectors[][2], int pb_high, int pb_wide)
{
    int ref_block;
    for(int i = 0; i < pb_high * pb_wide; ++i)
    {
        // offset the reference block by the motion vector
        ref_block = i + vectors[i][0] * pb_wide + vectors[i][1];

        // copy the MB addresses in from target block into prediction block
        pred_blks[i] = trg[ref_block];
    }
}

void motion_compensation(struct pblock_t ref[], struct pblock_t pred_blocks[],
                         int pb_high, int pb_wide)
{
    for (int i = 0; i < pb_high * pb_wide; ++i)
    {
        for (int j = 0; j < 64; ++j)
        {
            ref[i].Y1[j] -= pred_blocks[i].Y1[j];
            ref[i].Y2[j] -= pred_blocks[i].Y2[j];
            ref[i].Y3[j] -= pred_blocks[i].Y3[j];
            ref[i].Y4[j] -= pred_blocks[i].Y4[j];
            ref[i].U[j] -= pred_blocks[i].U[j];
            ref[i].V[j] -= pred_blocks[i].V[j];
        }
    }
}
