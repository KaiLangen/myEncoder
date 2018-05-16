#include <stdlib.h>
#include <math.h>
#include <limits.h>

#include "encoder.h"

static short mad(pxl ref[], pxl trg[], int refStart,
                 int trgStart, int height, int width)
{
    int offset, offU, offV;
    short sum = 0;
    /* Sum up the Y blocks */
    for (int i = 0; i < 16; ++i)
    {
        for (int j = 0; j < 16; ++j)
        {
            offset = i*width + j;
            sum += abs(ref[refStart + offset] - trg[trgStart + offset]);
        }
    }

    offset = (width*height);
    for (int i = 0; i < 8; ++i)
    {
        for (int j = 0; j < 8; ++j)
        {
            offU = offset + i*width/2 + j;
            offV = offU + width*height/4;
            sum += abs(ref[refStart/4 + offU] - trg[trgStart/4 + offU]);
            sum += abs(ref[refStart/4 + offV] - trg[trgStart/4 + offV]);
        }
    }

    return sum;
}

static int find_min(unsigned short costs[9])
{
    unsigned short minimum = costs[0];
    int location = 0;
    for(int c = 1; c < 9; ++c)
    {
        if(costs[c] < minimum)
        {
            minimum = costs[c];
            location = c;
        }
    }
    return location;
}

static int TSS(pxl ref[], pxl trg[],
        int step, int center, int height, int width)
{
    // search start location
    unsigned short costs[9] = {USHRT_MAX, USHRT_MAX, USHRT_MAX, USHRT_MAX,
                   USHRT_MAX, USHRT_MAX, USHRT_MAX, USHRT_MAX, USHRT_MAX};
    int locations[9] = {-1, -1, -1, -1, -1, -1, -1, -1, -1};
    int loc, og, cx, cy, x, y;
    // calculate the first center
    // avoid recalculating the center within the loop
    costs[4] = mad(ref, trg, center, center, height, width);
    locations[4] = center;

    while(step >= 1)
    {
        // center pb coordinates in the image = (cy, cx)
        cy = center / width;
        cx = center % width;
        // coordinates in the cost matrix = (i,j)
        for(int i = 0; i < 3; ++i)
        {
            for(int j = 0; j < 3; ++j)
            {
                // the 9 pts formed by stepping away from the center = (y, x)
                //
                // (cy-step, cx-step), (cy-step,      cx), (cy-step, cx+step)
                // (cy     , cx-step), (cy     ,      cx), (cy     , cx+step)
                // (cy+step, cx-step), (cy+step,      cx), (cy+step, cx+step)
                y = cy + (i-1) * step;
                x = cx + (j-1) * step;

                // check if the pt coordinates fall outside of the image
                if(x < 0 || (x+16) > width || y < 0 || (y+16) > height ||
                  (i == 1 && j == 1))
                {
//                    costs[i*3 + j] = USHRT_MAX;
                    continue;
                }
                locations[i*3+j] = y*width + x;
                costs[i*3+j] = mad(ref, trg, center, y*width+x, height, width);
            }
        }

        // re-center the search window on the local minimum
        loc = find_min(costs);
        center = locations[loc];
        step /= 2;

        // set the center and location
        costs[4] = costs[loc];
        locations[4] = center;
    }
    return center;
}

void TSS_ALL(int vectors[][2], pxl ref[], pxl trg[],
             int param, int height, int width)
{
    int res, newx, newy, idx;

    // take log2 of param to find the number of steps required
    double L = floor(log2(param + 1.0));
    double stepMax = pow(2.0, (L-1.0));
    int step = (int)stepMax;

    for(int i = 0; i < height; i += 16)
    {
        for(int j = 0; j < width; j += 16)
        {
            res = TSS(ref, trg, step, i*width+j, height, width);
            idx = (i*width/16 + j)/16;
            newy = res / width;
            newx = res % width;
            vectors[idx][0] = newy - i;
            vectors[idx][1] = newx - j;
        }
    }
}
