#include "encoder.h"

/**********************************************************/
/* inverse two dimensional DCT, Chen-Wang algorithm       */
/* (cf. IEEE ASSP-32, pp. 803-816, Aug. 1984)             */
/* 32-bit integer arithmetic (8 bit coefficients)         */
/* 11 mults, 29 adds per DCT                              */
/*                                      sE, 18.8.91       */
/**********************************************************/
/* coefficients extended to 12 bit for IEEE1180-1990      */
/* compliance                           sE,  2.1.94       */
/**********************************************************/

/* this code assumes >> to be a two's-complement arithmetic */
/* right shift: (-2)>>1 == -1 , (-3)>>1 == -2               */

#define W1 2841 /* 2048*sqrt(2)*cos(1*pi/16) */
#define W2 2676 /* 2048*sqrt(2)*cos(2*pi/16) */
#define W3 2408 /* 2048*sqrt(2)*cos(3*pi/16) */
#define W5 1609 /* 2048*sqrt(2)*cos(5*pi/16) */
#define W6 1108 /* 2048*sqrt(2)*cos(6*pi/16) */
#define W7 565  /* 2048*sqrt(2)*cos(7*pi/16) */

/* private data */
static short iclip[1024]; /* clipping table */
static short *iclp;

/* private prototypes */
static void idctrow (short *blk);
static void idctcol (short *blk);
//static int init_idct (void);

/* use a helping global variable to do initialization automatically */
//static const int g_bInit = init_idct();

/* row (horizontal) IDCT
 *
 *           7                       pi         1
 * dst[k] = sum c[l] * src[l] * cos( -- * ( k + - ) * l )
 *          l=0                      8          2
 *
 * where: c[0]    = 128
 *        c[1..7] = 128*sqrt(2)
 */

static void idctrow(short *blk)
{
	int x0, x1, x2, x3, x4, x5, x6, x7, x8;

	/* shortcut */
	if (!((x1 = blk[4]<<11) | (x2 = blk[6]) | (x3 = blk[2]) |
		    (x4 = blk[1]) | (x5 = blk[7]) | (x6 = blk[5]) | (x7 = blk[3])))
	{
		blk[0]=blk[1]=blk[2]=blk[3]=blk[4]=blk[5]=blk[6]=blk[7]=blk[0]<<3;
		return;
	}

	x0 = (blk[0]<<11) + 128; /* for proper rounding in the fourth stage */

	/* first stage */
	x8 = W7*(x4+x5);
	x4 = x8 + (W1-W7)*x4;
	x5 = x8 - (W1+W7)*x5;
	x8 = W3*(x6+x7);
	x6 = x8 - (W3-W5)*x6;
	x7 = x8 - (W3+W5)*x7;
	
	/* second stage */
	x8 = x0 + x1;
	x0 -= x1;
	x1 = W6*(x3+x2);
	x2 = x1 - (W2+W6)*x2;
	x3 = x1 + (W2-W6)*x3;
	x1 = x4 + x6;
	x4 -= x6;
	x6 = x5 + x7;
	x5 -= x7;
	
	/* third stage */
	x7 = x8 + x3;
	x8 -= x3;
	x3 = x0 + x2;
	x0 -= x2;
	x2 = (181*(x4+x5)+128)>>8;
	x4 = (181*(x4-x5)+128)>>8;
	
	/* fourth stage */
	blk[0] = (x7+x1)>>8;
	blk[1] = (x3+x2)>>8;
	blk[2] = (x0+x4)>>8;
	blk[3] = (x8+x6)>>8;
	blk[4] = (x8-x6)>>8;
	blk[5] = (x0-x4)>>8;
	blk[6] = (x3-x2)>>8;
	blk[7] = (x7-x1)>>8;
}

/* column (vertical) IDCT
 *
 *             7                         pi         1
 * dst[8*k] = sum c[l] * src[8*l] * cos( -- * ( k + - ) * l )
 *            l=0                        8          2
 *
 * where: c[0]    = 1/1024
 *        c[1..7] = (1/1024)*sqrt(2)
 */
static void idctcol(short *blk)
{
	int x0, x1, x2, x3, x4, x5, x6, x7, x8;

	/* shortcut */
	if (!((x1 = (blk[8*4]<<8)) | (x2 = blk[8*6]) | (x3 = blk[8*2]) |
		    (x4 = blk[8*1]) | (x5 = blk[8*7]) | (x6 = blk[8*5]) | (x7 = blk[8*3])))
	{
		blk[8*0]=blk[8*1]=blk[8*2]=blk[8*3]=blk[8*4]=blk[8*5]=blk[8*6]=blk[8*7]=
		  iclp[(blk[8*0]+32)>>6];
		return;
	}

	x0 = (blk[8*0]<<8) + 8192;

	/* first stage */
	x8 = W7*(x4+x5) + 4;
	x4 = (x8+(W1-W7)*x4)>>3;
	x5 = (x8-(W1+W7)*x5)>>3;
	x8 = W3*(x6+x7) + 4;
	x6 = (x8-(W3-W5)*x6)>>3;
	x7 = (x8-(W3+W5)*x7)>>3;
	
	/* second stage */
	x8 = x0 + x1;
	x0 -= x1;
	x1 = W6*(x3+x2) + 4;
	x2 = (x1-(W2+W6)*x2)>>3;
	x3 = (x1+(W2-W6)*x3)>>3;
	x1 = x4 + x6;
	x4 -= x6;
	x6 = x5 + x7;
	x5 -= x7;
	
	/* third stage */
	x7 = x8 + x3;
	x8 -= x3;
	x3 = x0 + x2;
	x0 -= x2;
	x2 = (181*(x4+x5)+128)>>8;
	x4 = (181*(x4-x5)+128)>>8;
	
	/* fourth stage */
	blk[8*0] = iclp[(x7+x1)>>14];
	blk[8*1] = iclp[(x3+x2)>>14];
	blk[8*2] = iclp[(x0+x4)>>14];
	blk[8*3] = iclp[(x8+x6)>>14];
	blk[8*4] = iclp[(x8-x6)>>14];
	blk[8*5] = iclp[(x0-x4)>>14];
	blk[8*6] = iclp[(x3-x2)>>14];
	blk[8*7] = iclp[(x7-x1)>>14];
}

/* two dimensional inverse discrete cosine transform */
void idct(short block[64])
{
	int i;

	for (i=0; i<8; i++)
		idctrow(block+8*i);

	for (i=0; i<8; i++)
		idctcol(block+i);
}

int init_idct()
{
	int i;

	iclp = iclip+512;
	for (i= -512; i<512; i++)
		iclp[i] = (i<-256) ? -256 : ((i>255) ? 255 : i);

	return 0;
}

void dct(short block[64])
{
	int        j1, i, j, k;
	float	b[8];
	float        b1[8];
	float        d[8][8];
	float f0=(float).7071068;
	float f1=(float).4903926;
	float f2=(float).4619398;
	float f3=(float).4157348;
	float f4=(float).3535534;
	float f5=(float).2777851;
	float f6=(float).1913417;
	float f7=(float).0975452;

	for (i = 0, k = 0; i < 8; i++, k += 8) {
		for (j = 0; j < 8; j++) {
		  b[j] = (float)block[k+j];
		}
		/* Horizontal transform */
		for (j = 0; j < 4; j++) {
		  j1 = 7 - j;
		  b1[j] = b[j] + b[j1];
		  b1[j1] = b[j] - b[j1];
		}
		b[0] = b1[0] + b1[3];
		b[1] = b1[1] + b1[2];
		b[2] = b1[1] - b1[2];
		b[3] = b1[0] - b1[3];
		b[4] = b1[4];
		b[5] = (b1[6] - b1[5]) * f0;
		b[6] = (b1[6] + b1[5]) * f0;
		b[7] = b1[7];
		d[i][0] = (b[0] + b[1]) * f4;
		d[i][4] = (b[0] - b[1]) * f4;
		d[i][2] = b[2] * f6 + b[3] * f2;
		d[i][6] = b[3] * f6 - b[2] * f2;
		b1[4] = b[4] + b[5];
		b1[7] = b[7] + b[6];
		b1[5] = b[4] - b[5];
		b1[6] = b[7] - b[6];
		d[i][1] = b1[4] * f7 + b1[7] * f1;
		d[i][5] = b1[5] * f3 + b1[6] * f5;
		d[i][7] = b1[7] * f7 - b1[4] * f1;
		d[i][3] = b1[6] * f3 - b1[5] * f5;
	}
	/* Vertical transform */
	for (i = 0; i < 8; i++) {
		for (j = 0; j < 4; j++) {
		  j1 = 7 - j;
		  b1[j] = d[j][i] + d[j1][i];
		  b1[j1] = d[j][i] - d[j1][i];
		}
		b[0] = b1[0] + b1[3];
		b[1] = b1[1] + b1[2];
		b[2] = b1[1] - b1[2];
		b[3] = b1[0] - b1[3];
		b[4] = b1[4];
		b[5] = (b1[6] - b1[5]) * f0;
		b[6] = (b1[6] + b1[5]) * f0;
		b[7] = b1[7];
		d[0][i] = (b[0] + b[1]) * f4;
		d[4][i] = (b[0] - b[1]) * f4;
		d[2][i] = b[2] * f6 + b[3] * f2;
		d[6][i] = b[3] * f6 - b[2] * f2;
		b1[4] = b[4] + b[5];
		b1[7] = b[7] + b[6];
		b1[5] = b[4] - b[5];
		b1[6] = b[7] - b[6];
		d[1][i] = b1[4] * f7 + b1[7] * f1;
		d[5][i] = b1[5] * f3 + b1[6] * f5;
		d[7][i] = b1[7] * f7 - b1[4] * f1;
		d[3][i] = b1[6] * f3 - b1[5] * f5;
	}
	for (i = 0; i < 8; i++)
		for (j = 0; j < 8; j++)
		  block[i << 3 | j] = (short)(d[i][j]);
}
