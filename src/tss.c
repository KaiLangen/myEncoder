#include <stdlib.h>
#include <math.h>
#include <limits.h>

#include "encoder.h"

static short mad(struct pblock_t trg, struct pblock_t ref)
{
    short sum = 0;
    for (int i = 0; i < 64; ++i)
    {
        sum += abs(ref.Y1[i] - trg.Y1[i]);
        sum += abs(ref.Y2[i] - trg.Y2[i]);
        sum += abs(ref.Y3[i] - trg.Y3[i]);
        sum += abs(ref.Y4[i] - trg.Y4[i]);
        sum += abs(ref.U[i] - trg.U[i]);
        sum += abs(ref.V[i] - trg.V[i]);
    }
    sum /= (64*6);
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

static int TSS(struct pblock_t trg[], struct pblock_t ref,
        int step, int center, int pb_high, int pb_wide)
{
    // search start location
    unsigned short costs[9] = {USHRT_MAX, USHRT_MAX, USHRT_MAX, USHRT_MAX,
                   USHRT_MAX, USHRT_MAX, USHRT_MAX, USHRT_MAX, USHRT_MAX};
    int locations[9] = {-1, -1, -1, -1, -1, -1, -1, -1, -1};
    int loc, og, cx, cy, x, y;
    // calculate the first center
    // avoid recalculating the center within the loop
    costs[4] = mad(trg[center], ref);
    locations[4] = center;

    while(step >= 1)
    {
        // center pb coordinates in the image = (cy, cx)
        cy = center / pb_wide;
        cx = center % pb_wide;
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
                if(x < 0 || x >= pb_wide || y < 0 || y >= pb_high ||
                  (i == 1 && j == 1))
                {
//                    costs[i*3 + j] = USHRT_MAX;
                    continue;
                }

                costs[i*3 + j] = mad(trg[y*pb_wide + x], ref);
                locations[i*3 + j] = y*pb_wide + x;
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

void TSS_ALL(int vectors[][2], struct pblock_t target[],
             struct pblock_t ref[], int param, int pb_high, int pb_wide)
{
    int res, oldx, oldy, newx, newy;

    // take log2 of param to find the number of steps required
    double L = floor(log2(param + 1.0));
    double stepMax = pow(2.0, (L-1.0));
    int step = (int)stepMax;

    for(int i = 0; i < pb_high * pb_wide; ++i)
    {
        res = TSS(target, ref[i], step, i, pb_high, pb_wide);
        oldy = i / pb_wide;
        oldx = i % pb_wide;
        newy = res / pb_wide;
        newx = res % pb_wide;
        vectors[i][0] = newy - oldy;
        vectors[i][1] = newx - oldx;
    }
}
