## Environment

  Ubuntu 22.04

  gcc version 11.4.0 (Ubuntu 11.4.0-1ubuntu1~22.04)

## How to compile

### Simple way (Makefile)

```
make
```

### Manual Compile

```
g++ -Wall -std=c++17 -o bin/hw2-1 src/hw2-1.cpp src/bmp.cpp
g++ -Wall -std=c++17 -o bin/hw2-2 src/hw2-2.cpp src/bmp.cpp
g++ -Wall -std=c++17 -o bin/hw2-3 src/hw2-3.cpp src/bmp.cpp
```

## How to run

### Simple way (Makefile)

```
make run
```

### Manual Run

```
bin/hw2-1 input/input1.bmp output/output1_1.bmp 0.5
bin/hw2-1 input/input1.bmp output/output1_2.bmp 0.7
bin/hw2-2 input/input2.bmp output/output2_1.bmp 3
bin/hw2-2 input/input2.bmp output/output2_2.bmp 10
bin/hw2-3 input/input3.bmp output/output3_1.bmp 5  1.0
bin/hw2-3 input/input3.bmp output/output3_2.bmp 11 10.0
```
