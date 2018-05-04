#ifndef dct_h_
#define dct_h_

// DCT type II, scaled. Algorithm by Arai, Agui, Nakajima, 1988.
// See: https://web.stanford.edu/class/ee398a/handouts/lectures/07-TransformCoding.pdf#page=30
void FastDct8_transform(double vector[8]);

// DCT type III, scaled. A straightforward inverse of the forward algorithm.
void FastDct8_inverseTransform(double vector[8]);

// Forward 8x8 DCT transform
void DCT2(double A[8][8]);

// Inverse 8x8 DCT transform
void IDCT2(double A[8][8]);

// Forward 8x8 Quantizer
void QUANT_intra(int A[8][8], const double B[8][8], const double step_size);
void QUANT_inter(int A[8][8], const double B[8][8], const double step_size);

// Inverse 8x8 Quantizer
void IQUANT(double A[8][8], const int B[8][8], const double step_size);

void zigzag_scan_8x8(int level[64], const int B[8][8]);
void runlength(char rl[400], const int level[64]);
#endif
