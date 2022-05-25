#include "includes.hpp"

void initialize_photo(int rows, int cols, pixel** &photo) {
    photo = new pixel*[rows];
    for (int row = 0 ; row < rows ; row++) {
        photo[row] = new pixel[cols];
    }
}

bool fillAndAllocate(char *&buffer, const char *fileName, int &rows, int &cols, int &bufferSize)
{
  std::ifstream file(fileName);

  if (file)
  {
    file.seekg(0, std::ios::end);
    std::streampos length = file.tellg();
    file.seekg(0, std::ios::beg);

    buffer = new char[length];
    file.read(&buffer[0], length);

    PBITMAPFILEHEADER file_header;
    PBITMAPINFOHEADER info_header;

    file_header = (PBITMAPFILEHEADER)(&buffer[0]);
    info_header = (PBITMAPINFOHEADER)(&buffer[0] + sizeof(BITMAPFILEHEADER));
    rows = info_header->biHeight;
    cols = info_header->biWidth;
    bufferSize = file_header->bfSize;
    return 1;
  }
  else
  {
    cout << "File" << fileName << " doesn't exist!" << endl;
    return 0;
  }
}

void get_pixels_from_bmp(int end, int rows, int cols, char *fileReadBuffer, pixel** &photo)
{
  int count = 1;
  int extra = cols % 4;
  for (int i = 0; i < rows; i++)
  {
    count += extra;
    for (int j = cols - 1; j >= 0; j--)
      for (int k = 0; k < 3; k++)
      {
        switch (k)
        {
        case 0:
            photo[i][j].red = fileReadBuffer[end - count];
            break;
        case 1:
            photo[i][j].green = fileReadBuffer[end - count];
            break;
        case 2:
            photo[i][j].blue = fileReadBuffer[end - count];
            break;
        }
        count++;
      }
  }
}

void export_image(char* fileBuffer, const char *nameOfFileToCreate, int bufferSize, pixel** photo)
{
    std::ofstream write(nameOfFileToCreate);
    if (!write) {
        cout << "Failed to write " << nameOfFileToCreate << endl;
        return;
    }
    int count = 1;
    int extra = cols % 4;
    for (int i = 0; i < rows; i++)
    {
        count += extra;
        for (int j = cols - 1; j >= 0; j--)
            for (int k = 0; k < 3; k++)
            {
                switch (k)
                {
                    case 0:
                        fileBuffer[bufferSize - count] = photo[i][j].red;
                        break;
                    case 1:
                        fileBuffer[bufferSize - count] = photo[i][j].green;
                        break;
                    case 2:
                        fileBuffer[bufferSize - count] = photo[i][j].blue;
                        break;
                }
                count++;
            }
    }
  write.write(fileBuffer, bufferSize);
}

pixel** apply_mirror_horizontal_filter(pixel** photo) {
    pixel** photo_with_filter;
    initialize_photo(rows, cols, photo_with_filter);
    for (int i = 0; i < rows; i++) {
        for (int j = cols - 1; j >= 0; j--) {
            photo_with_filter[i][cols-j].red = photo[i][j].red;
            photo_with_filter[i][cols-j].green = photo[i][j].green;
            photo_with_filter[i][cols-j].blue = photo[i][j].blue;
        }
    }

    return photo_with_filter;
}

pixel** apply_mirror_vertical_filter(pixel** photo) {
    pixel** photo_with_filter;
    initialize_photo(rows, cols, photo_with_filter);
    for (int i = 0; i < rows; i++) {
        for (int j = cols - 1; j >= 0; j--) {
            photo_with_filter[rows - 1 - i][j].red = photo[i][j].red;
            photo_with_filter[rows - 1 - i][j].green = photo[i][j].green;
            photo_with_filter[rows - 1 - i][j].blue = photo[i][j].blue;
        }
    }

    return photo_with_filter;
}

pixel** apply_median_filter(pixel** photo) {
    pixel** photo_with_filter;
    initialize_photo(rows, cols, photo_with_filter);
    for (int i = 0; i < rows; i++) {
        for (int j = cols - 1; j >= 0; j--) {
            int red_sum = 0;
            int green_sum = 0;
            int blue_sum = 0;
            int count = 0;
            for (int k = i - 1; k <= i + 1; k++) {
                for (int l = j - 1; l <= j + 1; l++) {
                    if (k >= 0 && k < rows && l >= 0 && l < cols) {
                        red_sum += photo[k][l].red;
                        green_sum += photo[k][l].green;
                        blue_sum += photo[k][l].blue;
                        count++;
                    }
                }
            }
            photo_with_filter[i][j].red = red_sum / count;
            photo_with_filter[i][j].green = green_sum / count;
            photo_with_filter[i][j].blue = blue_sum / count;
        }
    }
    return photo_with_filter;
}

pixel** apply_reverse_filter(pixel** photo) {
    pixel** photo_with_filter;
    initialize_photo(rows, cols, photo_with_filter);
    for (int i = 0; i < rows; i++) {
        for (int j = cols - 1; j >= 0; j--) {
            photo_with_filter[i][j].red = (255 - photo[i][j].red);
            photo_with_filter[i][j].green = (255 - photo[i][j].green);
            photo_with_filter[i][j].blue = (255 - photo[i][j].blue);
        }
    }
    return photo_with_filter;
}

pixel** apply_plus_sign_filter(pixel** photo) {
    pixel** photo_with_filter;
    initialize_photo(rows, cols, photo_with_filter);
    for (int i = 0; i < rows; i++) {
        for (int j = cols - 1; j >= 0; j--) {
            if(is_on_diameter(i, j, rows)) {
                photo_with_filter[i][j].red = 0;
                photo_with_filter[i][j].green = 0;
                photo_with_filter[i][j].blue = 0;
            } else {
                photo_with_filter[i][j].red = photo[i][j].red;
                photo_with_filter[i][j].green = photo[i][j].green;
                photo_with_filter[i][j].blue = photo[i][j].blue;
            }
        }
    }
    return photo_with_filter;
}


bool is_on_diameter(int i, int j, int row) {
    if (i == 268 || j == 268) {
        return true;
    }
    return false;
}

int main(int argc, char *argv[])
{
    char *fileBuffer;
    int bufferSize;
    const char* OUTPUT_PHOTO_NAME = "output.bmp";
    char *fileName = argv[1];
    auto start = high_resolution_clock::now();
    if (!fillAndAllocate(fileBuffer, fileName, rows, cols, bufferSize)) {
        cout << "File read error" << endl;
        return 1;
    }
    pixel** photo1;
    initialize_photo(rows, cols, photo1);
    get_pixels_from_bmp(bufferSize, rows, cols, fileBuffer, photo1);

    pixel** mirror_horizontal = apply_mirror_horizontal_filter(photo1);
    pixel** mirror_vertical = apply_mirror_vertical_filter(mirror_horizontal);
    pixel** median = apply_median_filter(mirror_vertical);
    pixel** reverse = apply_reverse_filter(median);
    pixel** plus_sign = apply_plus_sign_filter(reverse);
    

    export_image(fileBuffer, OUTPUT_PHOTO_NAME, bufferSize, plus_sign);

    auto end = high_resolution_clock::now();
    auto duration = duration_cast<milliseconds>(end - start);
    cout << "Duration: " << duration.count() << "ms" << endl;

    return 0;
}