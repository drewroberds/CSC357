#include <iostream>
#include <stdlib.h>
#include <fstream>
#include <stdio.h>
#include <unistd.h>
#include <math.h>

using namespace std;

typedef unsigned short WORD;
typedef unsigned int DWORD;
typedef unsigned int LONG;

struct tagBITMAPFILEHEADER
{
WORD bfType; //specifies the file type (2)
DWORD bfSize; //specifies the size in bytes of the bitmap file (4)
WORD bfReserved1; //reserved; must be 0 (2)
WORD bfReserved2; //reserved; must be 0 (2)
DWORD bfOffBits; //species the offset in bytes from the bitmapfileheader to the bitmap bits (4)
};
struct tagBITMAPINFOHEADER
{
DWORD biSize; //specifies the number of bytes required by the struct
LONG biWidth; //specifies width in pixels
LONG biHeight; //species height in pixels
WORD biPlanes; //specifies the number of color planes, must be 1
WORD biBitCount; //specifies the number of bit per pixel
DWORD biCompression;//spcifies the type of compression
DWORD biSizeImage; //size of image in bytes
LONG biXPelsPerMeter; //number of pixels per meter in x axis
LONG biYPelsPerMeter; //number of pixels per meter in y axis
DWORD biClrUsed; //number of colors used by th ebitmap
DWORD biClrImportant; //number of colors that are important
};

unsigned char ChangeContrast(unsigned char input, float scale){
    float normalizedInput = ((float)input)/255;
    float output = pow(normalizedInput, scale);
    output = output * 255;
    return output;
}

int main(int argc, char *argv[])
{
    //tagBITMAPINFOHEADER iheader; //create instance of tagBITMAPINFOHEADER struct

    if(argc != 4){
        printf("Program only accepts 4 arguments, please try again\n");
        return 0;
    }

    FILE *file = fopen(argv[1], "rb");
    if(file == NULL){
        cout<<"What file? Where? No file here!"<<endl;
    }
    tagBITMAPFILEHEADER fheader; //create instance of tagBITMAPFILEHEADER struct
    fread(&fheader.bfType, sizeof(fheader.bfType), 1, file);
    fread(&fheader.bfSize, sizeof(fheader.bfSize), 1, file);
    fread(&fheader.bfReserved1, sizeof(fheader.bfReserved1), 1, file);
    fread(&fheader.bfReserved2, sizeof(fheader.bfReserved2), 1, file);
    fread(&fheader.bfOffBits, sizeof(fheader.bfOffBits), 1, file);

    tagBITMAPINFOHEADER iheader;
    fread(&iheader.biSize, sizeof(iheader.biSize), 1, file);
    fread(&iheader.biWidth, sizeof(iheader.biWidth), 1, file);
    fread(&iheader.biHeight, sizeof(iheader.biHeight), 1, file);
    fread(&iheader.biPlanes, sizeof(iheader.biPlanes), 1, file);
    fread(&iheader.biBitCount, sizeof(iheader.biBitCount), 1, file);
    fread(&iheader.biCompression, sizeof(iheader.biCompression), 1, file);
    fread(&iheader.biSizeImage, sizeof(iheader.biSizeImage), 1, file);
    fread(&iheader.biXPelsPerMeter, sizeof(iheader.biXPelsPerMeter), 1, file);
    fread(&iheader.biYPelsPerMeter, sizeof(iheader.biYPelsPerMeter), 1, file);
    fread(&iheader.biClrUsed, sizeof(iheader.biClrUsed), 1, file);
    fread(&iheader.biClrImportant, sizeof(iheader.biClrImportant), 1, file);

    unsigned char *pixelptr = (unsigned char *) sbrk(iheader.biSizeImage);
    fread(pixelptr, iheader.biSizeImage, 1, file);
    fclose(file);

    FILE *fileout = fopen(argv[2], "wb");

    fwrite(&fheader.bfType, sizeof(fheader.bfType), 1, fileout);
    fwrite(&fheader.bfSize, sizeof(fheader.bfSize), 1, fileout);
    fwrite(&fheader.bfReserved1, sizeof(fheader.bfReserved1), 1, fileout);
    fwrite(&fheader.bfReserved2, sizeof(fheader.bfReserved2), 1, fileout);
    fwrite(&fheader.bfOffBits, sizeof(fheader.bfOffBits), 1, fileout);
    fwrite(&iheader.biSize, sizeof(iheader.biSize), 1, fileout);
    fwrite(&iheader.biWidth, sizeof(iheader.biWidth), 1, fileout);
    fwrite(&iheader.biHeight, sizeof(iheader.biHeight), 1, fileout);
    fwrite(&iheader.biPlanes, sizeof(iheader.biPlanes), 1, fileout);
    fwrite(&iheader.biBitCount, sizeof(iheader.biBitCount), 1, fileout);
    fwrite(&iheader.biCompression, sizeof(iheader.biCompression), 1, fileout);
    fwrite(&iheader.biSizeImage, sizeof(iheader.biSizeImage), 1, fileout);
    fwrite(&iheader.biXPelsPerMeter, sizeof(iheader.biXPelsPerMeter), 1, fileout);
    fwrite(&iheader.biYPelsPerMeter, sizeof(iheader.biYPelsPerMeter), 1, fileout);
    fwrite(&iheader.biClrUsed, sizeof(iheader.biClrUsed), 1, fileout);
    fwrite(&iheader.biClrImportant, sizeof(iheader.biClrImportant), 1, fileout);

    unsigned char *temp = pixelptr;
    for(temp; temp < pixelptr + iheader.biSizeImage; temp++){
        *temp = ChangeContrast(*temp, atof(argv[3]));
    }

    fwrite(pixelptr, iheader.biSizeImage, 1, fileout);

    fclose(fileout);
    sbrk(-iheader.biSizeImage);


    return 0;
}