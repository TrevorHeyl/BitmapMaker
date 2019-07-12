# BitmapMaker

**C parameteric Bitmap generation and drawing utility**

Utility to genearte bitmap files according to the BMP/DIB file format (bitmap)
The BITMAPV5HEADER type is used as the DIB header spec'd here:
*https://docs.microsoft.com/en-us/windows/win32/api/wingdi/ns-wingdi-bitmapv5header*

**Features**
* Creates bitmap images according to V5 format from C-code
* Saves files in RGB565 16 bit colour mode
* Create any sixe image with specified background colour
* Draw lines,Squares and Rectangles
* Option to exclude/include padding in image pixel data

**Revision History:**
9 July 2019 : First version, Generates any size BMP of 16 bit RGB565 colour format, 4 colours but many can be added

**The specific format here is as follows:**
Bytes 0-13 : Bitmap header
Bytes 14..137   DIB header
Bytes 138...  Pixel array data

See bmp_gen.h for the specific header format structure

**Usage:**
Make a 200,200 red filled 16 bit RGB565 bitmap called RED.bmp
MakeFilledBmpImageRGB565( "RED.bmp",200,200,imRED);

**Compatibility info:**
Tested under Windows with GNU compiler, using CodeBlocks IDE

**Samples**
Coloured bitmaps
![Red](https://github.com/TrevorHeyl/BitmapMaker/blob/master/RED.bmp)
![Lines](https://github.com/TrevorHeyl/BitmapMaker/blob/master/DIAMOND.bmp)
![Boxes](https://github.com/TrevorHeyl/BitmapMaker/blob/master/BOXES.bmp)
![Squares](https://github.com/TrevorHeyl/BitmapMaker/blob/master/SQUARES.bmp)


