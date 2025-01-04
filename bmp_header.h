#ifndef BMP_HEADER_H
#define BMP_HEADER_H

#include <stdint.h>

#pragma pack(1)
typedef struct {
    char format[2];      // Format identifier (should be 'BM')
    unsigned int file_size;    //Size of the BMP file in bytes
    unsigned short reserved1;  //Reserved
    unsigned short reserved2;  //Reserved
    unsigned int offset;       //Offset to start of pixel array
} bmp_header_type;
#pragma pack(4)


#endif // BMP_HEADER_H
