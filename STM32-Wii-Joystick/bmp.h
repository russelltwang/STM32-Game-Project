/*Filename: bmp.h*
 *Part of: Lab 10 *
 *Created By:Michael Ledesma(mledesma)*
 *Created On:04/03/2014*
 *Last Modified by:Michael Ledesma(mledesma)*
 *Last Modified on:04/03/2014*
 */

//BMP File structures (Lab Manual listing 12.3, p . 187)                        

struct bmpfile_magic {
  unsigned char magic[2];
};

struct bmpfile_header {
  uint32_t filesz;
  uint16_t creator1;
  uint16_t creator2;
  uint32_t bmp_offset;
};

typedef struct {
  uint32_t header_sz;
  uint32_t width;
  uint32_t height;
  uint16_t nplanes;
  uint16_t bitspp;
  uint32_t compress_type;
  uint32_t bmp_bytesz;
  int32_t hres;
  int32_t vres;
  uint32_t ncolors;
  uint32_t nimpcolors;
} BITMAPINFOHEADER;

struct bmppixel {//little endian                                                
  uint8_t b;
  uint8_t g;
  uint8_t r;
};

