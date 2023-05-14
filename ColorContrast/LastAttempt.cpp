#include <iostream>
#include <stdlib.h>
#include <fstream>
#include <stdio.h>
#include <unistd.h>
#include <math.h>
#include <sys/mman.h>
#include <sys/wait.h>

using namespace std;

typedef unsigned short WORD;
typedef unsigned int DWORD;
typedef unsigned int LONG;

typedef struct BMPFileHeader{
    WORD bfType; //specifies the file type (2)
    DWORD bfSize; //specifies the size in bytes of the bitmap file (4)
    DWORD bfReserved; //reserved; must be 0 (4)
    DWORD bfOffBits; //species the offset in bytes from the bitmapfileheader to the bitmap bits (4)
}BMPFileHeader;

typedef struct BMPImageHeader{
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
}BMPImageHeader;

int main(int argc, char *argv[]){
//int main(){

if(argc != 6){
    printf("Program requires 6, and only 6 arguments, please try again\n");
    return 0;
}

BMPFileHeader fheader;
BMPImageHeader iheader;

FILE *file = fopen(argv[1], "rb");

if(file == NULL){
    cout<<"What file? Where? No file here!"<<endl;
}

fread(&fheader.bfType, sizeof(fheader.bfType), 1, file);
fread(&fheader.bfSize, sizeof(fheader.bfSize), 1, file);
fread(&fheader.bfReserved, sizeof(fheader.bfReserved), 1, file);
fread(&fheader.bfOffBits, sizeof(fheader.bfOffBits), 1, file);

fread(&iheader.biSize, sizeof(iheader), 1, file);

unsigned char *pixelptr = (unsigned char*) mmap(NULL, iheader.biSizeImage, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);

fread(pixelptr, iheader.biSizeImage, 1, file);
fclose(file);

int halfWay = ((iheader.biWidth*3)) * (iheader.biHeight/2);

//for(int timesRun = 0; timesRun < 1000; timesRun++){
    clock_t programTime = clock();
    float startTime = ((float) programTime)/CLOCKS_PER_SEC;
    if(fork() == 0){
        for(int i = halfWay; i < iheader.biSizeImage; i = i + 3){
            unsigned char blueIn = pixelptr[i];
            unsigned char greenIn = pixelptr[i+1];
            unsigned char redIn = pixelptr[i+2];

            float blueOut = (float)blueIn*atof(argv[4]);
            float greenOut = (float)greenIn*atof(argv[3]);
            float redOut = (float)redIn*atof(argv[2]);

            pixelptr[i] = (unsigned char) blueOut;
            pixelptr[i + 1] = (unsigned char) greenOut;
            pixelptr[i + 2] = (unsigned char) redOut;
        }
        return 0;
    }
    else{
        for(int i = 0; i < halfWay; i = i + 3){
            unsigned char blueIn = pixelptr[i];
            unsigned char greenIn = pixelptr[i+1];
            unsigned char redIn = pixelptr[i+2];

            float blueOut = (float)blueIn*atof(argv[4]);
            float greenOut = (float)greenIn*atof(argv[3]);
            float redOut = (float)redIn*atof(argv[2]);

            pixelptr[i] = (unsigned char) blueOut;
            pixelptr[i + 1] = (unsigned char) greenOut;
            pixelptr[i + 2] = (unsigned char) redOut;
        }
        wait(0);
    };

    clock_t programEnd = clock();
    float endTime = ((float) programEnd)/CLOCKS_PER_SEC;

    FILE *outfile = fopen(argv[5], "wb");
    fwrite(&fheader.bfType, sizeof(fheader.bfType), 1, outfile);
    fwrite(&fheader.bfSize, sizeof(fheader.bfSize), 1, outfile);
    fwrite(&fheader.bfReserved, sizeof(fheader.bfReserved), 1, outfile);
    fwrite(&fheader.bfOffBits, sizeof(fheader.bfOffBits), 1, outfile);
    fwrite(&iheader.biSize, sizeof(iheader), 1, outfile);
    fwrite(pixelptr, iheader.biSizeImage, 1, outfile);
    fclose(outfile);

    clock_t noForkTime = clock();
    float noForkStart = ((float) noForkTime)/CLOCKS_PER_SEC;

    for(int i = 0; i < iheader.biSizeImage; i = i + 3){
            unsigned char blueIn = pixelptr[i];
            unsigned char greenIn = pixelptr[i+1];
            unsigned char redIn = pixelptr[i+2];

            float blueOut = (float)blueIn*atof(argv[4]);
            float greenOut = (float)greenIn*atof(argv[3]);
            float redOut = (float)redIn*atof(argv[2]);

            pixelptr[i] = (unsigned char) blueOut;
            pixelptr[i + 1] = (unsigned char) greenOut;
            pixelptr[i + 2] = (unsigned char) redOut;
        }

    clock_t noForkTimeEnd = clock();
    float noForkEnd = ((float) noForkTimeEnd)/CLOCKS_PER_SEC;
//}


munmap(pixelptr, iheader.biSizeImage);

cout << noForkStart << endl;

cout << "The forking program ran for a total time of: " <<  (endTime - startTime)*1000 << " milliseconds." << endl;
cout << "The non-forking program ran for: " << (noForkEnd - noForkStart)*1000 << " milliseconds." << endl;
cout << "Fork time is " << (int)((((noForkEnd - noForkStart)*1000)/((endTime - startTime)*1000)*100)-100) << "% faster than non-forking time." << endl;
return 0;
};