#include <stdlib.h>
#include <math.h>

short mad(short target[64], short ref[64])
{
    short sum = 0;
    for (int i = 0; i < 64; ++i)
        sum += abs(src[i] - target[i]);
    sum /= 64;
    return sum;
}

int find_min(int costs[9])
{
    int minimum = costs[0];
    location = 0;
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

int TSS(short t_blocks[][64], short ref_block[64],
        int step, int center, int mb_high, int mb_wide)
{
    // search start location
    int costs[9] =
    {65537, 65537, 65537, 65537, 65537, 65537, 65537, 65537, 65537};
    int locations[9];
    int loc, og, cx, cy, x, y;
    // calculate the first center
    // avoid recalculating the center within the loop
    costs[4] = mad(t_blocks[center], ref_block);
    locations[4] = center;

    while(step >= 1)
    {
        cy = center / mb_wide;
        cx = center % mb_wide;
        for(int i = 0; i < 3; ++i)
        {
            for(int j = 0; j < 3; ++j)
            {
                y = cy + (i-1) * step;
                x = cx + (j-1) * step;
                if(x < 0 || x > mb_wide || y < 0 || y > mb_high ||
                  (i == 1 && j == 1))
                {
                    costs[i*3 + j] = 65537;
                    continue;
                }

                costs[i*3 + j] = mad(t_blocks[y*mb_wide + x], ref_block);
                locations[i*3 + j] = y*mb_wide + x;
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

void TSS_ALL(int vectors[][2], short target[][64], short ref[][64],
             int param, int mb_high, int mb_wide)
{
    int res, oldx, oldy, newx, newy;

    // take log2 of param to find the number of steps required
    double L = floor(log2(param + 1.0));
    double stepMax = pow(2.0, (L-1.0));
    int step = (int)stepMax;

    for(int i = 0; i < mb_high * mb_wide; ++i)
    {
        res = TSS(target, ref[i], i, mb_high, mb_wide);
        oldy = i / mb_wide;
        oldx = i % mb_wide;
        newy = res / mb_wide;
        newx = res % mb_wide;
        vectors[i][0] = newy - oldy;
        vectors[i][1] = newx - oldx;
    }
}

