/**
 * @file bmp.cpp
 * @author sunfu-chou (sunfu-chou@gmail.com)
 * @brief This file contains the declarations of the BMPFileHeader, BMPInfoHeader, Pixel, and BMPImage classes.
 * The BMPFileHeader and BMPInfoHeader classes represent the file and information headers of a BMP image, respectively.
 * The Pixel class represents a single pixel in a BMP image, and the BMPImage class represents the entire BMP image.
 * This file provides the necessary functionality to read and write BMP images, as well as manipulate individual pixels.
 * @version 0.3
 * @date 2023-11-24
 *
 * @copyright Copyright (c) 2023
 *
 */
#include "bmp.h"
#include <algorithm>
#include <array>
#include <cmath>
#include <filesystem>
#include <fstream>
#include <stdexcept>
#include <string>

BMPImage::BMPImage(const std::filesystem::path& filename)
{
    read(filename);
}

void BMPImage::read(const std::filesystem::path& filename)
{
    std::ifstream file(filename, std::ios::binary);
    if (!file)
    {
        throw std::filesystem::filesystem_error("Unable to open file", filename, std::make_error_code(std::io_errc::stream));
    }

    // Read the file header and info header
    file.read(reinterpret_cast<char*>(&header), sizeof(header));
    file.read(reinterpret_cast<char*>(&info_header), sizeof(info_header));

    if (info_header.bit_count != 24 && info_header.bit_count != 32)
    {
        throw std::ios_base::failure("Unsupported bit count: " + std::to_string(info_header.bit_count));
    }

    // Resize the pixel data grid
    pixels.resize(info_header.height, std::vector<Pixel>(info_header.width));

    // Seek to the pixel data beginning
    file.seekg(header.offset, file.beg);

    // Read the pixel data
    int padding = (4 - (info_header.width * (info_header.bit_count / 8)) % 4) % 4;

    for (auto& row : pixels)
    {
        for (auto& pixel : row)
        {
            file.read(reinterpret_cast<char*>(&pixel), info_header.bit_count / 8);

            if (info_header.bit_count == 24)
            {
                pixel.a = 255;
            }
        }
        // Skip padding
        file.ignore(padding);
    }
}

void BMPImage::write(const std::filesystem::path& filename)
{
    std::ofstream file(filename, std::ios::binary);
    if (!file)
    {
        throw std::filesystem::filesystem_error("Unable to open file", filename, std::make_error_code(std::io_errc::stream));
    }

    // Write the file header and info header
    file.write(reinterpret_cast<const char*>(&header), sizeof(header));
    file.write(reinterpret_cast<const char*>(&info_header), sizeof(info_header));

    int padding = (4 - (info_header.width * (info_header.bit_count / 8)) % 4) % 4;
    std::vector<char> pad(padding, 0);

    // Write the pixel data
    for (const auto& row : pixels)
    {
        for (const auto& pixel : row)
        {
            file.write(reinterpret_cast<const char*>(&pixel), info_header.bit_count / 8);
        }
        if (padding > 0)
        {
            file.write(pad.data(), padding);
        }
    }
}

int BMPImage::getWidth() const
{
    return info_header.width;
}

int BMPImage::getHeight() const
{
    return info_header.height;
}

Pixel BMPImage::getPixel(int x, int y) const
{
    if (x < 0 || x >= info_header.width || y < 0 || y >= info_header.height)
    {
        throw std::out_of_range("Pixel coordinates out of range");
    }

    return pixels[y][x];
}

void BMPImage::setPixel(int x, int y, const Pixel& pixel)
{
    if (x < 0 || x >= info_header.width || y < 0 || y >= info_header.height)
    {
        throw std::out_of_range("Pixel coordinates out of range");
    }

    pixels[y][x] = pixel;
}

void BMPImage::adjustWhiteBalance()
{
    // Calculate average values
    long long total_r = 0, total_g = 0, total_b = 0;

    for (const auto& row : pixels)
    {
        for (const auto& [r, g, b, a] : row)
        {
            total_r += r;
            total_g += g;
            total_b += b;
        }
    }

    int pixel_count = info_header.width * info_header.height;

    double avg_r = static_cast<double>(total_r) / pixel_count;
    double avg_g = static_cast<double>(total_g) / pixel_count;
    double avg_b = static_cast<double>(total_b) / pixel_count;

    // Compute adjustment factors
    double avg_grey = (avg_r + avg_g + avg_b) / 3;
    double r_factor = avg_grey / avg_r;
    double g_factor = avg_grey / avg_g;
    double b_factor = avg_grey / avg_b;

    // Adjust pixels
    for (auto& row : pixels)
    {
        for (auto& [r, g, b, a] : row)
        {
            r = std::clamp(static_cast<int>(r * r_factor), 0, 255);
            g = std::clamp(static_cast<int>(g * g_factor), 0, 255);
            b = std::clamp(static_cast<int>(b * b_factor), 0, 255);
        }
    }
}

std::vector<std::vector<Pixel>> BMPImage::mirrorPadding(const std::vector<std::vector<Pixel>>& original_pixels, int edge)
{
    int new_width = info_header.width + 2 * edge;
    int new_height = info_header.height + 2 * edge;
    std::vector<std::vector<Pixel>> padded_pixels(new_height, std::vector<Pixel>(new_width));

    for (int y = 0; y < info_header.height; ++y)
    {
        std::copy(original_pixels[y].begin(), original_pixels[y].end(), padded_pixels[y + edge].begin() + edge);
    }

    for (int y = 0; y < edge; ++y)
    {
        std::copy_n(padded_pixels[2 * edge - y].begin(), new_width, padded_pixels[y].begin());
        std::copy_n(padded_pixels[new_height - 1 - 2 * edge + y].begin(), new_width, padded_pixels[new_height - 1 - y].begin());
    }

    for (int y = 0; y < new_height; ++y)
    {
        std::reverse_copy(padded_pixels[y].begin() + edge, padded_pixels[y].begin() + 2 * edge, padded_pixels[y].begin());
        std::reverse_copy(padded_pixels[y].end() - 2 * edge, padded_pixels[y].end() - edge, padded_pixels[y].end() - edge);
    }
    return padded_pixels;
}

void BMPImage::mirrorPadding(int edge)
{
    pixels = mirrorPadding(pixels, edge);
    info_header.width += 2 * edge;
    info_header.height += 2 * edge;
}

void BMPImage::sharpen(double sharpness)
{
    // The Laplacian kernel used for sharpening
    // clang-format off
    const std::array<std::array<int, 3>, 3> kernel = {{
        {  0, -1,  0 },
        { -1,  4, -1 },
        {  0, -1,  0 }}};
    // clang-format on
    int edge = kernel.size() / 2;

    std::vector<std::vector<Pixel>> padded_pixels = mirrorPadding(pixels, edge);

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
                    const auto& [r, g, b, a] = padded_pixels[y + edge + i][x + edge + j];
                    int weight = kernel[edge + i][edge + j];

                    red += r * weight;
                    green += g * weight;
                    blue += b * weight;
                }
            }

            // Combine the sharpened values with the original pixel values
            pixels[y][x].r = static_cast<uint8_t>(std::clamp(static_cast<int>(pixels[y][x].r + sharpness * red), 0, 255));
            pixels[y][x].g = static_cast<uint8_t>(std::clamp(static_cast<int>(pixels[y][x].g + sharpness * green), 0, 255));
            pixels[y][x].b = static_cast<uint8_t>(std::clamp(static_cast<int>(pixels[y][x].b + sharpness * blue), 0, 255));
        }
    }
}

void BMPImage::RGBtoHSV(double r, double g, double b, double& h, double& s, double& v)
{
    r = r / 255.0;
    g = g / 255.0;
    b = b / 255.0;
    auto [c_min, c_max] = std::minmax({ r, g, b });
    double c_delta = c_max - c_min;

    h = 0.0;
    if (c_delta > 0.0)
    {
        if (c_max == r)
        {
            h = std::fmod((60 * ((g - b) / c_delta) + 360), 360);
        }
        else if (c_max == g)
        {
            h = std::fmod((60 * ((b - r) / c_delta) + 120), 360);
        }
        else if (c_max == b)
        {
            h = std::fmod((60 * ((r - g) / c_delta) + 240), 360);
        }
    }

    s = (c_max == 0.0) ? 0.0 : (c_delta / c_max);

    v = c_max;
}

void BMPImage::HSVtoRGB(double h, double s, double v, double& r, double& g, double& b)
{
    double c = v * s;
    double x = c * (1 - std::fabs(fmod(h / 60.0, 2) - 1));
    double m = v - c;

    r = g = b = 0;

    if (h >= 0 && h < 60)
    {
        r = c, g = x, b = 0;
    }
    else if (h >= 60 && h < 120)
    {
        r = x, g = c, b = 0;
    }
    else if (h >= 120 && h < 180)
    {
        r = 0, g = c, b = x;
    }
    else if (h >= 180 && h < 240)
    {
        r = 0, g = x, b = c;
    }
    else if (h >= 240 && h < 300)
    {
        r = x, g = 0, b = c;
    }
    else if (h >= 300 && h <= 360)
    {
        r = c, g = 0, b = x;
    }

    r = (r + m) * 255;
    g = (g + m) * 255;
    b = (b + m) * 255;

    r = std::clamp(r, 0.0, 255.0);
    g = std::clamp(g, 0.0, 255.0);
    b = std::clamp(b, 0.0, 255.0);
}

void BMPImage::RGBtoHSI(double r, double g, double b, double& h, double& s, double& i)
{
    r /= 255.0;
    g /= 255.0;
    b /= 255.0;

    double num = 0.5 * ((r - g) + (r - b));
    double den = std::sqrt((r - g) * (r - g) + (r - b) * (g - b));
    h = (den == 0) ? 0 : std::acos(num / den);

    if (b > g)
    {
        h = 2 * M_PI - h;
    }

    h = h * 180 / M_PI;

    double min_rgb = std::min({ r, g, b });
    s = 1 - 3 * min_rgb / (r + g + b);
    s = (s < 0) ? 0 : s;

    i = (r + g + b) / 3;
}

void BMPImage::HSItoRGB(double h, double s, double i, double& r, double& g, double& b)
{
    h = h * M_PI / 180;

    if (h < 2 * M_PI / 3)
    {
        b = i * (1 - s);
        r = i * (1 + s * std::cos(h) / std::cos(M_PI / 3 - h));
        g = 3 * i - (r + b);
    }
    else if (h < 4 * M_PI / 3)
    {
        h -= 2 * M_PI / 3;
        r = i * (1 - s);
        g = i * (1 + s * std::cos(h) / std::cos(M_PI / 3 - h));
        b = 3 * i - (r + g);
    }
    else
    {
        h -= 4 * M_PI / 3;
        g = i * (1 - s);
        b = i * (1 + s * std::cos(h) / std::cos(M_PI / 3 - h));
        r = 3 * i - (g + b);
    }

    r *= 255.0;
    g *= 255.0;
    b *= 255.0;

    r = std::clamp(r, 0.0, 255.0);
    g = std::clamp(g, 0.0, 255.0);
    b = std::clamp(b, 0.0, 255.0);
}

void BMPImage::adjustSaturation(double saturation_factor)
{
    for (auto& row : pixels)
    {
        for (auto& pixel : row)
        {
            double h, s, i;
            RGBtoHSI(pixel.r / 255.0, pixel.g / 255.0, pixel.b / 255.0, h, s, i);

            s *= saturation_factor;
            s = std::clamp(s, 0.0, 1.0);
            double r, g, b;
            HSItoRGB(h, s, i, r, g, b);

            pixel.r = static_cast<uint8_t>(std::round(r * 255));
            pixel.g = static_cast<uint8_t>(std::round(g * 255));
            pixel.b = static_cast<uint8_t>(std::round(b * 255));
        }
    }
}

void BMPImage::adjustHue(double hue_adjustment)
{
    for (auto& row : pixels)
    {
        for (auto& pixel : row)
        {
            double h, s, i;
            RGBtoHSI(pixel.r / 255.0, pixel.g / 255.0, pixel.b / 255.0, h, s, i);

            h += hue_adjustment;
            if (h > 360)
                h -= 360;
            else if (h < 0)
                h += 360;

            double r, g, b;
            HSItoRGB(h, s, i, r, g, b);

            pixel.r = static_cast<uint8_t>(std::round(r * 255));
            pixel.g = static_cast<uint8_t>(std::round(g * 255));
            pixel.b = static_cast<uint8_t>(std::round(b * 255));
        }
    }
}

void BMPImage::adjustIntensity(double intensity_factor)
{
    for (auto& row : pixels)
    {
        for (auto& pixel : row)
        {
            double h, s, i;
            RGBtoHSI(pixel.r / 255.0, pixel.g / 255.0, pixel.b / 255.0, h, s, i);

            i *= intensity_factor;
            i = std::clamp(i, 0.0, 1.0);

            double r, g, b;
            HSItoRGB(h, s, i, r, g, b);

            pixel.r = static_cast<uint8_t>(std::round(r * 255));
            pixel.g = static_cast<uint8_t>(std::round(g * 255));
            pixel.b = static_cast<uint8_t>(std::round(b * 255));
        }
    }
}

void BMPImage::applyGammaCorrection(float gamma)
{
    if (gamma < 0)
    {
        throw std::runtime_error("Gamma value must be greater than 0");
    }

    // Iterate over all rows of pixels
    for (auto& row : pixels)
    {
        // Iterate over each pixel in the row
        for (auto& pixel : row)
        {
            // Apply gamma correction formula to each channel
            pixel.r = std::pow(pixel.r / 255.0f, gamma) * 255;
            pixel.g = std::pow(pixel.g / 255.0f, gamma) * 255;
            pixel.b = std::pow(pixel.b / 255.0f, gamma) * 255;
        }
    }
}

void BMPImage::adjustContrast(double contrastFactor)
{
    for (auto& row : pixels)
    {
        for (auto& pixel : row)
        {
            pixel.r = std::clamp(128 + static_cast<int>(contrastFactor * (pixel.r - 128)), 0, 255);
            pixel.g = std::clamp(128 + static_cast<int>(contrastFactor * (pixel.g - 128)), 0, 255);
            pixel.b = std::clamp(128 + static_cast<int>(contrastFactor * (pixel.b - 128)), 0, 255);
        }
    }
}
