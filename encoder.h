#ifndef dct_h_
#define dct_h_

#include <stdio.h>

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
void TSS_ALL(short vectors[][2], short target[][64], short ref[][64],
             int param, int mb_high, int mb_wide);

void motion_compensation(short blocks[][64], short comp_image[][64],
                         short vectors[][2], int mb_high, int mb_wide);


// File I/O
int readHeader(FILE *fp, int* width, int* height, int* fr, int* inl, int* par);
int readFrame(FILE *fp, char* buffer, int sqtype);
int imwrite(FILE *fp, char* buffer, int sqtype);
#endif
