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
g++ -Wall -Wextra -std=c++17 -I/usr/local/include/opencv4 -o bin/hw4-1 src/hw4-1.cpp src/bmp.cpp src/psnr.cpp src/restore_cv.cpp -L/usr/local/lib -lopencv_core -lopencv_imgproc -lopencv_highgui -lopencv_imgcodecs
```

## How to run

### Simple way (Makefile)

```
make run
```

### Manual Run

```
LD_LIBRARY_PATH=/usr/local/lib:D_LIBRARY_PATH bin/hw4-1 0
```
