#ifndef dct_h_
#define dct_h_

#include <stdio.h>

#include "defs.h"

// Forward / Inverse 8x8 DCT transform
void idct(short blk[64]);
int init_idct();
void dct(short blk[64]);


// Forward / Inverse 8x8 Quantizer
void QUANT(short blk[64], const short step_size);
void IQUANT(short blk[64], const short step_size);


// Runlength encoding functions
void zigzag_scan_8x8(short level[64], const short B[64]);
void runlength(unsigned char rl[400], const short level[64]);


// Motion Estimation / Compensation
void TSS_ALL(int vectors[][2], struct pblock_t trg[],
             struct pblock_t ref[], int param, int pb_high, int pb_wide);
void create_pred_image(struct pblock_t pred_blocks[], struct pblock_t trg[],
                       int vectors[][2], int pb_high, int pb_wide);
void motion_compensation(struct pblock_t ref[], struct pblock_t pred_blocks[],
                         int pb_high, int pb_wide);


// File I/O
int readHeader(FILE *fp, int* width, int* height, int* fr, int* inl, int* par);
int readFrame(FILE *fp, unsigned char* buffer, int sqtype);
int skipFrame(FILE *fp, int n);
int imwrite(FILE *fp, unsigned char* buffer, int sqtype);


// Macroblock trasformation and grouping
void blockify(short blocks[][64], const unsigned char* frame,
              int height, int width);
void unblockify(unsigned char* frame, const short blocks[][64], 
                int height, int width);
int tb2pb(struct pblock_t pblks[], short blocks[][64], int sqtype);
int pb2tb(short blocks[][64], struct pblock_t pblks[], int sqtype);
#endif
