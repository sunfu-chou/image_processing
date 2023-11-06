/**
 * @file bmp.cpp
 * @author sunfu-chou (sunfu-chou@gmail.com)
 * @brief This file contains the declarations of the BMPFileHeader, BMPInfoHeader, Pixel, and BMPImage classes.
 * The BMPFileHeader and BMPInfoHeader classes represent the file and information headers of a BMP image, respectively.
 * The Pixel class represents a single pixel in a BMP image, and the BMPImage class represents the entire BMP image.
 * This file provides the necessary functionality to read and write BMP images, as well as manipulate individual pixels.
 * @version 0.2
 * @date 2023-11-06
 *
 * @copyright Copyright (c) 2023
 *
 */
#include "bmp.h"
#include <algorithm>
#include <fstream>
#include <iomanip>
#include <filesystem>
#include <stdexcept>
#include <iostream>
#include <cmath>

BMPImage::BMPImage(const std::string& filename)
{
    read(filename);
}

void BMPImage::read(const std::string& filename)
{
    std::ifstream file(filename, std::ios::binary);
    if (!file)
    {
        throw std::runtime_error("Unable to open file");
    }

    // Read the file header and info header
    file.read(reinterpret_cast<char*>(&header), sizeof(header));
    file.read(reinterpret_cast<char*>(&info_header), sizeof(info_header));

    if (info_header.bit_count != 24 && info_header.bit_count != 32)
    {
        throw std::runtime_error("Unsupported BMP bit depth");
    }

    // Resize the pixel data grid
    pixels.resize(info_header.height, std::vector<Pixel>(info_header.width));

    // Seek to the pixel data beginning
    file.seekg(header.offset, file.beg);

    // Read the pixel data
    int padding = (4 - (info_header.width * (info_header.bit_count / 8)) % 4) % 4;

    for (int y = 0; y < info_header.height; y++)
    {
        for (int x = 0; x < info_header.width; x++)
        {
            Pixel& pixel = pixels[y][x];

            file.read(reinterpret_cast<char*>(&pixel), info_header.bit_count / 8);

            if (info_header.bit_count == 24)
            {
                pixel.a = 255;
            }
        }
        // Skip padding
        file.seekg(padding, file.cur);
    }

    file.close();
}

void BMPImage::write(const std::string& filename)
{
#ifdef __cplusplus
#if __cplusplus >= 201703L  // Check for C++17 or later
    std::filesystem::path filePath(filename);
    std::string folderName = filePath.parent_path().string();
    // Check if the directory exists, and if not, create it
    if (!std::filesystem::exists(folderName))
    {
        if (!std::filesystem::create_directory(folderName))
        {
            throw std::runtime_error("Failed to create directory");
        }
    }
#endif
#endif

    std::ofstream file(filename, std::ios::binary);
    if (!file)
    {
        throw std::runtime_error("Unable to open file");
    }

    // Write the file header and info header
    file.write(reinterpret_cast<const char*>(&header), sizeof(header));
    file.write(reinterpret_cast<const char*>(&info_header), sizeof(info_header));

    int padding = (4 - (info_header.width * (info_header.bit_count / 8)) % 4) % 4;

    // Write the pixel data
    for (int y = 0; y < info_header.height; y++)
    {
        for (int x = 0; x < info_header.width; x++)
        {
            const Pixel& pixel = pixels[y][x];
            file.write(reinterpret_cast<const char*>(&pixel), info_header.bit_count / 8);
        }
        // Write padding
        uint8_t padValue = 0;
        file.write(reinterpret_cast<const char*>(&padValue), padding);
    }

    file.close();
}

void BMPImage::applyGammaCorrection(float gamma)
{
    if (gamma < 0)
    {
        throw std::runtime_error("Gamma value must be greater than 0");
    }
    // Iterate over all pixels
    for (int y = 0; y < info_header.height; y++)
    {
        for (int x = 0; x < info_header.width; x++)
        {
            // Reference to the current pixel
            Pixel& pixel = pixels[y][x];

            // Apply gamma correction formula to each channel
            pixel.r = std::pow(pixel.r / 255.0f, gamma) * 255;
            pixel.g = std::pow(pixel.g / 255.0f, gamma) * 255;
            pixel.b = std::pow(pixel.b / 255.0f, gamma) * 255;
        }
    }
}

void BMPImage::sharpen(double sharpness)
{
    // The Laplacian kernel used for sharpening
    // clang-format off
    const std::vector<std::vector<int>> kernel = {
        {  0, -1,  0 },
        { -1,  4, -1 },
        {  0, -1,  0 }};
    // clang-format on
    int edge = kernel.size() / 2;

    int newWidth = info_header.width + 2 * edge;
    int newHeight = info_header.height + 2 * edge;
    std::vector<std::vector<Pixel>> paddedPixels(newHeight, std::vector<Pixel>(newWidth));

    for (int y = 0; y < info_header.height; ++y)
    {
        for (int x = 0; x < info_header.width; ++x)
        {
            paddedPixels[y + edge][x + edge] = pixels[y][x];
        }
    }

    // Perform mirror padding
    // Top and bottom padding
    for (int x = 0; x < info_header.width; ++x)
    {
        // Top edge
        for (int y = 0; y < edge; ++y)
        {
            paddedPixels[edge - 1 - y][x + edge] = pixels[y][x];                                            // Mirror top
            paddedPixels[info_header.height + edge + y][x + edge] = pixels[info_header.height - 1 - y][x];  // Mirror bottom
        }
    }

    // Left and right padding
    for (int y = 0; y < newHeight; ++y)
    {
        // Left edge
        for (int x = 0; x < edge; ++x)
        {
            paddedPixels[y][edge - 1 - x] = paddedPixels[y][edge + x];                                          // Mirror left
            paddedPixels[y][info_header.width + edge + x] = paddedPixels[y][info_header.width + edge - 1 - x];  // Mirror right
        }
    }

    // Corners padding
    for (int i = 0; i < edge; ++i)
    {
        // Top-left corner
        for (int j = 0; j < edge; ++j)
        {
            paddedPixels[i][j] = paddedPixels[2 * edge - i][2 * edge - j];
        }
        // Top-right corner
        for (int j = 0; j < edge; ++j)
        {
            paddedPixels[i][newWidth - 1 - j] = paddedPixels[2 * edge - i][newWidth - 1 - 2 * edge + j];
        }
        // Bottom-left corner
        for (int j = 0; j < edge; ++j)
        {
            paddedPixels[newHeight - 1 - i][j] = paddedPixels[newHeight - 1 - 2 * edge + i][2 * edge - j];
        }
        // Bottom-right corner
        for (int j = 0; j < edge; ++j)
        {
            paddedPixels[newHeight - 1 - i][newWidth - 1 - j] = paddedPixels[newHeight - 1 - 2 * edge + i][newWidth - 1 - 2 * edge + j];
        }
    }

    // Iterate over each pixel (excluding the border pixels)
    for (int y = 0; y < info_header.height; ++y)
    {
        for (int x = 0; x < info_header.width; ++x)
        {
            int red = 0, green = 0, blue = 0;

            // Apply the kernel to each color channel
            for (int i = -edge; i <= edge; ++i)
            {
                for (int j = -edge; j <= edge; ++j)
                {
                    const Pixel& p = paddedPixels[y + edge + i][x + edge + j];
                    int weight = kernel[edge + i][edge + j];

                    red += p.r * weight;
                    green += p.g * weight;
                    blue += p.b * weight;
                }
            }

            // Combine the sharpened values with the original pixel values
            pixels[y][x].r = static_cast<uint8_t>(std::clamp(static_cast<int>(pixels[y][x].r + sharpness * red), 0, 255));
            pixels[y][x].g = static_cast<uint8_t>(std::clamp(static_cast<int>(pixels[y][x].g + sharpness * green), 0, 255));
            pixels[y][x].b = static_cast<uint8_t>(std::clamp(static_cast<int>(pixels[y][x].b + sharpness * blue), 0, 255));
        }
    }
}

void BMPImage::applyGaussianSmoothing(int kernelSize, float sigma)
{
    if (kernelSize % 2 == 0)
    {
        throw std::runtime_error("Kernel size must be odd");
    }
    // Create the Gaussian kernel
    std::vector<std::vector<float>> kernel(kernelSize, std::vector<float>(kernelSize));
    float sum = 0.0;
    int edge = kernelSize / 2;

    // Populate the Gaussian kernel and sum the elements for normalization
    for (int i = -edge; i <= edge; i++)
    {
        for (int j = -edge; j <= edge; j++)
        {
            kernel[i + edge][j + edge] = exp(-((i * i + j * j) / (2 * sigma * sigma)));
            sum += kernel[i + edge][j + edge];
        }
    }

    // Normalize the kernel to ensure the sum of all elements is 1
    for (int i = 0; i < kernelSize; ++i)
        for (int j = 0; j < kernelSize; ++j)
            kernel[i][j] /= sum;

    int newWidth = info_header.width + 2 * edge;
    int newHeight = info_header.height + 2 * edge;
    std::vector<std::vector<Pixel>> paddedPixels(newHeight, std::vector<Pixel>(newWidth));

    for (int y = 0; y < info_header.height; ++y)
    {
        for (int x = 0; x < info_header.width; ++x)
        {
            paddedPixels[y + edge][x + edge] = pixels[y][x];
        }
    }

    // Top and bottom padding
    for (int x = 0; x < info_header.width; ++x)
    {
        // Top edge
        for (int y = 0; y < edge; ++y)
        {
            paddedPixels[y][x + edge] = pixels[edge - y][x];                                                // Mirror top
            paddedPixels[info_header.height + edge + y][x + edge] = pixels[info_header.height - 2 - y][x];  // Mirror bottom
        }
    }

    // Left and right padding
    for (int y = 0; y < info_header.height; ++y)
    {
        for (int x = 0; x < edge; ++x)
        {
            paddedPixels[y + edge][x] = pixels[y][edge - x];                                              // Mirror left
            paddedPixels[y + edge][info_header.width + edge + x] = pixels[y][info_header.width - 2 - x];  // Mirror right
        }
    }

    // Corner padding - You need to mirror the corners based on the mirrored edges already set above
    // Top-left corner
    for (int y = 0; y < edge; ++y)
    {
        for (int x = 0; x < edge; ++x)
        {
            paddedPixels[y][x] = paddedPixels[y][2 * edge - x];
        }
    }
    // Top-right corner
    for (int y = 0; y < edge; ++y)
    {
        for (int x = info_header.width + edge; x < newWidth; ++x)
        {
            paddedPixels[y][x] = paddedPixels[y][2 * (info_header.width + edge) - x - 1];
        }
    }
    // Bottom-left corner
    for (int y = info_header.height + edge; y < newHeight; ++y)
    {
        for (int x = 0; x < edge; ++x)
        {
            paddedPixels[y][x] = paddedPixels[y][2 * edge - x];
        }
    }
    // Bottom-right corner
    for (int y = info_header.height + edge; y < newHeight; ++y)
    {
        for (int x = info_header.width + edge; x < newWidth; ++x)
        {
            paddedPixels[y][x] = paddedPixels[y][2 * (info_header.width + edge) - x - 1];
        }
    }

    // Apply the Gaussian kernel to each pixel (excluding the borders)
    for (int y = 0; y < info_header.height; ++y)
    {
        for (int x = 0; x < info_header.width; ++x)
        {
            float sumR = 0.0f, sumG = 0.0f, sumB = 0.0f;

            // Apply the kernel to each color channel
            for (int i = -edge; i <= edge; i++)
            {
                for (int j = -edge; j <= edge; j++)
                {
                    const Pixel& pixel = paddedPixels[y + edge + i][x + edge + j];
                    float weight = kernel[edge + i][edge + j];

                    sumR += pixel.r * weight;
                    sumG += pixel.g * weight;
                    sumB += pixel.b * weight;
                }
            }

            // Set the new pixel value
            pixels[y][x].r = static_cast<uint8_t>(std::clamp(static_cast<int>(sumR), 0, 255));
            pixels[y][x].g = static_cast<uint8_t>(std::clamp(static_cast<int>(sumG), 0, 255));
            pixels[y][x].b = static_cast<uint8_t>(std::clamp(static_cast<int>(sumB), 0, 255));
        }
    }
}

void BMPImage::printFileHeader() const
{
    int print_width = 20;
    std::cout << "----------------------------------------" << std::endl;
    std::cout << "File header:" << std::endl;
    std::cout << std::left << std::setw(print_width) << "type:" << std::string(reinterpret_cast<const char*>(&header.type), 2) << std::endl;
    std::cout << std::left << std::setw(print_width) << "size:" << header.size << std::endl;
    std::cout << std::left << std::setw(print_width) << "reserved1:" << header.reserved1 << std::endl;
    std::cout << std::left << std::setw(print_width) << "reserved2:" << header.reserved2 << std::endl;
    std::cout << std::left << std::setw(print_width) << "offset:" << header.offset << std::endl;
    std::cout << std::endl;
}

void BMPImage::printInfoHeader() const
{
    int print_width = 20;
    std::cout << "----------------------------------------" << std::endl;
    std::cout << "Info header:" << std::endl;
    std::cout << std::left << std::setw(print_width) << "size:" << info_header.size << std::endl;
    std::cout << std::left << std::setw(print_width) << "width:" << info_header.width << std::endl;
    std::cout << std::left << std::setw(print_width) << "height:" << info_header.height << std::endl;
    std::cout << std::left << std::setw(print_width) << "planes:" << info_header.planes << std::endl;
    std::cout << std::left << std::setw(print_width) << "bit_count:" << info_header.bit_count << std::endl;
    std::cout << std::left << std::setw(print_width) << "compression:" << info_header.compression << std::endl;
    std::cout << std::left << std::setw(print_width) << "image_size:" << info_header.image_size << std::endl;
    std::cout << std::left << std::setw(print_width) << "x_resolution:" << info_header.x_resolution << std::endl;
    std::cout << std::left << std::setw(print_width) << "y_resolution:" << info_header.y_resolution << std::endl;
    std::cout << std::left << std::setw(print_width) << "used_colors:" << info_header.used_colors << std::endl;
    std::cout << std::left << std::setw(print_width) << "important_colors:" << info_header.important_colors << std::endl;
    std::cout << std::endl;
}
