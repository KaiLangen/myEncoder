#include <stdio.h>
#include <string.h>

#include "encoder.h"

/*
 * blockify: Transform a Frame of chars into an array of short MacroBlocks
 *
 * Frame is padded with zeroes to ensure its width
 * and height are multiples of 8.
 * */
void blockify(short blocks[][64], const unsigned char* frame,
              int height, int width)
{
    /* calculate the number of transform block's fill the image */
    int tb_high = height / 8;
    int tb_wide = width / 8;

    // copy every 8x8 pixel block into a transform block
    for(int i = 0; i < tb_high; ++i)
        for(int j = 0; j < tb_wide; ++j)
            for(int m = 0; m < 8; ++m)
                for (int n = 0; n < 8; ++n)
                    blocks[i*tb_wide + j][m*8 + n] =
                        frame[(i*8 + m)*width + j*8 + n];
}

void unblockify(unsigned char* frame, const short blocks[][64],
                int height, int width)
{
    /* calculate the number of tb's fill the image */
    int tb_high = height / 8;
    int tb_wide = width / 8;

    // copy every TB value back into the frame
    for(int i = 0; i < tb_high; ++i)
        for(int j = 0; j < tb_wide; ++j)
            for(int m = 0; m < 8; ++m)
                for (int n = 0; n < 8; ++n)
                    frame[(i*8 + m)*width + j*8 + n] =
                        (unsigned char)blocks[i*tb_wide + j][m*8 + n];
}


int tb2pb(struct pblock_t pblks[], short blocks[][64], int sqtype)
{
    int luma_w, luma_h, pb_wide, pb_high, tb_wide, tb_high;
    short (*chromU)[64], (*chromV)[64];
    if (sqtype == CIF)
    {
        luma_h = CIF_HEIGHT;
        luma_w = CIF_WIDTH;

    }
    else if (sqtype == QCIF)
    {
        luma_h = QCIF_HEIGHT;
        luma_w = QCIF_WIDTH;
    }
    else
    {
        fprintf(stderr, "Invalid sqtype: %d", sqtype);
        return -1;
    }
    pb_high = luma_h / 16;
    pb_wide = luma_w / 16;
    tb_high = luma_h / 8;
    tb_wide = luma_w / 8;

    chromU = blocks + tb_high * tb_wide;
    chromV = chromU + pb_high * pb_wide;

    for(int i = 0; i < pb_high; ++i)
    {
        for(int j = 0; j < pb_wide; ++j)
        {
           // group four Y blocks together to form a prediction block
           // Y1  Y2
           // Y3  Y4
           pblks[i*pb_wide + j].Y1 = blocks[i*2*tb_wide + j*2];
           pblks[i*pb_wide + j].Y2 = blocks[i*2*tb_wide + j*2+1];
           pblks[i*pb_wide + j].Y3 = blocks[(i*2+1)*tb_wide + j*2];
           pblks[i*pb_wide + j].Y4 = blocks[(i*2+1)*tb_wide + j*2+1];

           // 1 cr/u chrom block
           pblks[i*pb_wide + j].U = chromU[i*pb_wide + j];

           // 1 cb/v chrom block
           pblks[i*pb_wide + j].V = chromV[i*pb_wide + j];
        }
    }
}

int pb2tb(short blocks[][64], struct pblock_t pblks[], int sqtype)
{
    int luma_w, luma_h, pb_wide, pb_high, tb_wide, tb_high;
    short (*chromU)[64], (*chromV)[64];
    if (sqtype == CIF)
    {
        luma_h = CIF_HEIGHT;
        luma_w = CIF_WIDTH;

    }
    else if (sqtype == QCIF)
    {
        luma_h = QCIF_HEIGHT;
        luma_w = QCIF_WIDTH;
    }
    else
    {
        fprintf(stderr, "Invalid sqtype: %d", sqtype);
        return -1;
    }
    pb_high = luma_h / 16;
    pb_wide = luma_w / 16;
    tb_high = luma_h / 8;
    tb_wide = luma_w / 8;

    chromU = blocks + tb_high * tb_wide;
    chromV = chromU + pb_high * pb_wide;

    for(int i = 0; i < pb_high; ++i)
    {
        for(int j = 0; j < pb_wide; ++j)
        {
           // copy four Y blocks from prediction block
           // Y1  Y2
           // Y3  Y4
           memcpy(blocks[i*2*tb_wide + j*2],
                  pblks[i*pb_wide + j].Y1, sizeof(short)*64);
           memcpy(blocks[i*2*tb_wide + j*2+1],
                  pblks[i*pb_wide + j].Y2, sizeof(short)*64);
           memcpy(blocks[(i*2+1)*tb_wide + j*2],
                  pblks[i*pb_wide + j].Y3, sizeof(short)*64);
           memcpy(blocks[(i*2+1)*tb_wide + j*2+1],
                  pblks[i*pb_wide + j].Y4, sizeof(short)*64);

           // 1 cr/u chrom block
           memcpy(chromU[i*pb_wide + j],
                  pblks[i*pb_wide + j].U, sizeof(short)*64);

           // 1 cb/v chrom block
           memcpy(chromV[i*pb_wide + j],
                  pblks[i*pb_wide + j].V, sizeof(short)*64);
        }
    }
}
