# Compiler
CXX = g++

# Compiler flags
CXXFLAGS = -Wall -std=c++17

# Source directory
SRC_DIR = src

# Output directory
OUT_DIR = bin

# Targets
TARGET1 = hw2-1
TARGET2 = hw2-2
TARGET3 = hw2-3

IWYU = iwyu

all: $(TARGET1) $(TARGET2) $(TARGET3)

$(TARGET1): $(SRC_DIR)/$(TARGET1).cpp $(SRC_DIR)/bmp.cpp
	$(CXX) $(CXXFLAGS) -o $(OUT_DIR)/$(TARGET1) $^

$(TARGET2): $(SRC_DIR)/$(TARGET2).cpp $(SRC_DIR)/bmp.cpp
	$(CXX) $(CXXFLAGS) -o $(OUT_DIR)/$(TARGET2) $^

$(TARGET3): $(SRC_DIR)/$(TARGET3).cpp $(SRC_DIR)/bmp.cpp
	$(CXX) $(CXXFLAGS) -o $(OUT_DIR)/$(TARGET3) $^

INPUT1 = input1
INPUT2 = input2
INPUT3 = input3

run: all
	@$(OUT_DIR)/$(TARGET1) input/$(INPUT1).bmp output/output1_1.bmp 0.5
	@$(OUT_DIR)/$(TARGET1) input/$(INPUT1).bmp output/output1_2.bmp 0.7
	
	@$(OUT_DIR)/$(TARGET2) input/$(INPUT2).bmp output/output2_1.bmp 3
	@$(OUT_DIR)/$(TARGET2) input/$(INPUT2).bmp output/output2_2.bmp 10

	@$(OUT_DIR)/$(TARGET3) input/$(INPUT3).bmp output/output3_1.bmp 5  1.0
	@$(OUT_DIR)/$(TARGET3) input/$(INPUT3).bmp output/output3_2.bmp 11 10.0

	@tree output

.PHONY: clean

clean:
	rm -f $(OUT_DIR)/$(TARGET1) $(OUT_DIR)/$(TARGET2) $(OUT_DIR)/$(TARGET3)
	@tree bin

clean_output:
	rm -rf output/*
	@tree output

iwyu:
	$(IWYU) $(SRC_DIR)/bmp.cpp
	$(IWYU) $(SRC_DIR)/$(TARGET1).cpp
	$(IWYU) $(SRC_DIR)/$(TARGET2).cpp
	$(IWYU) $(SRC_DIR)/$(TARGET3).cpp

docs:
	@doxygen
