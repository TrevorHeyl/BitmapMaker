#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <mem.h>
#include "bmp_gen.h"

/*

C parameteric Bitmap generation/drawing utilities

Utility to genearte bitmap files according to the BMP/DIB file format (bitmap)
The BITMAPV5HEADER type is used as the DIB header spec'd here:
https://docs.microsoft.com/en-us/windows/win32/api/wingdi/ns-wingdi-bitmapv5header

The specific format here is as follows:
Bytes 0-13 : Bitmap header
Bytes 14..137   DIB header
Bytes 138...  Pixel array data

See bmp_gen.h for the specific header format structure

Also supports lines, box and rectangle drawing

Use:
Make a 200,200 red filled 16 bit RGB565 bitmap called RED.bmp
imMakeFilledBmpImageRGB565( "RED.bmp",200,200,imRED);

Compatibility info:
Tested under Windows with GNU compiler, using CodeBlocks IDE

*/


/*
    imFree
    Release memory associated with an image
        void * image : pointer to memory/image data to free
*/
bool imFree(void * image) {

 if(image) {
    free(image);
    return true;
 } else {
     return false;
 }
}

/*
    imSaveImage
    Save image data to a file,just a binary write. The supplied data is already in bitmap format
    void * ImageData : Pointer to data to same
    const char 8 filename : filename to use for saving
*/
bool imSaveImage(void * ImageData,const char * filename ) {

    FILE *fw;
    bitmap_hdr *bm_hdr = (bitmap_hdr* )ImageData;
    uint32_t fsize = bm_hdr->size ;
    fw = fopen(filename,"wb");
    if (!fwrite(ImageData,1,fsize,fw)) {
        return false;
    }

    //free(ImageData);
    fclose(fw);
    return true;
}

/*
    imPlotPixel
    Plot one pixel of specific colour to specified image given starting and ending coordinates
    Handles transposing of coordinates to conceptually implement the coordinate plane where
    (x,y) = (0,0) is the top left of the image such that the x axis increases form left to right
    and the yaxis increases from top to bottom

*/
void imPlotPixel(void * ImageData, uint32_t x, uint32_t y,IBMP_COL colour) {

     dib_mbpv5hdr *dib_hdr = (dib_mbpv5hdr* )(ImageData+imDIB_OFFSET);
     uint32_t h = dib_hdr->bV5Height;
     uint32_t w = dib_hdr->bV5Width;
     uint16_t *PixelData = (uint16_t *)(ImageData+imPIXEL_DATA_OFFS);
     //uint32_t x,y;

     uint16_t *wrpos = PixelData + (h-y-1)*w + x ;
     *wrpos = (uint16_t)colour;

}


/*

  Columns (X axis) are from left to right if one visualizes the pixel data as a 2D array
  Rows are every "width" pixels, but row 0 as displayed is the last row in the pixel data file

*/




void drawCircle(void * ImageData, int xc, int yc, int x,int y, IBMP_COL colour)
{
    imPlotPixel(ImageData,xc+x, yc+y, colour);
    imPlotPixel(ImageData,xc-x, yc+y, colour);
    imPlotPixel(ImageData,xc+x, yc-y, colour);
    imPlotPixel(ImageData,xc-x, yc-y, colour);
    imPlotPixel(ImageData,xc+y, yc+x, colour);
    imPlotPixel(ImageData,xc-y, yc+x, colour);
    imPlotPixel(ImageData,xc+y, yc-x, colour);
    imPlotPixel(ImageData,xc-y, yc-x, colour);
}

void imDrawCircle_pro(void * ImageData,uint32_t xc, uint32_t yc, uint32_t r,  IBMP_COL colour)
{
    int x = 0, y = r;
    int d = 3 - 2 * r;
    drawCircle(ImageData,xc, yc, x, y,colour);
    while (y >= x)
    {
        // for each pixel we will
        // draw all eight pixels

        x++;

        // check for decision parameter
        // and correspondingly
        // update d, x, y
        if (d > 0)
        {
            y--;
            d = d + 4 * (x - y) + 10;
        }
        else
            d = d + 4 * x + 6;
        drawCircle(ImageData,xc, yc, x, y,colour);

    }
}

/*
imDrawCircle
    Draw a circle of specified colour and circle centre and radius
    using Bresenham's algorithm - c implemenation form here:
    https://www.geeksforgeeks.org/bresenhams-circle-drawing-algorithm/ article by  Shivam Pradhan

*/


void imDrawCircle(void * ImageData,uint32_t xc, uint32_t yc, uint32_t r, uint8_t thickness , IBMP_COL colour) {


    while(thickness) {
        imDrawCircle_pro(ImageData, xc, yc,  r-thickness,  colour);
        thickness--;
    }

}


/*
    imDrawRectangle
    Draws a rectangle of specified colour from a starting and ending coordinate pair.
    Speciy the width, height
*/
void imDrawRectangle(void * ImageData, uint32_t x,uint32_t y,uint32_t width, uint32_t height,IBMP_COL colour ) {

    uint8_t lp;
    for(lp=y;lp<y+height;lp++) {
        imDrawLine(ImageData,x ,lp ,    x+width,  lp,colour);
    }


}


/*
    imDrawSquare
    Draws a square of specified colour from a starting and ending coordinate pair.
    Speciy the width, height and line thickness
*/
void imDrawSquare(void * ImageData, uint32_t x,uint32_t y,uint32_t width, uint32_t height,IBMP_COL colour, uint8_t thickness) {

    uint8_t lp;
    for(lp=0;lp<thickness;lp++) {
        imDrawLine(ImageData,x+lp       ,y+lp       ,    x+width-lp,         y+lp,colour);
        imDrawLine(ImageData,x+width-lp ,y+lp       ,    x+width-lp,  y+height-lp,colour);
        imDrawLine(ImageData,x+width-lp ,y+height-lp,x+lp          ,y+height-lp  ,colour);
        imDrawLine(ImageData,x+lp       ,y+height-lp,x+lp          ,y+lp         ,colour);
    }
}

/*
    imDrawLine
    This C implementation of the Bresenham line algorithm taken from https://github.com/miloyip/line
    Draws a line of specified colour from a starting and ending coordinate pair.
    Handles transposing of coordinates to conceptually implement the coordinate plane where
    (x,y) = (0,0) is the top left of the image such that the x axis increases form left to right
    and the yaxis increases from top to bottom
*/
void imDrawLine(void * ImageData, uint32_t x0,uint32_t y0,uint32_t x1,uint32_t y1,IBMP_COL colour) {

    int dx = abs(x1 - x0), sx = x0 < x1 ? 1 : -1;
    int dy = abs(y1 - y0), sy = y0 < y1 ? 1 : -1;
    int err = (dx > dy ? dx : -dy) / 2;

    while (imPlotPixel(ImageData,x0, y0,colour), x0 != x1 || y0 != y1) {
        int e2 = err;
        if (e2 > -dx) { err -= dy; x0 += sx; }
        if (e2 <  dy) { err += dx; y0 += sy; }
}

}

#if 0 // first attempt
void imDrawLine(void * ImageData, uint32_t xstart,uint32_t ystart,uint32_t xend,uint32_t yend,IBMP_COL colour) {

     uint32_t x,y;
     // Prevent missing pixels in line draw
     // is x span longer than y span? IF so scale x/step y
     if( (xend-xstart) >= (yend-ystart) ) {
        // are we drawing backward in x axis (xstart > xend)
        if( xstart > xend) {
           for(x=xstart;x>=xend;x--) {
                y = ystart + ((yend-ystart)*(x-xstart))/(xend-xstart);
                imPlotPixel( ImageData, x, y , colour);
           }

        } else {
           for(x=xstart;x<=xend;x++) {
                y = ystart + ((yend-ystart)*(x-xstart))/(xend-xstart);
                imPlotPixel( ImageData, x, y , colour);
           }
        }
      // y span longer than x span   , so scale y span
     } else {

         if( ystart > yend) {
             for(y=ystart;y>=yend;y--) {
                  x = xstart + ((xend-xstart)*(y-ystart))/(yend-ystart);
                  imPlotPixel( ImageData, x, y , colour);
             }
         } else {
             for(y=ystart;y<=yend;y++) {
                  x = xstart + ((xend-xstart)*(y-ystart))/(yend-ystart);
                  imPlotPixel( ImageData, x, y , colour);
             }
         }


     }



}
#endif

/*
    FillImage
    Floodfill the image data with a colour, use as a background fill
        imagedata is the actualpixel data buffer excluding all headers
        size is the size of the imagedata buffer
        colour is the fill colour
*/
void  FillImage(
                void * imagedata,
                uint32_t width,
                uint32_t height,
                IBMP_COL colour)
{

    uint16_t *wrpos = imagedata+imPIXEL_DATA_OFFS;
    uint32_t h,w;


    for(h=0;h<height*2;h++) {
        for(w=0;w<width/2;w++) {
            *wrpos = (uint16_t)colour;
            wrpos++;
        }
    }
}

/*
MakeDIBHeader
Populate the DIB header - we choose V5 header format
    dib_hdr supply the dib structure
    w,h width and height of image
    colour_depth in bytes (2 bytes = 16 bits for 16 bit RGB565)
*/
void MakeDIBHeader(
                   dib_mbpv5hdr *dib_hdr,
                   uint32_t w,
                   uint32_t h,
                   uint32_t colour_depth)
{

    memset((void *)dib_hdr,0,sizeof(dib_mbpv5hdr));
    // populate dib info
    dib_hdr->bV5Size = (imDIBV5FMT_SIZE);
    dib_hdr->bV5Width = (w);
    dib_hdr->bV5Height = (h);
    dib_hdr->bV5Planes = (1);
    dib_hdr->bV5BitCount = (16);
    dib_hdr->bV5Compression = (3); // or 0?
    dib_hdr->bV5SizeImage = (w*h*colour_depth);
    dib_hdr->bV5XPelsPerMeter = imPIXELS_PM ;
    dib_hdr->bV5YPelsPerMeter = imPIXELS_PM ;
    dib_hdr->bV5Intent = 2;
    // Choose RGB565 16 bit colour space
    dib_hdr->bV5RedMask = imRGB565_REDMASK; // top 5 bits RED
    dib_hdr->bV5GreenMask = imRGB565_GREENMASK; // middel 6 bits green
    dib_hdr->bV5BlueMask = imRGB565_BLUEMASK;  // last 5 bits Blue
    dib_hdr->bV5CSType = imRGBs_TYPE; // RGBs

}


/*
MakeBMPHeader
Make the generic BMP header of 14 bytes
    bm_hdr supply the header structure
    w,h width and height of image
    colour_depth in bytes (2 bytes = 16 bits for 16 bit RGB565)
*/
void  MakeBMPHeader(
                    bitmap_hdr *bm_hdr,
                    uint32_t w,
                    uint32_t h,
                    uint32_t colour_depth )
{

    memset((void *)bm_hdr,0,sizeof(bitmap_hdr));
    // populate header info
    bm_hdr->hdr_field[0] = 'B';
    bm_hdr->hdr_field[1] = 'M';
    bm_hdr->size = (sizeof(dib_mbpv5hdr) + sizeof(bitmap_hdr) + w*h*colour_depth);
    bm_hdr->offs = imPIXEL_DATA_OFFS; // where pixel data starts

}


/*
imMakeFilledBmpImageRGB565
Make a BMP file of specified size, filled with specified colour
    bm_hdr supply the header structure
    w,h width and height of image
    colour_depth in bytes (2 bytes = 16 bits for 16 bit RGB565)
*/
void imMakeFilledBmpImageFileRGB565( const char * filename,
                         uint32_t w,
                         uint32_t h,
                         IBMP_COL col  )
{

    uint32_t width,height,colour_depth;
    //uint32_t imData_size;

    bitmap_hdr bm_hdr;
    dib_mbpv5hdr dib_hdr;
#ifdef ADD_ROW_PADDING
    width= (w/4)*4+4; // padding required for rows
#else
    width= w; // padding required for rows
#endif
    height=h;
    colour_depth=2; // bytes

    //imData_size = width*height*colour_depth;

    MakeBMPHeader(&bm_hdr, width, height,colour_depth );
    MakeDIBHeader(&dib_hdr, width, height,colour_depth );

    void *imagedata = calloc(1,width*height*colour_depth+imPIXEL_DATA_OFFS);
    memcpy(imagedata,(void *)&bm_hdr,sizeof(bm_hdr));
    memcpy((imagedata+14),(void *)&dib_hdr,sizeof(dib_hdr));
    FillImage(imagedata,width,height,(uint16_t)col);

    FILE *fw;
    uint32_t fsize = bm_hdr.size ;
    fw = fopen(filename,"wb");
    if (!fwrite(imagedata,1,fsize,fw)) {
        printf("Could not write file");
    }

    free(imagedata);
    fclose(fw);
}


/*******************************************************
imMakeFilledBmpImageRGB565
Make a BMP file od specified size, filled with specified colour
    bm_hdr supply the header structure
    w,h width and height of image
    colour_depth in bytes (2 bytes = 16 bits for 16 bit RGB565)
*******************************************************/
void * imMakeFilledBmpImageRGB565(
                         uint32_t w,
                         uint32_t h,
                         IBMP_COL col  )
{

    uint32_t width,height,colour_depth;
    //uint32_t imData_size;

    bitmap_hdr bm_hdr;
    dib_mbpv5hdr dib_hdr;
#ifdef ADD_ROW_PADDING
    width= (w/4)*4+4; // padding required for rows
#else
    width= w; // padding required for rows
#endif
    height=h;
    colour_depth=2; // bytes

    //imData_size = width*height*colour_depth;

    MakeBMPHeader(&bm_hdr, width, height,colour_depth );
    MakeDIBHeader(&dib_hdr, width, height,colour_depth );

    void *imagedata = calloc(1,width*height*colour_depth+imPIXEL_DATA_OFFS);
    memcpy(imagedata,(void *)&bm_hdr,sizeof(bm_hdr));
    memcpy((imagedata+14),(void *)&dib_hdr,sizeof(dib_hdr));
    FillImage(imagedata,width,height,(uint16_t)col);

    return imagedata;


}




