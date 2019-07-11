#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "bmp_gen.h"

/*

Utility to genearte bitmap files according to the BMP/DIB file format (bitmap)
The BITMAPV5HEADER type is used as the DIB header spec'd here:
https://docs.microsoft.com/en-us/windows/win32/api/wingdi/ns-wingdi-bitmapv5header

The specific format here is as follows:
Bytes 0-13 : Bitmap header
Bytes 14..137   DIB header
Bytes 138...  Pixel array data


*/



int main()
{

    printf("Demo to make some solid colour 16 bit bitmaps\n");

    //MakeFilledBmpImageFileRGB565( "RED.bmp",120, 120, imRED  );
    //MakeFilledBmpImageFileRGB565( "GREEN.bmp",240, 240, imGREEN  );
    //MakeFilledBmpImageFileRGB565( "BLUE.bmp",240, 240, imBLUE  );
    //MakeFilledBmpImageFileRGB565( "PURPLE.bmp",80, 80, imPURPLE  );
    //MakeFilledBmpImageFileRGB565( "WHITE.bmp",80, 80, imWHITE  );
    //MakeFilledBmpImageFileRGB565( "BLACK.bmp",80, 80, imBLACK  );
    //MakeFilledBmpImageFileRGB565( "GRAY.bmp",80, 80, imGRAY  );
    //MakeFilledBmpImageFileRGB565( "LIGHTGRAY.bmp",80, 80, imLGRAY  );


    void * ImageData = MakeFilledBmpImageRGB565( 100, 100, imBLUE  );

    //DrawLine(ImageData,10,10,30,30,imGREEN);
    //DrawLine(ImageData,10,10,30,40,imWHITE);
    //DrawLine(ImageData,10,10,40,30,imBLACK);
    // lines to make a box
    //DrawLine(ImageData,10,10,30,10,imGREEN);
    //DrawLine(ImageData,30,10,30,30,imWHITE);
    //DrawLine(ImageData,30,30,10,30,imBLACK);
    //DrawLine(ImageData,10,30,10,10,imRED);
    // diamond
    DrawLine(ImageData,50,10,70,30,imGREEN);
    DrawLine(ImageData,70,30,50,50,imWHITE);
    DrawLine(ImageData,50,50,20,30,imBLACK);
    DrawLine(ImageData,20,30,50,10,imRED);


    //PlotPixel( ImageData,10,10 ,imWHITE) ;
    //PlotPixel( ImageData,30,30 ,imWHITE) ;
    //DrawLine(ImageData,30,50,50,100,imRED);
    //DrawLine(ImageData,0,75,10,100,imBLACK);
    FILE *fw;
    bitmap_hdr *bm_hdr = (bitmap_hdr* )ImageData;
    uint32_t fsize = bm_hdr->size ;
    fw = fopen("f100.bmp","wb");
    if (!fwrite(ImageData,1,fsize,fw)) {
        printf("Could not write file");
    }

    free(ImageData);
    fclose(fw);


    printf("Done!");
    return 0;
}
