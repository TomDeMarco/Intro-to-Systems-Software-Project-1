#ifndef PIXEL_H
#define PIXEL_H

#include <stdint.h> // Include for using fixed-width integer types

// Define the pixel struct with three 8-bit integers for color intensities
typedef struct {
    uint8_t blue;  // Blue intensity 
    uint8_t green; // Green intensity
    uint8_t red;   // Red intensity
} pixel;

unsigned char bit_swap(unsigned char color) {
    unsigned char msb = (color >> 4);   // upper 4 bits
    unsigned char lsb = (color & 0x0F); // lower 4 bits
    return (lsb << 4) | msb;           // swap and combine
}

#endif // PIXEL_H
