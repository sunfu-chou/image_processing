#include "bmp.h"
#include <algorithm>
#include <fstream>
#include <iomanip>
#include <filesystem>
#include <stdexcept>
#include <iostream>

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

void BMPImage::flipHorizontal()
{
    for (int i = 0; i < info_header.height; i++)
    {
        std::reverse(pixels[i].begin(), pixels[i].end());
    }
}

void BMPImage::quantize(const int bit_depth)
{
    if (bit_depth <= 0 || bit_depth > 8)
    {
        throw std::runtime_error("Invalid bit depth for quantization.");
    }

    // Compute the factor to scale down and then scale up the pixel value.
    int scale_down_factor = (1 << (8 - bit_depth));
    int scale_up_factor = (1 << (8 - bit_depth)) - 1;

    // print the scale factors
    std::cout << "scale_down_factor: " << scale_down_factor << std::endl;
    std::cout << "scale_up_factor: " << scale_up_factor << std::endl;

    for (int y = 0; y < info_header.height; y++)
    {
        for (int x = 0; x < info_header.width; x++)
        {
            pixels[y][x].r = (pixels[y][x].r / scale_down_factor) * scale_up_factor;
            pixels[y][x].g = (pixels[y][x].g / scale_down_factor) * scale_up_factor;
            pixels[y][x].b = (pixels[y][x].b / scale_down_factor) * scale_up_factor;
            if (info_header.bit_count == 32)
            {
                pixels[y][x].a = (pixels[y][x].a / scale_down_factor) * scale_up_factor;
            }
        }
    }
}

void BMPImage::scale(float rate, const bool is_upscaling, const bool crop)
{
    if (!is_upscaling)  // if it's down-scaling
    {
        rate = 1.0 / rate;  // Take the reciprocal of the rate
    }

    if (rate <= 0)
    {
        throw std::runtime_error("Invalid scale rate. Must be positive.");
    }

    // Calculate the new dimensions of the image.
    int new_width = static_cast<int>(info_header.width * rate);
    int new_height = static_cast<int>(info_header.height * rate);

    // Check if cropping is needed
    if (crop)
    {
        // Adjust the width to be a multiple of 4
        new_width = (new_width / 4) * 4;
    }

    // Create a new pixel data grid with the new dimensions.
    std::vector<std::vector<Pixel>> new_pixels(new_height, std::vector<Pixel>(new_width));

    for (int y = 0; y < new_height; y++)
    {
        for (int x = 0; x < new_width; x++)
        {
            // Calculate the coordinates in the original image.
            float gx = x / rate;
            float gy = y / rate;

            // Get surrounding coordinates.
            int gxi = static_cast<int>(gx);
            int gyi = static_cast<int>(gy);

            // Ensure the coordinates are within bounds.
            int gxi1 = std::min(gxi + 1, info_header.width - 1);
            int gyi1 = std::min(gyi + 1, info_header.height - 1);

            // Get fractions to use in interpolation.
            float fx = gx - gxi;
            float fy = gy - gyi;
            float fx1 = 1.0f - fx;
            float fy1 = 1.0f - fy;

            // Fetch the four surrounding pixels.
            Pixel& p1 = pixels[gyi][gxi];
            Pixel& p2 = pixels[gyi][gxi1];
            Pixel& p3 = pixels[gyi1][gxi];
            Pixel& p4 = pixels[gyi1][gxi1];

            // Perform bilinear interpolation.
            new_pixels[y][x].r = static_cast<uint8_t>(p1.r * fx1 * fy1 + p2.r * fx * fy1 + p3.r * fx1 * fy + p4.r * fx * fy);
            new_pixels[y][x].g = static_cast<uint8_t>(p1.g * fx1 * fy1 + p2.g * fx * fy1 + p3.g * fx1 * fy + p4.g * fx * fy);
            new_pixels[y][x].b = static_cast<uint8_t>(p1.b * fx1 * fy1 + p2.b * fx * fy1 + p3.b * fx1 * fy + p4.b * fx * fy);
            new_pixels[y][x].a = static_cast<uint8_t>(p1.a * fx1 * fy1 + p2.a * fx * fy1 + p3.a * fx1 * fy + p4.a * fx * fy);
        }
    }

    // Replace the original pixel data with the new data, and update the header information.
    pixels = new_pixels;
    info_header.width = new_width;
    info_header.height = new_height;
    // header
    header.size = info_header.width * info_header.height * (info_header.bit_count / 8) + header.offset;
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

// int BMPImage::getWidth() const
// {
//     return info_header.width;
// }

// int BMPImage::getHeight() const
// {
//     return info_header.height;
// }

// const BMPFileHeader &BMPImage::getHeader() const
// {
//     return header;
// }

// const BMPInfoHeader &BMPImage::getInfoHeader() const
// {
//     return info_header;
// }

// const std::vector<std::vector<Pixel>> &BMPImage::getPixels() const
// {
//     return pixels;
// }
