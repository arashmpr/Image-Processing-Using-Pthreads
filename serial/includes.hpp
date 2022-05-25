#ifndef INCLUDES_HPP
#define INCLUDES_HPP

#include <iostream>
#include <unistd.h>
#include <fstream>
#include <chrono>

using std::cout;
using std::endl;
using std::ifstream;
using std::ofstream;
using namespace std::chrono;

#pragma pack(1)
#pragma once

typedef int LONG;
typedef unsigned short WORD;
typedef unsigned int DWORD;

typedef struct rgb_pixel {
  unsigned char red;
  unsigned char blue;
  unsigned char green;
} pixel;

typedef struct tagBITMAPFILEHEADER
{
  WORD bfType;
  DWORD bfSize;
  WORD bfReserved1;
  WORD bfReserved2;
  DWORD bfOffBits;
} BITMAPFILEHEADER, *PBITMAPFILEHEADER;

typedef struct tagBITMAPINFOHEADER
{
  DWORD biSize;
  LONG biWidth;
  LONG biHeight;
  WORD biPlanes;
  WORD biBitCount;
  DWORD biCompression;
  DWORD biSizeImage;
  LONG biXPelsPerMeter;
  LONG biYPelsPerMeter;
  DWORD biClrUsed;
  DWORD biClrImportant;
} BITMAPINFOHEADER, *PBITMAPINFOHEADER;

int rows;
int cols;

void initialize_photo(int rows, int cols, pixel** &photo);
bool fillAndAllocate(char *&buffer, const char *fileName, int &rows, int &cols, int &bufferSize);
void get_pixels_from_bmp(int end, int rows, int cols, char *fileReadBuffer, pixel** &photo);
pixel** apply_mirror_horizontal_filter(pixel** photo);
pixel** apply_mirror_vertical_filter(pixel** photo);
pixel** apply_median_filter(pixel** photo);
pixel** apply_reverse_filter(pixel** photo);
pixel** apply_plus_sign_filter(pixel** photo);
bool is_on_diameter(int i, int j, int row);
void export_image(char* fileBuffer, const char *nameOfFileToCreate, int bufferSize, pixel** photo);

#endif