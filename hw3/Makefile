# Compiler
CXX = g++

# Compiler flags
CXXFLAGS = -Wall -Wextra -std=c++17

IWYU = iwyu

# Source directory
SRC_DIR = src

# Output directory
OUT_DIR = bin

$(shell mkdir -p $(OUT_DIR))

# Targets
TARGET1 = $(OUT_DIR)/hw3-1
TARGET2 = $(OUT_DIR)/hw3-2
TARGETS = $(TARGET1) $(TARGET2)

all: $(TARGETS)

$(OUT_DIR)/hw3-%: $(SRC_DIR)/hw3-%.cpp $(SRC_DIR)/bmp.cpp
	$(CXX) $(CXXFLAGS) -o $@ $^

clean:
	rm -f $(OUT_DIR)/*

IN_IMG_DIR = input
OUT_IMG_DIR = output

$(shell mkdir -p $(OUT_IMG_DIR))


INPUT1 = $(IN_IMG_DIR)/input1.bmp
INPUT2 = $(IN_IMG_DIR)/input2.bmp
INPUT3 = $(IN_IMG_DIR)/input3.bmp
INPUT4 = $(IN_IMG_DIR)/input4.bmp

INPUTS = $(INPUT1) $(INPUT2) $(INPUT3) $(INPUT4)

run: all
	@$(TARGET1) $(INPUT1) $(OUT_IMG_DIR)/output1_1.bmp
	@$(TARGET1) $(INPUT2) $(OUT_IMG_DIR)/output2_1.bmp
	@$(TARGET1) $(INPUT3) $(OUT_IMG_DIR)/output3_1.bmp
	@$(TARGET1) $(INPUT4) $(OUT_IMG_DIR)/output4_1.bmp
	@$(TARGET2) $(OUT_IMG_DIR)/output1_1.bmp $(OUT_IMG_DIR)/output1_2.bmp --sequence HSIGA --hue -5 --saturation 1.6 --intensity 1.1 --gamma 0.6 --sharpness 0.2
	@$(TARGET2) $(OUT_IMG_DIR)/output2_1.bmp $(OUT_IMG_DIR)/output2_2.bmp --sequence CHSIG --contrast 0.9 --hue -5 --saturation 0.8 --intensity 1.2 --gamma 0.8  
	@$(TARGET2) $(OUT_IMG_DIR)/output3_1.bmp $(OUT_IMG_DIR)/output3_2.bmp --sequence SG --saturation 1.5 --gamma 0.75
	@$(TARGET2) $(OUT_IMG_DIR)/output4_1.bmp $(OUT_IMG_DIR)/output4_2.bmp --sequence CSIG --contrast 0.7 --saturation 1.3 --intensity 1.1 --gamma 1.4

clean_output:
	rm -rf $(OUT_IMG_DIR)/*
	@tree $(OUT_IMG_DIR)

iwyu:
	$(IWYU) $(SRC_DIR)/bmp.cpp
	$(IWYU) $(SRC_DIR)/hw3-1.cpp
	$(IWYU) $(SRC_DIR)/hw3-2.cpp

docs:
	@doxygen
