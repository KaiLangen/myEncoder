#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "encoder.h"
#include "defs.h"

const char* frname[] = 
{
    "F30:1",        // 30 FPS
    "F25:1",        // 25 FPS
    "F24:1",        // 24 FPS
    "F30000:1001",  // 29.97 FPS
    "F24000:1001"   // 23.976 FPS
};

const double framerate[] ={30., 25., 24., 29.97, 23.976};

const char* inlname[] = { "Ip", "It","Ib", "Im" };

int readHeader(FILE *fp, int* width, int* height, int* fr, int* inl, int* ar)
{
    int res;
    const char* signature = "YUV4MPEG2";
    char sig[10], W[10], H[10], F[10], I[10], A[10];
    char str[BUFF_SIZE];

    fseek(fp, 0, SEEK_SET);
    fgets(str, BUFF_SIZE, fp);
    printf("%s\n", str);
    if (res = sscanf(str, "%s %s %s %s %s %s", sig, W, H, F, I, A) == 6)
    {
        if (strcmp(sig, signature) != 0)
        {
            fprintf(stderr, "Invalid video signature: %s\n", sig);
            return -1;
        }
        *width = atoi(&W[1]);
        *height = atoi(&H[1]);

        // compare against frame-rate formats
        *fr = -1;
        for (int i = 0; i < 5; ++i)
        {
            if (strcmp(frname[i], F) == 0)
            {
                *fr = i;
                break;
            }
        }
        if (*fr == -1)
        {
            fprintf(stderr, "Frame-rate: %s not found.\n", F);
            return -1;
        }

        // compare against interlace formats
        *inl = -1;
        for (int i = 0; i < 5; ++i)
        {
            if (strcmp(inlname[i], I) == 0)
            {
                *inl = i;
                break;
            }
        }
        if (*inl == -1)
        {
            fprintf(stderr, "Interlace type: %s not found.\n", I);
            return -1;
        }
        // compare against pixel aspect-ratio formats
    }
    else
    {
        fprintf(stderr, "Invalid number of header items %d\n", res);
        return -1;
    }
    return 0;
}

int readFrame(FILE *fp, char* buffer, int sqtype)
{
    const char frame[] = "FRAME\n";
    fgets(buffer, BUFF_SIZE, fp);
    if (strcmp(buffer, frame) != 0)
    {
        fprintf(stderr, "Each frame must start with %s not %s.\n", frame, buffer);
        return -1;
    }

    if(sqtype == QCIF)
    {
        //fseek(fp, frameNum * QCIF_SIZE, SEEK_SET);
        //fseek(fp, frameNum * LUMA_SIZE_QCIF + 2*CHROMA_SIZE_QCIF, SEEK_SET);
        fseek(fp, 0, SEEK_CUR);
        fread(buffer, QCIF_SIZE, 1, fp);
    }
    else if(sqtype == CIF)
    {
        fseek(fp, 0, SEEK_CUR);
        fread(buffer, CIF_SIZE, 1, fp);
    }
    else
    {
        // throw an error of some sort??
        fprintf(stderr, "Invalid sqtype: %d", sqtype);
        return -1;
    }
    return 0;
}

int imwrite(FILE *fp, char* buffer, int sqtype)
{
    if(sqtype == QCIF)
    {
        //fseek(fp, frameNum * (LUMA_SIZE_CIF + 2*CHROMA_SIZE_CIF), SEEK_SET);
        //fseek(fp, frameNum * QCIF_SIZE, SEEK_SET);
        fseek(fp, 0, SEEK_CUR);
        fread(buffer, QCIF_SIZE, 1, fp);
    }
    else if(sqtype == CIF)
    {
        fread(buffer, CIF_SIZE, 1, fp);
    }
    else
    {
        // throw an error of some sort??
        fprintf(stderr, "Invalid sqtype: %d", sqtype);
        return -1;
    }
    return 0;
}
