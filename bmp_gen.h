#ifndef _BMP_GEN_H_
#define _BMP_GEN_H_
#include<stdint.h>
#include<stdbool.h>

// Row padding is usually so that structure data is 4byte aligned, but for embedded system it might be useful to omit this
//#define ADD_ROW_PADDING // make sure the row data is mutiples of 4 bytes, i.e uin32_t boundary

#define imDIB_OFFSET 14
#define imPIXEL_DATA_OFFS 0x8A
#define imDIBV5FMT_SIZE 0x7C
#define imRGBs_TYPE 0x73524742
#define imRGB565_REDMASK  0xf800
#define imRGB565_BLUEMASK 0x001f
#define imRGB565_GREENMASK 0x07e0
#define imPIXELS_PM 2835


/*Solid fill colours in RGB565 format

The colours ar emapped as follows:
b15...b10
*/
typedef enum IBMP_COL {
    imRED = 0xf800,
    imBLUE = 0x0175,//0x001f,
    imGREEN =  0x07e0,
    imPURPLE = 0xf81f,
    imBLACK = 0x0000,
    imWHITE = 0xFFFF,
    imGRAY = 0x52aa, //  01010 010101 01010
    imLGRAY = 0x73ae, // 01110 011101 01110
}IBMP_COL;


#pragma pack(1)
typedef struct bitmap_hdr
{
    uint8_t hdr_field[2]; // Offs 0
    uint32_t size;     // offs 2
    uint16_t res1;     // offs 6
    uint16_t res2;     // offs 8
    uint32_t offs;     // offs 10

} bitmap_hdr;

typedef struct dib_mbpv5hdr{
  uint32_t        bV5Size;
  uint32_t        bV5Width;
  uint32_t        bV5Height;
  uint16_t        bV5Planes;
  uint16_t        bV5BitCount;
  uint32_t        bV5Compression;
  uint32_t        bV5SizeImage;
  uint32_t        bV5XPelsPerMeter;
  uint32_t        bV5YPelsPerMeter;
  uint32_t        bV5ClrUsed;
  uint32_t        bV5ClrImportant;
  uint32_t        bV5RedMask;
  uint32_t        bV5GreenMask;
  uint32_t        bV5BlueMask;
  uint32_t        bV5AlphaMask;
  uint32_t        bV5CSType;
  uint8_t         bV5Endpoints[36];
  uint32_t        bV5GammaRed;
  uint32_t        bV5GammaGreen;
  uint32_t        bV5GammaBlue;
  uint32_t        bV5Intent;
  uint32_t        bV5ProfileData;
  uint32_t        bV5ProfileSize;
  uint32_t        bV5Reserved;
} dib_mbpv5hdr;



/*
 Make a RGB565 16 bit bitmap image of specific size filled with a background colour and save to a file
*/
void imMakeFilledBmpImageFileRGB565( const char * filename,uint32_t w, uint32_t h, IBMP_COL col  );
/*
 Make a RGB565 16 bit bitmap image of specific size filled with a background colour
*/
void *imMakeFilledBmpImageRGB565( uint32_t w, uint32_t h,  IBMP_COL col  );
/*
 Draw a line of specific colour to specified image given starting and ending coordinates
*/
void imDrawLine(void * ImageData, uint32_t xstart,uint32_t ystart,uint32_t xend,uint32_t yend,IBMP_COL colour) ;
/*
 Plot one pixel of specific colour to specified image given starting and ending coordinates
*/
void imPlotPixel(void * ImageData, uint32_t x, uint32_t y,IBMP_COL colour) ;
/*
 Save image data to a file
*/
bool imSaveImage(void * ImageData,const char * filename );
/*
    Release memory associated with an image
*/
bool imFree(void * image);

bool imSaveImage(void * ImageData,const char * filename );
/*
    Draw a square of specified colour, given starting coordinates, width and heigth
*/
void imDrawSquare(void * ImageData, uint32_t x,uint32_t y,uint32_t width, uint32_t height,IBMP_COL colour,uint8_t thickness);
/*
    Draws a rectangle of specified colour from a starting and ending coordinate pair.
    Speciy the width, height
*/
void imDrawRectangle(void * ImageData, uint32_t x,uint32_t y,uint32_t width, uint32_t height,IBMP_COL colour ) ;
/*
    Draws a circle of specified colour from circle centre given radius.
    Speciy the centre, radius and colour
*/
void imDrawCircle(void * ImageData,uint32_t xc, uint32_t yc, uint32_t r, uint8_t thickness , IBMP_COL colour);

#endif // _BMP_GEN_H_
