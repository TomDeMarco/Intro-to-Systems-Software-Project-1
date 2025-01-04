#ifndef DIB_HEADER_H
#define DIB_HEADER_H

#include <stdint.h>

#pragma pack(1)
typedef struct {
    unsigned int header_size;      //Size of this header (should be 40 for the Windows BMP format)
    unsigned int width;            //Image width
    unsigned int height;           //Image height
    unsigned short color_planes;   //Number of color planes 
    unsigned short bits_per_pixel; //Bits per pixel (should be 24 for 24-bit RGB)
    unsigned int compression;      //Compression type 
    unsigned int image_size;       //Image size 
    unsigned int x_resolution;     //Horizontal resolution
    unsigned int y_resolution;     //Vertical resolution
    unsigned int colors;           //Number of colors in palette
    unsigned int important_colors; //Important colors used
} dib_header_type;
#pragma pack(4)


#endif // DIB_HEADER_H
