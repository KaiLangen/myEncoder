#ifndef dct_h_
#define dct_h_

#include <stdio.h>

#include "defs.h"

typedef unsigned char pxl;

// Forward / Inverse 8x8 DCT transform
void idct(short blk[64]);
int init_idct();
void dct(short blk[64]);


// Forward / Inverse 8x8 Quantizer
void QUANT(short blk[64], const short step_size);
void IQUANT(short blk[64], const short step_size);


// Runlength encoding functions
void zigzag_scan_8x8(short level[64], const short B[64]);
void runlength(pxl rl[400], const short level[64]);


// Motion Estimation / Compensation
void TSS_ALL(int vectors[][2], pxl ref[], pxl trg[],
             int param, int height, int width);
void create_pred_image(pxl ref[], pxl trg[], int vectors[][2],
                       int height, int width);
void motion_compensation(pxl ref[], pxl trg[], int vectors[][2],
                         int height, int width);


// File I/O
int readHeader(FILE *fp, int* width, int* height, int* fr, int* inl, int* par);
int readFrame(FILE *fp, pxl* buffer, int sqtype);
int skipFrame(FILE *fp, int n);
int imwrite(FILE *fp, pxl* buffer, int sqtype);


// Macroblock trasformation and grouping
void blockify(short blocks[][64], const pxl* frame,
              int height, int width);
void unblockify(pxl* frame, const short blocks[][64], 
                int height, int width);
int tb2pb(struct pblock_t pblks[], short blocks[][64], int sqtype);
int pb2tb(short blocks[][64], struct pblock_t pblks[], int sqtype);
#endif
