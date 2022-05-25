CC = g++
BUILD_DIR = build
CFLAGS = -std=c++11

EXECUTABLE_FILE = ImageFilters.out

MAIN_OBJECTS = \
	$(BUILD_DIR)/image.o

ImageSensitivityList = \
	image.cpp \
	includes.hpp 

all: $(BUILD_DIR) $(EXECUTABLE_FILE)

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(EXECUTABLE_FILE): $(MAIN_OBJECTS)
	$(CC) $(MAIN_OBJECTS) $(CFLAGS) -o $(EXECUTABLE_FILE)


$(BUILD_DIR)/image.o: $(ImageSensitivityList)
	$(CC) -c image.cpp $(CFLAGS) -o $(BUILD_DIR)/image.o

.PHONY: clean
clean:
	rm -rf $(BUILD_DIR) *.o *.out output.bmp