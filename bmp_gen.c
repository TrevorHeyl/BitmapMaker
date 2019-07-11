#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <mem.h>
#include "bmp_gen.h"

/*

Bitmap generation utilities

Utility to genearte bitmap files according to the BMP/DIB file format (bitmap)
The BITMAPV5HEADER type is used as the DIB header spec'd here:
https://docs.microsoft.com/en-us/windows/win32/api/wingdi/ns-wingdi-bitmapv5header

The specific format here is as follows:
Bytes 0-13 : Bitmap header
Bytes 14..137   DIB header
Bytes 138...  Pixel array data

See bmp_gen.h for the specific header format structure

Use:
Make a 200,200 red filled 16 bit RGB565 bitmap called RED.bmp
MakeFilledBmpImageRGB565( "RED.bmp",200,200,imRED);

Compatibility info:
Tested under Windows with GNU compiler, using CodeBlocks IDE

/*

  Columns (X axis) are from left to right if one visualizes the pixel data as a 2D array
  Rows are every "width" pixels, but row 0 as displayed is the last row in the pixel data file

*/


void DrawLine(void * ImageData, uint32_t xstart,uint32_t ystart,uint32_t xend,uint32_t yend,IBMP_COL colour) {

     dib_mbpv5hdr *dib_hdr = (dib_mbpv5hdr* )(ImageData+imDIB_OFFSET);
     uint32_t h = dib_hdr->bV5Height;
     uint32_t w = dib_hdr->bV5Width;
     uint16_t *PixelData = (uint16_t *)(ImageData+imPIXEL_DATA_OFFS);
     uint32_t x,y;

     uint16_t *wrpos = PixelData + (h-ystart-1)*w  ;
     for(x=xstart;x<xend;x++) {
        *wrpos = (uint16_t)colour;
        wrpos++;
     }



}


/*******************************************************
FillImage
Floodfill the image data with a colour, use as a background fill
    imagedata is the actualpixel data buffer excluding all headers
    size is the size of the imagedata buffer
    colour is the fill colour
*******************************************************/
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

/*******************************************************
MakeDIBHeader
Populate the DIB header - we choose V5 header format
    dib_hdr supply the dib structure
    w,h width and height of image
    colour_depth in bytes (2 bytes = 16 bits for 16 bit RGB565)
*******************************************************/
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


/*******************************************************
MakeBMPHeader
Make the generic BMP header of 14 bytes
    bm_hdr supply the header structure
    w,h width and height of image
    colour_depth in bytes (2 bytes = 16 bits for 16 bit RGB565)
*******************************************************/
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


/*******************************************************
MakeFilledBmpImageRGB565
Make a BMP file od specified size, filled with specified colour
    bm_hdr supply the header structure
    w,h width and height of image
    colour_depth in bytes (2 bytes = 16 bits for 16 bit RGB565)
*******************************************************/
void MakeFilledBmpImageFileRGB565( const char * filename,
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
MakeFilledBmpImageRGB565
Make a BMP file od specified size, filled with specified colour
    bm_hdr supply the header structure
    w,h width and height of image
    colour_depth in bytes (2 bytes = 16 bits for 16 bit RGB565)
*******************************************************/
void * MakeFilledBmpImageRGB565(
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




