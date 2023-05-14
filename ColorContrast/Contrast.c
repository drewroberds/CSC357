//#include <iostream>
#include <stdlib.h>
//#include <fstream>
#include <stdio.h>
#include <unistd.h>
#include <math.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <time.h>

//using namespace std;

typedef unsigned short WORD;
typedef unsigned int DWORD;
typedef unsigned int LONG;

typedef struct tagBITMAPFILEHEADER
{
WORD bfType; //specifies the file type (2)
DWORD bfSize; //specifies the size in bytes of the bitmap file (4)
WORD bfReserved1; //reserved; must be 0 (2)
WORD bfReserved2; //reserved; must be 0 (2)
DWORD bfOffBits; //species the offset in bytes from the bitmapfileheader to the bitmap bits (4)
}tagBITMAPFILEHEADER;
typedef struct tagBITMAPINFOHEADER
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
}tagBITMAPINFOHEADER;

//tagBITMAPFILEHEADER;
//tagBITMAPINFOHEADER;


int main(int argc, char *argv[])
//int main()
{
    //tagBITMAPINFOHEADER iheader; //create instance of tagBITMAPINFOHEADER struct

    if(argc != 6){
        printf("Program requires 6, and only 6 arguments, please try again\n");
        return 0;
    }

    clock_t programTime = clock();
    FILE *file = fopen(argv[1], "rb");
    //FILE *file = fopen("jar.bmp", "rb");
    if(file == NULL){
        printf("What file? Where? No file here!\n");
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

    //unsigned char *pixelptr = (unsigned char *) sbrk(iheader.biSizeImage);

    unsigned char *pixelptr = (unsigned char *) mmap(NULL, iheader.biSizeImage, PROT_WRITE | PROT_READ, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    //unsigned char *halfWay = (unsigned char *) mmap(NULL, 8, PROT_WRITE | PROT_READ, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    //unsigned char *end = (unsigned char *) mmap(NULL, 8, PROT_WRITE | PROT_READ, MAP_SHARED | MAP_ANONYMOUS, -1, 0);

    fread(pixelptr, iheader.biSizeImage, 1, file);
    fclose(file);

    FILE *fileout = fopen(argv[5], "wb");
    

    //int padding = (4 - (iheader.biWidth * 3) %4)%4;

    int halfWay = ((iheader.biWidth*3)) * (iheader.biHeight/2);
    //int end = pixelptr + iheader.biSizeImage;

    //FILE *fileout = fopen("output.bmp", "wb");

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
    
        // unsigned char *temp = pixelptr;
        // for(temp; temp < halfWay; temp = temp + 3){
        //     unsigned char blueIn = *temp;
        //     unsigned char greenIn = *(temp+1);
        //     unsigned char redIn = *(temp+2);

        //     float blueOut = (float)blueIn*1;
        //     float greenOut = (float)greenIn*0;
        //     float redOut = (float)redIn*0;

        //     *temp = blueOut;
        //     *(temp+1) = greenOut;
        //     *(temp+2) = redOut;
        // }
        // unsigned char *temp2 = halfWay;
        // for(temp2; temp2 < end; temp2 = temp2 + 3){
        //     unsigned char blueIn = *temp2;
        //     unsigned char greenIn = *(temp2+1);
        //     unsigned char redIn = *(temp2+2);

        //     float blueOut = (float)blueIn*1;
        //     float greenOut = (float)greenIn*0;
        //     float redOut = (float)redIn*0;

        //     *temp2 = blueOut;
        //     *(temp2+1) = greenOut;
        //     *(temp2+2) = redOut;
        // }

   if(fork() == 0){
        //unsigned char *temp2 = halfWay;
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
    //unsigned char *temp = pixelptr;
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
   }
   wait(0);
    fwrite(pixelptr, iheader.biSizeImage, 1, fileout);
    printf("The program ran for a total time of: %f seconds", (float) programTime/CLOCKS_PER_SEC);

    munmap(pixelptr, iheader.biSizeImage);
    //munmap(halfWay, 8);
    //munmap(end, 8);
    fclose(fileout);


    return 0;
    
}















// #include <iostream>
// #include <stdlib.h>
// #include <fstream>
// #include <stdio.h>
// #include <unistd.h>
// #include <math.h>
// #include <sys/mman.h>
// #include <sys/wait.h>

// using namespace std;

// typedef unsigned short WORD;
// typedef unsigned int DWORD;
// typedef unsigned int LONG;

// struct tagBITMAPFILEHEADER
// {
// WORD bfType; //specifies the file type (2)
// DWORD bfSize; //specifies the size in bytes of the bitmap file (4)
// WORD bfReserved1; //reserved; must be 0 (2)
// WORD bfReserved2; //reserved; must be 0 (2)
// DWORD bfOffBits; //species the offset in bytes from the bitmapfileheader to the bitmap bits (4)
// };
// struct tagBITMAPINFOHEADER
// {
// DWORD biSize; //specifies the number of bytes required by the struct
// LONG biWidth; //specifies width in pixels
// LONG biHeight; //species height in pixels
// WORD biPlanes; //specifies the number of color planes, must be 1
// WORD biBitCount; //specifies the number of bit per pixel
// DWORD biCompression;//spcifies the type of compression
// DWORD biSizeImage; //size of image in bytes
// LONG biXPelsPerMeter; //number of pixels per meter in x axis
// LONG biYPelsPerMeter; //number of pixels per meter in y axis
// DWORD biClrUsed; //number of colors used by th ebitmap
// DWORD biClrImportant; //number of colors that are important
// };

// typedef struct pixel {

//     unsigned char red;
//     unsigned char green;
//     unsigned char blue;

// }pixel;

// struct pixel colorGrade(struct pixel pix, float red, float green, float blue){
//     pix.red = (unsigned char) (pix.red * red);
//     pix.green = (unsigned char) (pix.green * green);
//     pix.blue = (unsigned char) (pix.blue * blue);
//     return pix;
// }

// // int main(int argc, char *argv[])
// int main()
// {
//     //tagBITMAPINFOHEADER iheader; //create instance of tagBITMAPINFOHEADER struct

//     // if(argc != 5){
//     //     printf("Program requires 6, and only 6 arguments, please try again\n");
//     //     return 0;
//     // }

//     //FILE *file = fopen(argv[1], "rb");
//     FILE *file = fopen("jar.bmp", "rb");
//     if(file == NULL){
//         cout<<"What file? Where? No file here!"<<endl;
//     }
//     tagBITMAPFILEHEADER fheader; //create instance of tagBITMAPFILEHEADER struct
//     fread(&fheader.bfType, sizeof(fheader.bfType), 1, file);
//     fread(&fheader.bfSize, sizeof(fheader.bfSize), 1, file);
//     fread(&fheader.bfReserved1, sizeof(fheader.bfReserved1), 1, file);
//     fread(&fheader.bfReserved2, sizeof(fheader.bfReserved2), 1, file);
//     fread(&fheader.bfOffBits, sizeof(fheader.bfOffBits), 1, file);

//     tagBITMAPINFOHEADER iheader;
//     fread(&iheader.biSize, sizeof(iheader.biSize), 1, file);
//     fread(&iheader.biWidth, sizeof(iheader.biWidth), 1, file);
//     fread(&iheader.biHeight, sizeof(iheader.biHeight), 1, file);
//     fread(&iheader.biPlanes, sizeof(iheader.biPlanes), 1, file);
//     fread(&iheader.biBitCount, sizeof(iheader.biBitCount), 1, file);
//     fread(&iheader.biCompression, sizeof(iheader.biCompression), 1, file);
//     fread(&iheader.biSizeImage, sizeof(iheader.biSizeImage), 1, file);
//     fread(&iheader.biXPelsPerMeter, sizeof(iheader.biXPelsPerMeter), 1, file);
//     fread(&iheader.biYPelsPerMeter, sizeof(iheader.biYPelsPerMeter), 1, file);
//     fread(&iheader.biClrUsed, sizeof(iheader.biClrUsed), 1, file);
//     fread(&iheader.biClrImportant, sizeof(iheader.biClrImportant), 1, file);

//     //unsigned char *pixelptr = (unsigned char *) sbrk(iheader.biSizeImage);

//     unsigned char *pixelptr = (unsigned char *) mmap(NULL, iheader.biSizeImage, PROT_WRITE | PROT_READ, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
//     unsigned char *halfWay = (unsigned char *) mmap(NULL, 8, PROT_WRITE | PROT_READ, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
//     unsigned char *end = (unsigned char *) mmap(NULL, 8, PROT_WRITE | PROT_READ, MAP_SHARED | MAP_ANONYMOUS, -1, 0);

//     fread(pixelptr, iheader.biSizeImage, 1, file);
//     fclose(file);

//     //FILE *fileout = fopen(argv[5], "wb");
    

//     //int padding = (4 - (iheader.biWidth * 3) %4)%4;
//     halfWay = pixelptr + ((iheader.biWidth*3) * iheader.biHeight/2);
//     end = pixelptr + iheader.biSizeImage;

//     FILE *fileout = fopen("output.bmp", "wb");

//     fwrite(&fheader.bfType, sizeof(fheader.bfType), 1, fileout);
//     fwrite(&fheader.bfSize, sizeof(fheader.bfSize), 1, fileout);
//     fwrite(&fheader.bfReserved1, sizeof(fheader.bfReserved1), 1, fileout);
//     fwrite(&fheader.bfReserved2, sizeof(fheader.bfReserved2), 1, fileout);
//     fwrite(&fheader.bfOffBits, sizeof(fheader.bfOffBits), 1, fileout);
//     fwrite(&iheader.biSize, sizeof(iheader.biSize), 1, fileout);
//     fwrite(&iheader.biWidth, sizeof(iheader.biWidth), 1, fileout);
//     fwrite(&iheader.biHeight, sizeof(iheader.biHeight), 1, fileout);
//     fwrite(&iheader.biPlanes, sizeof(iheader.biPlanes), 1, fileout);
//     fwrite(&iheader.biBitCount, sizeof(iheader.biBitCount), 1, fileout);
//     fwrite(&iheader.biCompression, sizeof(iheader.biCompression), 1, fileout);
//     fwrite(&iheader.biSizeImage, sizeof(iheader.biSizeImage), 1, fileout);
//     fwrite(&iheader.biXPelsPerMeter, sizeof(iheader.biXPelsPerMeter), 1, fileout);
//     fwrite(&iheader.biYPelsPerMeter, sizeof(iheader.biYPelsPerMeter), 1, fileout);
//     fwrite(&iheader.biClrUsed, sizeof(iheader.biClrUsed), 1, fileout);
//     fwrite(&iheader.biClrImportant, sizeof(iheader.biClrImportant), 1, fileout);
    
//         // unsigned char *temp = pixelptr;
//         // for(temp; temp < halfWay; temp = temp + 3){
//         //     unsigned char blueIn = *temp;
//         //     unsigned char greenIn = *(temp+1);
//         //     unsigned char redIn = *(temp+2);

//         //     float blueOut = (float)blueIn*atof(argv[4]);
//         //     float greenOut = (float)greenIn*atof(argv[3]);
//         //     float redOut = (float)redIn*atof(argv[2]);

//         //     *temp = blueOut;
//         //     *(temp+1) = greenOut;
//         //     *(temp+2) = redOut;
//         // }
//         // unsigned char *temp2 = halfWay;
//         // for(temp2; temp2 < end; temp2 = temp2 + 3){
//         //     unsigned char blueIn = *temp2;
//         //     unsigned char greenIn = *(temp2+1);
//         //     unsigned char redIn = *(temp2+2);

//         //     float blueOut = (float)blueIn*atof(argv[4]);
//         //     float greenOut = (float)greenIn*atof(argv[3]);
//         //     float redOut = (float)redIn*atof(argv[2]);

//         //     *temp2 = blueOut;
//         //     *(temp2+1) = greenOut;
//         //     *(temp2+2) = redOut;
//         // }
        
//    if(fork() == 0){
//         unsigned char *temp2 = halfWay;
//         for(temp2; temp2 < end; temp2 = temp2 + 3){
//             pixel p;
//             p.blue = *temp2;
//             p.green = *(temp2+1);
//             p.red = *(temp2+2);

//             p = colorGrade(p, 1, 1, 1);

//             *temp2 = p.blue;
//             *(temp2+1) = p.green;
//             *(temp2+2) = p.red;
//         }
//     exit(0);
//    }

//    else{
//     unsigned char *temp = pixelptr;
//         for(temp; temp < halfWay; temp = temp + 3){
//             unsigned char blueIn = *temp;
//             unsigned char greenIn = *(temp+1);
//             unsigned char redIn = *(temp+2);

//             float blueOut = (float)blueIn*1;
//             float greenOut = (float)greenIn*1;
//             float redOut = (float)redIn*0;

//             *temp = (unsigned char) blueOut;
//             *(temp+1) = (unsigned char) greenOut;
//             *(temp+2) = (unsigned char) redOut;
//         }
//         wait(0);
//    }

//     fwrite(pixelptr, iheader.biSizeImage, 1, fileout);

//     munmap(pixelptr, iheader.biSizeImage);
//     munmap(halfWay, 8);
//     munmap(end, 8);
//     fclose(fileout);


//     return 0;
    
// }