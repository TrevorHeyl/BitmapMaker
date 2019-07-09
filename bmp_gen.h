#ifndef _BMP_GEN_H_
#define _BMP_GEN_H_

#define imPIXEL_DATA_OFFS 0x8A
#define imDIBV5FMT_SIZE 0x7C
#define imRGBs_TYPE 0x73524742
#define imRGB565_REDMASK  0xf800
#define imRGB565_BLUEMASK 0x001f
#define imRGB565_GREENMASK 0x07e0
#define imPIXELS_PM 2835




/*Solid fill colours in RGB565 format  */
typedef enum IBMP_COL {
    imRED = 0xf800,
    imBLUE = 0x001f,
    imGREEN =  0x07e0,
    imPURPLE = 0xf81f,
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




void MakeFilledBmpImageRGB565( const char * filename,uint32_t w, uint32_t h, IBMP_COL col  );

#endif // _BMP_GEN_H_
