#ifndef INCLUDES_HPP
#define INCLUDES_HPP

#include <iostream>
#include <stdlib.h>
#include <unistd.h>
#include <fstream>
#include <pthread.h>
#include <chrono>

using std::cout;
using std::endl;
using std::ifstream;
using std::ofstream;
using std::sort;
using namespace std::chrono;

#pragma pack(1)
#pragma once

#define TOTAL_THREADS 3
#define RED 0
#define GREEN 1
#define BLUE 2

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
pixel** photo;


void initialize_photo(int rows, int cols, pixel** &photo);
bool fillAndAllocate(char *&buffer, const char *fileName, int &rows, int &cols, int &bufferSize);
void get_pixels_from_bmp(int end, int rows, int cols, char *fileReadBuffer, pixel** &photo);
void apply_filters();

void handle_mirror_horizontal_filter();
void* mirror_horizontal_filter(void* tid);
void apply_mirror_horizontal_filter(int color);

void handle_mirror_vertical_filter();
void* mirror_vertical_filter(void* tid);
void apply_mirror_vertical_filter(int color);

void handle_median_filter();
void* median_filter(void* tid);
void apply_median_filter(int color);

void handle_reverse_filter();
void* reverse_filter(void* tid);
void apply_reverse_filter(int color);

void handle_plus_sign_filter();
void* plus_sign_filter(void* tid);
void apply_plus_sign_filter(int color);

void export_image(char* fileBuffer, const char *nameOfFileToCreate, int bufferSize);

#endif