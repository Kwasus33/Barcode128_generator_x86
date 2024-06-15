#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "image.h"


static const unsigned int imgHeight = 52;
static const unsigned int quietZone = 10;
static const unsigned int signLength = 11;
static const unsigned int stopSignLength = 13;
static const unsigned int startSignValue = 104;


ImageInfo* _code128_generation(ImageInfo* pImg, unsigned int x, char* TextPtr, unsigned int checkValue, unsigned int barSize);

unsigned int calculateCheckSign(char* text)
{
    unsigned int checkValue = startSignValue;
    
    for(unsigned int i = 0; text[i] != '\0'; i++)
    {
        checkValue += (i + 1) * (text[i] - 32);
    }

    return checkValue % 103;
}

void code128_prep(ImageInfo* pImg, char* text, unsigned int barSize)
{
    unsigned int checkSign = calculateCheckSign(text);

    _code128_generation(pImg, (quietZone/2) * barSize, text, checkSign, barSize);
    multiplyLine(pImg);
}


unsigned int barSize = 2;
char* texts[] = {"ARKO1234", "PROI_2024", "x86 is 'fun'"};


int main(int argc, char *argv[])
{
    if (sizeof(bmpHdr) != 54)
    {
        printf("Size of the bitmap header is invalid (%d). Please, check compiler options.\n", sizeof(bmpHdr));
        return 1;
    }

    for (unsigned int i = 0; texts[i] != NULL; i++)
    {
        unsigned int imgLength = (2 * quietZone + strlen(texts[i]) * signLength + 2 * signLength + stopSignLength) * barSize;
        ImageInfo *pImg = createImage(imgLength, 52, 4);

        if (pImg == NULL)
        {
            printf("Error opening input file *.bmp\n");
            return 1;
        }
        set_white_4bpp(pImg);

        code128_prep(pImg, texts[i], barSize);

        char filename[100];
        sprintf(filename, "%s.bmp", texts[i]);
        saveBmp(filename, pImg);
        freeImage(pImg);
    }

    return 0;
}
