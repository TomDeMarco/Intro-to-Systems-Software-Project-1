#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "bmp_header.h"
#include "dib_header.h"
#include "pixel.h"


void test_bmp(const char *filename, const char *filename2 ,int info, int reveal, int hide) {

    //Error Statements-------------------------------------------------------------------------------------------------------------
    //check first file
    FILE *file1 = fopen(filename, "rb+"); //open file for read and write
    if (!file1) {
        printf("ERROR: File not found.\n");
        return;
    }

    //check second file if hiding
    FILE *file2 = NULL;
    if(hide){
        file2 = fopen(filename2, "rb+"); //only open file2 for read and write if neccesary
        if (!file2) {
            printf("ERROR: File not found.\n");
            fclose(file1); //close file after every check
            return;
        }
    }

    //start of first file checks----------------------------------------------

    //read and validate the BMP header
    bmp_header_type bmpHeader1; 
    fread(&bmpHeader1, sizeof(bmp_header_type), 1, file1);

    // validate the BMP format (must be 'BM')
    if (bmpHeader1.format[0] != 'B' || bmpHeader1.format[1] != 'M') {
        printf("ERROR: The format is not supported.\n");
        fclose(file1);
        return;
    }

    //read and validate the DIB header
    dib_header_type dibHeader1; 
    fread(&dibHeader1, sizeof(dib_header_type), 1, file1);

    //validate the DIB header size (must be 40)
    if (dibHeader1.header_size != 40) {
        printf("ERROR: The format is not supported.\n");
        fclose(file1);
        return;
    }

    //validate that the BMP is 24 bits per pixel
    if (dibHeader1.bits_per_pixel != 24) {
        printf("ERROR: The format is not supported.\n");
        fclose(file1);
        return;
    }

    //end of checks for first file ----------------------------------------------------------

    if(hide){
    //start of checks for file 2--------------------------------------------------------

        //read and validate the BMP header
        bmp_header_type bmpHeader2; 
        fread(&bmpHeader2, sizeof(bmp_header_type), 1, file2);

        // validate the BMP format (must be 'BM')
        if (bmpHeader2.format[0] != 'B' || bmpHeader2.format[1] != 'M') {
            printf("ERROR: The format is not supported.\n");
            fclose(file1); //must close both files
            fclose(file2);
            return;
        }

        //read and validate the DIB header
        dib_header_type dibHeader2; 
        fread(&dibHeader2, sizeof(dib_header_type), 1, file2);

        //validate the DIB header size (must be 40)
        if (dibHeader2.header_size != 40) {
            printf("ERROR: The format is not supported.\n");
            fclose(file1); //must close both files
            fclose(file2);
            return;
        }

        //validate that the BMP is 24 bits per pixel
        if (dibHeader2.bits_per_pixel != 24) {
            printf("ERROR: The format is not supported.\n");
            fclose(file1);
            fclose(file2); //must close both files
            return;
        }

        // check if both images have the same dimensions
        if ((dibHeader1.width != dibHeader2.width) || (dibHeader1.height != dibHeader2.height)) {
            printf("ERROR: The format is not supported.\n");
            fclose(file1);
            fclose(file2);
            return;
        }
    }

    //end of checks for file 2 -----------------------------------------------------------

    //End of Error Statements------------------------------------------------------------------------------------------------------------

    // implements info---------------------------------------------------------------------------------------------------------------------------
    if (info) {
        printf("=== BMP Header ===\n");
        printf("Type: %c%c\n", bmpHeader1.format[0], bmpHeader1.format[1]);
        printf("Size: %u\n", bmpHeader1.file_size);
        printf("Reserved 1: %u\n", bmpHeader1.reserved1);
        printf("Reserved 2: %u\n", bmpHeader1.reserved2);
        printf("Image offset: %u\n", bmpHeader1.offset);

        printf("=== DIB Header ===\n");
        printf("Size: %u\n", dibHeader1.header_size);
        printf("Width: %u\n", dibHeader1.width);
        printf("Height: %u\n", dibHeader1.height);
        printf("# color planes: %u\n", dibHeader1.color_planes);
        printf("# bits per pixel: %u\n", dibHeader1.bits_per_pixel);
        printf("Compression scheme: %u\n", dibHeader1.compression);
        printf("Image size: %u\n", dibHeader1.image_size);
        printf("Horizontal resolution: %u\n", dibHeader1.x_resolution);
        printf("Vertical resolution: %u\n", dibHeader1.y_resolution);
        printf("# colors in palette: %u\n", dibHeader1.colors);
        printf("# important colors: %u\n", dibHeader1.important_colors);

        //close the file
        fclose(file1);
        return;
    }

    //end info -----------------------------------------------------------------------------------------------------------------

    // Move the file pointer to the pixel array's starting position using the offset from the BMP header
    fseek(file1, bmpHeader1.offset, SEEK_SET);

    // Calculate padding: width in bytes rounded up to closest multiple of 4 for both hide and reveal depending on input
    int row_padded = (dibHeader1.width * 3 + 3) & (~3);
    int padding = row_padded - dibHeader1.width * 3;



    //implements reveal -------------------------------------------------------------------------------------------------------------------------
    if(reveal){
    // Process each pixel row by row
    for (unsigned int i = 0; i < dibHeader1.height; i++) {
        for (unsigned int j = 0; j < dibHeader1.width; j++) {
            pixel pix;
            fread(&pix, sizeof(pixel), 1, file1); // Read pixel
            
            // print the pixel values before modification
            //printf("Pixel at row %u, col %u - Before modification: R=%u, G=%u, B=%u\n", 
               // i, j, pix.red, pix.green, pix.blue);

                // Swap the bits of each color
                pix.red = bit_swap(pix.red);
                pix.blue = bit_swap(pix.blue);
                pix.green = bit_swap(pix.green);

            //Print the pixel values after modifying
            //printf("Pixel at row %u, col %u - After modification: R=%u, G=%u, B=%u\n", 
                //i, j, pix.red, pix.green, pix.blue);

            //move the file pointer back to the pixel position for writing
            fseek(file1, -sizeof(pixel), SEEK_CUR);

            //Print the current file position before writing
            //long position_before_write = ftell(file);
            //printf("File position before writing pixel at row %u, col %u: %ld\n", i, j, position_before_write);


            // write modified pixel back
            fwrite(&pix, sizeof(pixel), 1, file1);

            //Print the current file position after writing the pixel
            //long position_after_write = ftell(file);
            //printf("File position after writing pixel at row %u, col %u: %ld\n", i, j, position_after_write);
        }
        // Skip padding bytes at the end of each row
        fseek(file1, padding, SEEK_CUR);

        // Print after skipping padding
        //printf("Skipped padding of %d bytes after row %u\n", padding, i);
    }
    }
    //end reveal-------------------------------------------------------------------------------------------------------------

    //implements hide -------------------------------------------------------------------------------------------------------------------
    if(hide){
            // Process each pixel row by row
    for (unsigned int i = 0; i < dibHeader1.height; i++) {
        for (unsigned int j = 0; j < dibHeader1.width; j++) {
            pixel pix1, pix2;

            fread(&pix1, sizeof(pixel), 1, file1); // Read pixel for file 1
            fread(&pix2, sizeof(pixel), 1, file2); //read pixel for file 2

            // Print pixel values before modification (source image and hidden image)
            //printf("Before modification at row %u, col %u: FILENAME1 Pixel (R=%u, G=%u, B=%u), FILENAME2 Pixel (R=%u, G=%u, B=%u)\n",
                  // i, j, pix1.red, pix1.green, pix1.blue, pix2.red, pix2.green, pix2.blue);

            //hide 4msb of pix 2 into the 4lsb of pix1!
            pix1.red = (pix1.red & 0xF0) | (pix2.red >> 4); //red
            pix1.green = (pix1.green & 0xF0) | (pix2.green >> 4); //green
            pix1.blue = (pix1.blue & 0xF0) | (pix2.blue >> 4);//blue

            // Print pixel values after modification
            //printf("After modification at row %u, col %u: FILENAME1 Pixel (R=%u, G=%u, B=%u)\n",
                  // i, j, pix1.red, pix1.green, pix1.blue);

            //go back to pixel we want to write too
            fseek(file1,-sizeof(pixel),SEEK_CUR);

            //write modified pixel back
            fwrite(&pix1, sizeof(pixel), 1, file1);
        }

    //skip padding for each file
    fseek(file1, padding, SEEK_CUR);
    fseek(file2, padding, SEEK_CUR);
    }
    fclose(file2);
    }
    //end hide////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    fclose(file1);

} 

//main-----------------------------------------------------------------------------------
int main(int argc, char *argv[]) {
    if (argc <= 1) {
        printf("ERROR: Missing arguments.\n");
        return 1;
    }

    for (int i = 1; i < argc; i++) {
        //check if the argument is --info
        if (strcmp(argv[i], "--info") == 0) {
            //the next argument should be the filename
            //printf("info is running ");
            if (i + 1 < argc) {
                //printf("\nTesting %s:\n", argv[i + 1]);
                test_bmp(argv[i + 1],NULL, 1, 0,0); // Set info to 1 and reveal and hide to 0
                i++; 
            } else {
                printf("ERROR: Missing arguments.\n");
                return 1;
            }
        } else if (strcmp(argv[i], "--reveal") == 0) {
            // next argument should be the filename
            //printf("Reveal is running ");
            if (i + 1 < argc) {
                //printf("\nRevealing hidden image in %s:\n", argv[i + 1]);
                test_bmp(argv[i + 1],NULL,0, 1,0); // Set info to 0, hide to 0 and reveal to 1
                i++; 
            } else {
                printf("ERROR: Missing arguments.");
                return 1;
            }

        } else if (strcmp(argv[i], "--hide") == 0) {
            // next argument should be the filename
            //printf("hide is running ");
            if (i + 2 < argc) {
                //printf("\nhiding %s in %s:\n", argv[i + 2], argv[i+1]);
                test_bmp(argv[i + 1], argv[i + 2] ,0, 0,1); // Set info to 0 and reveal to 0 and set hide to 1
                i+=2; 
            } else {
                printf("ERROR: Missing arguments.");
                return 1;
            }
        } 
    }
    return 0;
}
//end main-----------------------------------------------------------------------------------