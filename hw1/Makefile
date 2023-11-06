# Compiler
CXX = g++

# Compiler flags
CXXFLAGS = -Wall -std=c++17

# Source directory
SRC_DIR = src

# Output directory
OUT_DIR = bin

# Targets
TARGET1 = hw1-1
TARGET2 = hw1-2
TARGET3 = hw1-3

IWYU = iwyu

all: $(TARGET1) $(TARGET2) $(TARGET3)

$(TARGET1): $(SRC_DIR)/hw1-1.cpp $(SRC_DIR)/bmp.cpp
	$(CXX) $(CXXFLAGS) -o $(OUT_DIR)/$(TARGET1) $^

$(TARGET2): $(SRC_DIR)/hw1-2.cpp $(SRC_DIR)/bmp.cpp
	$(CXX) $(CXXFLAGS) -o $(OUT_DIR)/$(TARGET2) $^

$(TARGET3): $(SRC_DIR)/hw1-3.cpp $(SRC_DIR)/bmp.cpp
	$(CXX) $(CXXFLAGS) -o $(OUT_DIR)/$(TARGET3) $^

run: all
	$(OUT_DIR)/$(TARGET1) input/input1.bmp output/output1/1-flipped.bmp

	$(OUT_DIR)/$(TARGET1) input/input2.bmp output/output2/1-flipped.bmp

	$(OUT_DIR)/$(TARGET2) input/input2.bmp output/output2/2-2bit.bmp 2
	$(OUT_DIR)/$(TARGET2) input/input2.bmp output/output2/2-4bit.bmp 4
	$(OUT_DIR)/$(TARGET2) input/input2.bmp output/output2/2-6bit.bmp 6

	$(OUT_DIR)/$(TARGET2) input/input1.bmp output/output1/2-2bit.bmp 2
	$(OUT_DIR)/$(TARGET2) input/input1.bmp output/output1/2-4bit.bmp 4
	$(OUT_DIR)/$(TARGET2) input/input1.bmp output/output1/2-6bit.bmp 6

	$(OUT_DIR)/$(TARGET3) input/input1.bmp output/output1/3-down.bmp 1.5 0 0
	$(OUT_DIR)/$(TARGET3) input/input1.bmp output/output1/extra/3-down-crop.bmp 1.5 0 1
	$(OUT_DIR)/$(TARGET3) input/input1.bmp output/output1/3-up.bmp 1.5 1 0
	$(OUT_DIR)/$(TARGET3) input/input1.bmp output/output1/extra/3-up-crop.bmp 1.5 1 1

	$(OUT_DIR)/$(TARGET3) input/input2.bmp output/output2/3-down.bmp 1.5 0 0
	$(OUT_DIR)/$(TARGET3) input/input2.bmp output/output2/extra/3-down-crop.bmp 1.5 0 1
	$(OUT_DIR)/$(TARGET3) input/input2.bmp output/output2/3-up.bmp 1.5 1 0
	$(OUT_DIR)/$(TARGET3) input/input2.bmp output/output2/extra/3-up-crop.bmp 1.5 1 1

		$(OUT_DIR)/$(TARGET1) input/test1.bmp output/test1/1-flipped.bmp

	$(OUT_DIR)/$(TARGET1) input/test2.bmp output/test2/1-flipped.bmp

	$(OUT_DIR)/$(TARGET2) input/test2.bmp output/test2/2-2bit.bmp 2
	$(OUT_DIR)/$(TARGET2) input/test2.bmp output/test2/2-4bit.bmp 4
	$(OUT_DIR)/$(TARGET2) input/test2.bmp output/test2/2-6bit.bmp 6

	$(OUT_DIR)/$(TARGET2) input/test1.bmp output/test1/2-2bit.bmp 2
	$(OUT_DIR)/$(TARGET2) input/test1.bmp output/test1/2-4bit.bmp 4
	$(OUT_DIR)/$(TARGET2) input/test1.bmp output/test1/2-6bit.bmp 6

	$(OUT_DIR)/$(TARGET3) input/test1.bmp output/test1/3-down.bmp 1.5 0 0
	$(OUT_DIR)/$(TARGET3) input/test1.bmp output/test1/extra/3-down-crop.bmp 1.5 0 1
	$(OUT_DIR)/$(TARGET3) input/test1.bmp output/test1/3-up.bmp 1.5 1 0
	$(OUT_DIR)/$(TARGET3) input/test1.bmp output/test1/extra/3-up-crop.bmp 1.5 1 1

	$(OUT_DIR)/$(TARGET3) input/test2.bmp output/test2/3-down.bmp 1.5 0 0
	$(OUT_DIR)/$(TARGET3) input/test2.bmp output/test2/extra/3-down-crop.bmp 1.5 0 1
	$(OUT_DIR)/$(TARGET3) input/test2.bmp output/test2/3-up.bmp 1.5 1 0
	$(OUT_DIR)/$(TARGET3) input/test2.bmp output/test2/extra/3-up-crop.bmp 1.5 1 1

	tree output

.PHONY: clean

clean:
	rm -f $(OUT_DIR)/$(TARGET1) $(OUT_DIR)/$(TARGET2) $(OUT_DIR)/$(TARGET3)

clean_output:
	rm -rf output/*/*

iwyu:
	$(IWYU) $(SRC_DIR)/bmp.cpp
	$(IWYU) $(SRC_DIR)/hw1-1.cpp
	$(IWYU) $(SRC_DIR)/hw1-2.cpp
	$(IWYU) $(SRC_DIR)/hw1-3.cpp
	
docs:
	@doxygen
