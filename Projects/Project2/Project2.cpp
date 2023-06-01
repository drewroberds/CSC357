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
typedef unsigned char BYTE;

//numeric representation of colors for offest
int blue = 0;
int green = 1;
int red = 2;

//variables for smaller image
float red_s;
float green_s;
float blue_s;

//variables for larger image
float red_l;
float green_l;
float blue_l;

//variables for result image
// float resultRed;
// float resultGreen;
// float resultBlue;

// int sizeBytes;

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

tagBITMAPFILEHEADER largeFileheader;
tagBITMAPINFOHEADER largeInfoheader;
tagBITMAPINFOHEADER smallInfoheader;

BYTE get_color(BYTE *imageArray, int largeImageWidth, int largeImageHeight, int smallImageWidth, int smallImageHeight, int x_coordinate, int y_coordinate, int color)
{

    float xs = (float)x_coordinate*((float)(smallImageWidth - 1) / (float)(largeImageWidth - 1));
    float ys = (float)y_coordinate*((float)(smallImageHeight - 1) / (float)(largeImageHeight - 1));

    int x1 = floor(xs); //right side
    int x2 = ceil(xs); //left side
    int y1 = floor(ys); //bottom
    int y2 = ceil(ys); //top

    float dx = xs - (int)xs;
    float dy = ys - (int)ys;

    float right_upper = imageArray[x2*3 + y2*smallImageWidth + color];
    float left_upper = imageArray[x1*3 + y2*smallImageWidth + color];
    float right_lower = imageArray[x2*3 + y1*smallImageWidth + color];
    float left_lower = imageArray[x1*3 +y1*smallImageWidth + color];

    float upper = left_upper*(1-dx) + right_upper*dx;
    float lower = left_lower*(1-dx) + right_lower*dx;
    BYTE return_color = upper*dy + lower*(1-dy);

    return return_color;
}

int main(int argc, char *argv[])
{

    if(argc != 5){
        printf("Program only accepts 5 arguments, please try again\n");
        return 0;
    }

    //open first image (assign pointer), populate fheader and iheader with image data
    FILE *file = fopen(argv[1], "rb"); //taking only this one------------------------------------------>>>
    if(file == NULL){
        cout<<"Input file #1 not found, please check spelling and try again."<<endl;
        return 0;
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

    //open second image (assign pointer), populate fheader2 and iheader2 with image data
    FILE *file2 = fopen(argv[2], "rb");
        if(file2 == NULL)
    {
        cout<<"Input file #2 not found, please check spelling and try again."<<endl;
        return 0;
    }

    tagBITMAPFILEHEADER fheader2; //create instance of tagBITMAPFILEHEADER struct
    fread(&fheader2.bfType, sizeof(fheader2.bfType), 1, file2);
    fread(&fheader2.bfSize, sizeof(fheader2.bfSize), 1, file2);
    fread(&fheader2.bfReserved1, sizeof(fheader2.bfReserved1), 1, file2);
    fread(&fheader2.bfReserved2, sizeof(fheader2.bfReserved2), 1, file2);
    fread(&fheader2.bfOffBits, sizeof(fheader2.bfOffBits), 1, file2);

    tagBITMAPINFOHEADER iheader2;
    fread(&iheader2.biSize, sizeof(iheader2.biSize), 1, file2);
    fread(&iheader2.biWidth, sizeof(iheader2.biWidth), 1, file2);
    fread(&iheader2.biHeight, sizeof(iheader2.biHeight), 1, file2);
    fread(&iheader2.biPlanes, sizeof(iheader2.biPlanes), 1, file2);
    fread(&iheader2.biBitCount, sizeof(iheader2.biBitCount), 1, file2);
    fread(&iheader2.biCompression, sizeof(iheader2.biCompression), 1, file2);
    fread(&iheader2.biSizeImage, sizeof(iheader2.biSizeImage), 1, file2);
    fread(&iheader2.biXPelsPerMeter, sizeof(iheader2.biXPelsPerMeter), 1, file2);
    fread(&iheader2.biYPelsPerMeter, sizeof(iheader2.biYPelsPerMeter), 1, file2);
    fread(&iheader2.biClrUsed, sizeof(iheader2.biClrUsed), 1, file2);
    fread(&iheader2.biClrImportant, sizeof(iheader2.biClrImportant), 1, file2);

    //create vaiables assigned to large and small images

    BYTE *largePixelPtr = NULL;
    BYTE *smallPixelPtr = NULL;
    BYTE *resultPixelPtr = NULL;

    if(iheader.biWidth > iheader2.biWidth)
    {

        largeInfoheader = iheader;
        largeFileheader = fheader;
        largePixelPtr = (BYTE *) malloc(largeInfoheader.biSizeImage); 
        fread(largePixelPtr, largeInfoheader.biSizeImage, 1, file);

        smallInfoheader = iheader2;
        smallPixelPtr = (BYTE *) malloc(smallInfoheader.biSizeImage);
        fread(smallPixelPtr, smallInfoheader.biSizeImage, 1, file2);
        fclose(file2);

        resultPixelPtr = (BYTE *) malloc(largeInfoheader.biSizeImage); //allocate a BYTE array of the target size
        //fread(resultPixelPtr, largeInfoheader.biSizeImage, 1, file); //move the pointer to start of image data
        fclose(file);
    }
    else //for infoheader2 > infoheader case and for equal size pictures
    {
        largeInfoheader = iheader2;
        largeFileheader = fheader2;
        largePixelPtr = (BYTE *) malloc(largeInfoheader.biSizeImage);
        fread(largePixelPtr, largeInfoheader.biSizeImage, 1, file2);
        
        smallInfoheader = iheader;
        smallPixelPtr = (BYTE *) malloc(smallInfoheader.biSizeImage); //
        fread(smallPixelPtr, smallInfoheader.biSizeImage, 1, file);
        fclose(file);

        resultPixelPtr = (BYTE *) malloc(largeInfoheader.biSizeImage); //allocate a BYTE array of the target size
        //fread(resultPixelPtr, largeInfoheader.biSizeImage, 1, file2); //move the pointer to start of image data
        fclose(file2);
    }

    float scale = atof(argv[3]);

    if (scale < 0 || scale > 1)
    {
        printf("Incompadiable value detected, please enter a number between 0 and 1 and try again.\n");
        return 0;
    }

    int largePadding = (4-(largeInfoheader.biWidth*3)%4)%4; //cover padding for large
    int smallPadding= (4-(smallInfoheader.biWidth*3)%4)%4; //cover padding for small
    int widthBytesLarge = (largeInfoheader.biWidth)*3+largePadding;
    int widthBytesSmall = (smallInfoheader.biWidth)*3+smallPadding;
    int heightBytesSmall = smallInfoheader.biHeight;
    int heightBytesLarge = largeInfoheader.biHeight;

    for(int x = 0; x < largeInfoheader.biWidth; x++)
    {
        for(int y = 0; y < largeInfoheader.biHeight; y++)
        {
            BYTE blue_l = largePixelPtr[x*3 + y*widthBytesLarge + blue];
            BYTE green_l = largePixelPtr[x*3 + y*widthBytesLarge + green];
            BYTE red_l = largePixelPtr[x*3 + y*widthBytesLarge + red];

            blue_s = get_color(smallPixelPtr, widthBytesLarge, heightBytesLarge, widthBytesSmall, heightBytesSmall, x, y, blue);
            green_s = get_color(smallPixelPtr, widthBytesLarge, heightBytesLarge, widthBytesSmall, heightBytesSmall, x, y, green);
            red_s = get_color(smallPixelPtr, widthBytesLarge, heightBytesLarge, widthBytesSmall, heightBytesSmall, x, y, red);
            
            resultPixelPtr[x*3 + y*widthBytesLarge + blue] = blue_l*(1-scale) + blue_s*(scale);
            resultPixelPtr[x*3 + y*widthBytesLarge + green] = green_l*(1-scale) + green_s*(scale);
            resultPixelPtr[x*3 + y*widthBytesLarge + red] = red_l*(1-scale) + red_s*(scale);
        }
    }

    //Both ptrs should be positioned at the first pixel of each image in memory


    FILE *fileout = fopen(argv[4], "wb");

    fwrite(&largeFileheader.bfType, sizeof(largeFileheader.bfType), 1, fileout);
    fwrite(&largeFileheader.bfSize, sizeof(largeFileheader.bfSize), 1, fileout);
    fwrite(&largeFileheader.bfReserved1, sizeof(largeFileheader.bfReserved1), 1, fileout);
    fwrite(&largeFileheader.bfReserved2, sizeof(largeFileheader.bfReserved2), 1, fileout);
    fwrite(&largeFileheader.bfOffBits, sizeof(largeFileheader.bfOffBits), 1, fileout);
    fwrite(&largeInfoheader.biSize, sizeof(largeInfoheader.biSize), 1, fileout);
    fwrite(&largeInfoheader.biWidth, sizeof(largeInfoheader.biWidth), 1, fileout);
    fwrite(&largeInfoheader.biHeight, sizeof(largeInfoheader.biHeight), 1, fileout);
    fwrite(&largeInfoheader.biPlanes, sizeof(largeInfoheader.biPlanes), 1, fileout);
    fwrite(&largeInfoheader.biBitCount, sizeof(largeInfoheader.biBitCount), 1, fileout);
    fwrite(&largeInfoheader.biCompression, sizeof(largeInfoheader.biCompression), 1, fileout);
    fwrite(&largeInfoheader.biSizeImage, sizeof(largeInfoheader.biSizeImage), 1, fileout);
    fwrite(&largeInfoheader.biXPelsPerMeter, sizeof(largeInfoheader.biXPelsPerMeter), 1, fileout);
    fwrite(&largeInfoheader.biYPelsPerMeter, sizeof(largeInfoheader.biYPelsPerMeter), 1, fileout);
    fwrite(&largeInfoheader.biClrUsed, sizeof(largeInfoheader.biClrUsed), 1, fileout);
    fwrite(&largeInfoheader.biClrImportant, sizeof(largeInfoheader.biClrImportant), 1, fileout);

    fwrite(resultPixelPtr, largeInfoheader.biSizeImage, 1, fileout);

    fclose(fileout);
    free(resultPixelPtr); //for return picture data
    free(smallPixelPtr); //for smaller picture data
    free(largePixelPtr); //for larger picture data

    return 0;
}