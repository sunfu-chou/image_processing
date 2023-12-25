# Compiler
CXX = g++

IWYU = iwyu

# Compiler flags
CXXFLAGS = -Wall -Wextra -std=c++17

INCLUDES = $(shell pkg-config --cflags opencv4)
LFLAGS = -L/usr/local/lib

LIBS = -lopencv_core -lopencv_imgproc -lopencv_highgui -lopencv_imgcodecs

# Source directory
SRC_DIR = src

# Output directory
OUT_DIR = bin

$(shell mkdir -p $(OUT_DIR))

# Targets
TARGET1 = $(OUT_DIR)/hw4-1

# All targets
TARGETS = $(TARGET1) $(TARGET2)

# Default target
all: $(TARGETS)

# Compile and link the program
$(OUT_DIR)/hw4-%: $(SRC_DIR)/hw4-%.cpp $(SRC_DIR)/bmp.cpp $(SRC_DIR)/psnr.cpp $(SRC_DIR)/restore_cv.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) -o $@ $^ $(LFLAGS) $(LIBS)

clean:
	rm -f $(OUT_DIR)/*

IN_IMG_DIR = input
OUT_IMG_DIR = output

$(shell mkdir -p $(OUT_IMG_DIR))


INPUT1 = $(IN_IMG_DIR)/input1.bmp
INPUT1_ORI = $(IN_IMG_DIR)/input1_ori.bmp
INPUT2 = $(IN_IMG_DIR)/input2.bmp


run: all
	LD_LIBRARY_PATH=/usr/local/lib:$LD_LIBRARY_PATH $(TARGET1) 0

clean_output:
	rm -rf $(OUT_IMG_DIR)/*
	@tree $(OUT_IMG_DIR)

iwyu:
	$(IWYU) $(SRC_DIR)/bmp.cpp $(CXXFLAGS) $(INCLUDES)
	$(IWYU) $(SRC_DIR)/psnr.cpp $(CXXFLAGS) $(INCLUDES)
	$(IWYU) $(SRC_DIR)/restore_cv.cpp $(CXXFLAGS) $(INCLUDES)
	$(IWYU) $(SRC_DIR)/hw4-1.cpp $(CXXFLAGS) $(INCLUDES)

docs:
	@doxygen
