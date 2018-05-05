#ifndef dct_h_
#define dct_h_

// Forward 8x8 DCT transform
void DCT2(double A[64]);

// Inverse 8x8 DCT transform
void IDCT2(double A[64]);

// Forward 8x8 Quantizer
void QUANT_intra(int A[64], const double B[64], const double step_size);
void QUANT_inter(int A[64], const double B[64], const double step_size);

// Inverse 8x8 Quantizer
void IQUANT(double A[64], const int B[64], const double step_size);

void zigzag_scan_8x8(int level[64], const int B[64]);
void runlength(char rl[400], const int level[64]);
#endif
