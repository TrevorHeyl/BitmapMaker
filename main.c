#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "bmp_gen.h"

/*

Utility to generate/draw 16bit RBG565 bitmap files according to the BMP/DIB file format (bitmap)
The BITMAPV5HEADER type is used as the DIB header spec'd here:
https://docs.microsoft.com/en-us/windows/win32/api/wingdi/ns-wingdi-bitmapv5header

The specific format here is as follows:
Bytes 0-13 : Bitmap header
Bytes 14..137   DIB header
Bytes 138...  Pixel array data


*/



int main()
{

    printf("Make some solid colour 16 bit RGB565 bitmaps\n");
    imMakeFilledBmpImageFileRGB565( "RED.bmp",120, 120, imRED  );
    imMakeFilledBmpImageFileRGB565( "GREEN.bmp",240, 240, imGREEN  );

    printf("Make some solid colour 16 bit bitmaps with lines drawn on\n");
    void * ImageData = imMakeFilledBmpImageRGB565( 100, 100, imBLUE  );
    // Coloured diamond drawn from head to tail
    imDrawLine(ImageData,50,10,70,30,imGREEN);
    imDrawLine(ImageData,70,30,50,50,imWHITE);
    imDrawLine(ImageData,50,50,30,30,imBLACK);
    imDrawLine(ImageData,30,30,50,10,imRED);
    imSaveImage(ImageData,"DIAMOND.bmp");
    imFree(ImageData);


    printf("Draw coloured squares\n");
    void * ImageDataS = imMakeFilledBmpImageRGB565( 100, 100, imGREEN  );
    imDrawSquare(ImageDataS,10,10,40,40,imBLACK,1);
    imDrawSquare(ImageDataS,30,30,30,30,imBLUE,4);
    imSaveImage(ImageDataS,"SQUARES.bmp");
    imFree(ImageDataS);


    printf("Draw coloured Rectangles\n");
    void * ImageDataB = imMakeFilledBmpImageRGB565( 100, 100, imBLUE  );
    imDrawRectangle(ImageDataB,10,10,40,40,imRED);
    imDrawRectangle(ImageDataB,30,50,60,20,imGREEN);
    imSaveImage(ImageDataB,"BOXES.bmp");
    imFree(ImageDataS);

    printf("Draw coloured Circles\n");
    void * ImageDataC = imMakeFilledBmpImageRGB565( 100, 100, imBLUE  );
    imDrawCircle(ImageDataC, 50, 50, 20, 3, imWHITE);
    imDrawCircle(ImageDataC, 20, 20, 15, 4, imRED);
    imDrawCircle(ImageDataC, 60, 40, 10, 1, imGREEN);
    imSaveImage(ImageDataC,"CIRCLES.bmp");
    imFree(ImageDataC);


    printf("Done!");
    return 0;
}
