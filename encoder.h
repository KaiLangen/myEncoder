#ifndef dct_h_
#define dct_h_

#include <stdio.h>

// Forward / Inverse 8x8 DCT transform
void DCT2(double A[64]);

void IDCT2(double A[64]);



// Forward / Inverse 8x8 Quantizer
void QUANT_intra(int A[64], const double B[64], const double step_size);

void QUANT_inter(int A[64], const double B[64], const double step_size);

void IQUANT(double A[64], const int B[64], const double step_size);



// Runlength encoding functions
void zigzag_scan_8x8(int level[64], const int B[64]);

void runlength(char rl[400], const int level[64]);



// Motion Estimation / Compensation
void TSS_ALL(int vectors[][2], double target[][64], double ref[][64],
             int param, int mb_high, int mb_wide);

void motion_compensation(double blocks[][64], double comp_image[][64],
                         int vectors[][2], int mb_high, int mb_wide);


// File I/O
int readHeader(FILE *fp, int* width, int* height, int* fr, int* inl, int* par);
int readFrame(FILE *fp, char* buffer, int sqtype);
int imwrite(FILE *fp, char* buffer, int sqtype);
#endif
