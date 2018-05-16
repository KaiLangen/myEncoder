#include "encoder.h"

void motion_compensation(pxl ref[], pxl trg[], int vectors[][2], 
                         int height, int width)
{
    int offset, offU, offV, motion, mbStart;
    int pb_high = height / 16;
    int pb_wide = width / 16;
    for (int i = 0; i < pb_high; ++i)
    {
        for (int j = 0; j < pb_wide; ++j)
        {
            mbStart = i*pb_wide + j;
            motion = vectors[mbStart][0] * width + vectors[mbStart][1];
            // luma components
            for (int m = 0; m < 16; ++m)
            {
                for (int n = 0; n < 16; ++n)
                {
                    // offset = MB start + row offset + col offset
                    offset = (i*16 + m)*width + j*16 + n;
                    ref[offset] -= trg[offset + motion];
                }
            }

            // chroma components
            offset = (width*height);
            motion = vectors[mbStart][0] * width / 4 + vectors[mbStart][1] / 2;
            for (int m = 0; m < 8; ++m)
            {
                for (int n = 0; n < 8; ++n)
                {
                    // offset = MB start + row offset + col offset
                    offU = offset + (i*8+m)*width/2 + (j*8) + n;
                    offV = offU + width*height/4;
                    ref[offU] -= trg[offU + motion];
                    ref[offV] -= trg[offV + motion];
                }
            }
        }
    }
}


void create_pred_image(pxl pred[], pxl trg[], int vectors[][2], 
                       int height, int width)
{
    int offset, offU, offV, motion, mbStart;
    int pb_high = height / 16;
    int pb_wide = width / 16;
    for (int i = 0; i < pb_high; ++i)
    {
        for (int j = 0; j < pb_wide; ++j)
        {
            mbStart = i*pb_wide + j;
            motion = vectors[mbStart][0] * width + vectors[mbStart][1];
            for (int m = 0; m < 16; ++m)
            {
                for (int n = 0; n < 16; ++n)
                {
                    // offset = MB start + row offset + col offset
                    offset = (i*16 + m)*width + j*16 + n;
                    pred[offset] = trg[offset + motion];
                }
            }
            
            // chroma components
            offset = (width*height);
            motion = vectors[mbStart][0] * width / 4 + vectors[mbStart][1] / 2;
            for (int m = 0; m < 8; ++m)
            {
                for (int n = 0; n < 8; ++n)
                {
                    // offset = MB start + row offset + col offset
                    offU = offset + (i*8+m)*width/2 + (j*8) + n;
                    offV = offU + width*height/4;
                    pred[offU] = trg[offU + motion];
                    pred[offV] = trg[offV + motion];
                }
            }
        }
    }
}
