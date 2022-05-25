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

void get_pixels_from_bmp(int end, int rows, int cols, char *fileReadBuffer)
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

void export_image(char* fileBuffer, const char *nameOfFileToCreate, int bufferSize)
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

void handle_mirror_horizontal_filter(){
    pthread_t tids[TOTAL_THREADS];
    int ids[TOTAL_THREADS];
    for(int i = 0; i < TOTAL_THREADS; i++){
        ids[i] = i;
        int return_code = pthread_create(&tids[i], NULL, mirror_horizontal_filter, (void*)&ids[i]);
    }

    void* status;
    for(int i = 0; i < TOTAL_THREADS; i++){
        int ret = pthread_join(tids[i], &status);
    }
}

void handle_mirror_vertical_filter(){
    pthread_t tids[TOTAL_THREADS];
    int ids[TOTAL_THREADS];
    for(int i = 0; i < TOTAL_THREADS; i++){
        ids[i] = i;
        int return_code = pthread_create(&tids[i], NULL, mirror_vertical_filter, (void*)&ids[i]);
    }

    void* status;
    for(int i = 0; i < TOTAL_THREADS; i++){
        int ret = pthread_join(tids[i], &status);
    }
}

void handle_median_filter() {
    pthread_t tids[TOTAL_THREADS];
    int ids[TOTAL_THREADS];
    for(int i = 0; i < TOTAL_THREADS; i++){
        ids[i] = i;
        int return_code = pthread_create(&tids[i], NULL, median_filter, (void*)&ids[i]);
    }

    void* status;
    for(int i = 0; i < TOTAL_THREADS; i++){
        int ret = pthread_join(tids[i], &status);
    }
}

void handle_reverse_filter() {
  pthread_t tids[TOTAL_THREADS];
    int ids[TOTAL_THREADS];
    for(int i = 0; i < TOTAL_THREADS; i++){
        ids[i] = i;
        int return_code = pthread_create(&tids[i], NULL, reverse_filter, (void*)&ids[i]);
    }

    void* status;
    for(int i = 0; i < TOTAL_THREADS; i++){
        int ret = pthread_join(tids[i], &status);
    }
}

void handle_plus_sign_filter() {
    pthread_t tids[TOTAL_THREADS];
    int ids[TOTAL_THREADS];
    for(int i = 0; i < TOTAL_THREADS; i++){
        ids[i] = i;
        int return_code = pthread_create(&tids[i], NULL, plus_sign_filter, (void*)&ids[i]);
    }

    void* status;
    for(int i = 0; i < TOTAL_THREADS; i++){
        int ret = pthread_join(tids[i], &status);
    }
}

void* mirror_horizontal_filter(void* tid)
{
    int id = *((int*) tid);
    apply_mirror_horizontal_filter(id);
    pthread_exit(NULL);
}

void* mirror_vertical_filter(void* tid)
{
    int id = *((int*) tid);
    apply_mirror_vertical_filter(id);
    pthread_exit(NULL);
}

void* median_filter(void* tid) {
    int id = *((int*) tid);
    apply_median_filter(id);
    pthread_exit(NULL);
}

void* reverse_filter(void* tid) {
    int id = *((int*) tid);
    apply_reverse_filter(id);
    pthread_exit(NULL);
}

void* plus_sign_filter(void* tid) {
    int id = *((int*) tid);
    apply_plus_sign_filter(id);
    pthread_exit(NULL);
}



void apply_mirror_horizontal_filter(int color) {
    unsigned char temp;
    for (int i = 0; i < rows; i++) {
        for (int j = cols/2 - 1; j >= 0; j--) {
          if (color == RED) {
            temp = photo[i][j].red;
            photo[i][j].red = photo[i][cols-j].red;
            photo[i][cols -j].red = temp;
            
          } else if (color == GREEN) {
            temp = photo[i][j].green;
            photo[i][j].green = photo[i][cols-j].green;
            photo[i][cols -j].green = temp;
          } else {
            temp = photo[i][j].blue;
            photo[i][j].blue = photo[i][cols-j].blue;
            photo[i][cols -j].blue = temp;
          }
        }
    }
}

void apply_mirror_vertical_filter(int color) {
    unsigned char temp;
    for (int i = 0; i < rows/2; i++) {
        for (int j = cols - 1; j >= 0; j--) {
          if (color == RED) {
            temp = photo[i][j].red;
            photo[i][j].red = photo[rows - 1 - i][j].red;
            photo[rows - 1 - i][j].red = temp;
            
          } else if (color == GREEN) {
            temp = photo[i][j].green;
            photo[i][j].green = photo[rows - 1 - i][j].green;
            photo[rows - 1 - i][j].green = temp;
          } else {
            temp = photo[i][j].blue;
            photo[i][j].blue = photo[rows - 1 - i][j].blue;
            photo[rows - 1 - i][j].blue = temp;
          }
        }
    }
}

void apply_median_filter(int color) {
    unsigned char temp;
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
          if (color == RED) {
            int red_sum = 0;
            int count = 0;
            for (int k = i - 1; k <= i + 1; k++) {
                for (int l = j - 1; l <= j + 1; l++) {
                    if (k >= 0 && k < rows && l >= 0 && l < cols) {
                        red_sum += photo[k][l].red;
                        count++;
                    }
                }
            }
          } else if (color == GREEN) {
            int green_sum = 0;
            int count = 0;
            for (int k = i - 1; k <= i + 1; k++) {
                for (int l = j - 1; l <= j + 1; l++) {
                    if (k >= 0 && k < rows && l >= 0 && l < cols) {
                        green_sum += photo[k][l].green;
                        count++;
                    }
                }
            }
          } else {
            int blue_sum = 0;
            int count = 0;
            for (int k = i - 1; k <= i + 1; k++) {
                for (int l = j - 1; l <= j + 1; l++) {
                    if (k >= 0 && k < rows && l >= 0 && l < cols) {
                        blue_sum += photo[k][l].blue;
                        count++;
                    }
                }
            }
          }
        }
    }
}

void apply_reverse_filter(int color) {
    for (int i = 0; i < rows ; i++) {
        for (int j = cols - 1; j >= 0; j--) {
          if (color == RED) {
            photo[i][j].red = 255 - photo[i][j].red;
          } else if (color == GREEN) {
            photo[i][j].green = 255 - photo[i][j].green;
          } else {
            photo[i][j].blue = 255 - photo[i][j].blue;
          }
        }
    }
}

void apply_plus_sign_filter(int color) {
  for (int i = 0; i < rows ; i++) {
        for (int j = cols - 1; j >= 0; j--) {
          if(i==268 || j==268) {
            if (color == RED) {
              photo[i][j].red = 255;
            } else if (color == GREEN) {
              photo[i][j].green = 255;
            } else {
              photo[i][j].blue = 255;
            }
          }
        }
    }
}

void apply_filters() {
      handle_mirror_horizontal_filter();
      handle_mirror_vertical_filter();
      handle_median_filter();
      handle_reverse_filter();
      handle_plus_sign_filter();
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
    initialize_photo(rows, cols, photo);
    get_pixels_from_bmp(bufferSize, rows, cols, fileBuffer);

    apply_filters();

    export_image(fileBuffer, OUTPUT_PHOTO_NAME, bufferSize);

    auto end = high_resolution_clock::now();
    auto duration = duration_cast<milliseconds>(end - start);
    cout << "Duration: " << duration.count() << "ms" << endl;

    return 0;
}