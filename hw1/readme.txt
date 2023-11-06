## Environment
  Ubuntu 22.04
  gcc version 11.4.0 (Ubuntu 11.4.0-1ubuntu1~22.04)

## How to compile
  ### Simple way
    make
  ### Manual Compile
    g++ -Wall -std=c++17 -o bin/hw1-1 src/hw1-1.cpp src/bmp.cpp
    g++ -Wall -std=c++17 -o bin/hw1-2 src/hw1-2.cpp src/bmp.cpp
    g++ -Wall -std=c++17 -o bin/hw1-3 src/hw1-3.cpp src/bmp.cpp
## How to run
  ### Simple way
    make run
  ### Manual Compile
    bin/hw1-1 input/input1.bmp output/output1/1-flipped.bmp
    bin/hw1-1 input/input2.bmp output/output2/1-flipped.bmp
    bin/hw1-2 input/input2.bmp output/output2/2-2bit.bmp 2
    bin/hw1-2 input/input2.bmp output/output2/2-4bit.bmp 4
    bin/hw1-2 input/input2.bmp output/output2/2-6bit.bmp 6
    bin/hw1-2 input/input1.bmp output/output1/2-2bit.bmp 2
    bin/hw1-2 input/input1.bmp output/output1/2-4bit.bmp 4
    bin/hw1-2 input/input1.bmp output/output1/2-6bit.bmp 6
    bin/hw1-3 input/input1.bmp output/output1/3-down.bmp 1.5 0 0
    bin/hw1-3 input/input1.bmp output/output1/extra/3-down-crop.bmp 1.5 0 1
    bin/hw1-3 input/input1.bmp output/output1/3-up.bmp 1.5 1 0
    bin/hw1-3 input/input1.bmp output/output1/extra/3-up-crop.bmp 1.5 1 1
    bin/hw1-3 input/input2.bmp output/output2/3-down.bmp 1.5 0 0
    bin/hw1-3 input/input2.bmp output/output2/extra/3-down-crop.bmp 1.5 0 1
    bin/hw1-3 input/input2.bmp output/output2/3-up.bmp 1.5 1 0
    bin/hw1-3 input/input2.bmp output/output2/extra/3-up-crop.bmp 1.5 1 1
