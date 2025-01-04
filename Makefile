# Compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -std=c11

# Executable name
TARGET = bmp_steganography

# Source and header files
SRC = main.c
HEADERS = bmp_header.h dib_header.h pixel.h

# Default target to build the executable
$(TARGET): $(SRC) $(HEADERS)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRC)

# Clean the compiled files
clean:
	rm -f $(TARGET)
