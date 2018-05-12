#ifndef defs_h_
#define defs_h_

/**
 * defs.h
 * Contains macros used by the other files.
 */
// CIF File format info
#define CIF_HEIGHT 288 
#define CIF_WIDTH 352 
#define LUMA_SIZE_CIF CIF_HEIGHT * CIF_WIDTH
#define CHROMA_SIZE_CIF LUMA_SIZE_CIF / 4
#define CIF_SIZE CIF_HEIGHT * CIF_WIDTH * 3 / 2 

// QCIF File format info
#define QCIF_HEIGHT 144 
#define QCIF_WIDTH 176 
#define LUMA_SIZE_QCIF QCIF_HEIGHT * QCIF_WIDTH
#define CHROMA_SIZE_QCIF LUMA_SIZE_QCIF / 4
#define QCIF_SIZE QCIF_HEIGHT * QCIF_WIDTH * 3 / 2 

#define BUFF_SIZE 100

// Enums, global constants, structs,...
enum Sqtype {QCIF, CIF};

enum interlace {IP, IT, IB, IM};

extern const double framerate[];

struct pblock_t
{
    short* Y1;
    short* Y2;
    short* Y3;
    short* Y4;
    short* U;
    short* V;
};


#endif
