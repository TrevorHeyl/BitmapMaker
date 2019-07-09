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

    MakeFilledBmpImageRGB565( "RED.bmp",240, 240, imRED  );
    MakeFilledBmpImageRGB565( "GREEN.bmp",240, 480, imGREEN  );
    MakeFilledBmpImageRGB565( "BLUE.bmp",32, 16, imBLUE  );
    MakeFilledBmpImageRGB565( "PURPLE.bmp",80, 80, imPURPLE  );

    printf("Done!");
    return 0;
}
