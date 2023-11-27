/**
 * @file bmp.h
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

#pragma once

#include <filesystem>
#include <stdint.h>
#include <vector>

#define RESET "\033[0m"
#define BLACK "\033[30m"
#define RED "\033[31m"
#define GREEN "\033[32m"
#define YELLOW "\033[33m"
#define BLUE "\033[34m"
#define MAGENTA "\033[35m"
#define CYAN "\033[36m"
#define WHITE "\033[37m"

#pragma pack(push, 1)

/**
 * @brief The BMPFileHeader struct represents the file header of a BMP image.
 * The BMPFileHeader struct contains the following fields:
 * - type: the file type, which is always 0x4D42 (BM) for BMP images
 * - size: the size of the BMP file in bytes
 * - reserved1: reserved field (unused)
 * - reserved2: reserved field (unused)
 * - offset: the offset from the beginning of the file to the beginning of the image data
 *
 * @note The BMPFileHeader struct is packed so that it is 14 bytes long.
 *
 */
struct BMPFileHeader
{
    uint16_t type;
    uint32_t size;
    uint16_t reserved1;
    uint16_t reserved2;
    uint32_t offset;
};

/**
 * @brief The BMPInfoHeader struct represents the information header of a BMP image.
 * The BMPInfoHeader struct contains the following fields:
 * - size: the size of the BMPInfoHeader struct in bytes
 * - width: the width of the image in pixels
 * - height: the height of the image in pixels
 * - planes: the number of planes in the image (always 1)
 * - bit_count: the number of bits per pixel
 * - compression: the type of compression used
 * - image_size: the size of the image in bytes
 * - x_resolution: the horizontal resolution of the image (in pixels per meter)
 * - y_resolution: the vertical resolution of the image (in pixels per meter)
 * - used_colors: the number of colors in the image (0 for full-color images)
 * - important_colors: the number of important colors in the image (0 when every color is important)
 *
 * @note The BMPInfoHeader struct is packed so that it is 40 bytes long.
 *
 */
struct BMPInfoHeader
{
    uint32_t size;
    int32_t width;
    int32_t height;
    uint16_t planes;
    uint16_t bit_count;
    uint32_t compression;
    uint32_t image_size;
    int32_t x_resolution;
    int32_t y_resolution;
    uint32_t used_colors;
    uint32_t important_colors;
};
#pragma pack(pop)

/**
 * @brief The Pixel struct represents a single pixel in a BMP image.
 * The Pixel struct contains the following fields:
 * - r: the red component of the pixel
 * - g: the green component of the pixel
 * - b: the blue component of the pixel
 * - a: the alpha (transparency) component of the pixel
 *
 */
struct Pixel
{
    uint8_t r, g, b, a;
};

/**
 * @brief A class representing a BMP (Bitmap) image with various operations for manipulation and I/O.
 *
 * This class provides the capability to read and write BMP image files, as well as perform
 * various operations on the image data such as flipping, quantization, and scaling.
 */
class BMPImage
{
  private:
    BMPFileHeader header;
    BMPInfoHeader info_header;
    std::vector<std::vector<Pixel>> pixels;

  public:
    /**
     * @brief Construct a new BMPImage object
     *
     * @param filename The name of the BMP file to read.
     */
    BMPImage(const std::filesystem::path& filename);

    /**
     * @brief Reads an image from a BMP file.
     *
     * @param filename The name of the BMP file to read.
     */
    void read(const std::filesystem::path& filename);

    /**
     * @brief Writes the image to a BMP file.
     *
     * @param filename The name of the BMP file to write.
     */
    void write(const std::filesystem::path& filename);

    /**
     * @brief Adjusts the white balance of the image using grep world method.
     * White balance adjustment can help correct color tints in an image
     * caused by different light sources.
     */
    void adjustWhiteBalance();

    /**
     * @brief Mirrors the image by adding padding to the edges.
     *
     * @param original_pixels The original image data.
     * @param edge The size of the edge padding to add.
     * @return The mirrored image data.
     */
    std::vector<std::vector<Pixel>> mirrorPadding(const std::vector<std::vector<Pixel>>& original_pixels, int edge);

    /**
     * @brief Mirrors the image by adding padding to the edges.
     *
     * @param edge The size of the edge padding to add.
     */
    void mirrorPadding(int edge);

    /**
     * @brief Sharpens the image by enhancing the edges.
     * This function applies a sharpening filter to the image, which enhances edges and fine details.
     * Higher values of sharpness lead to a more pronounced sharpening effect.
     *
     * @param sharpness A positive value indicating the strength of the sharpening effect.
     */
    void sharpen(double sharpness);

    /**
     * @brief Converts an RGB color to HSV color space.
     *
     * @param r The red component of the RGB color.
     * @param g The green component of the RGB color.
     * @param b The blue component of the RGB color.
     * @param h [out] The resulting hue component in the HSV color space.
     * @param s [out] The resulting saturation component in the HSV color space.
     * @param v [out] The resulting value component in the HSV color space.
     */
    void RGBtoHSV(double r, double g, double b, double& h, double& s, double& v);

    /**
     * @brief Converts an HSV color to RGB color space.
     *
     * @param h The hue component in the HSV color space.
     * @param s The saturation component in the HSV color space.
     * @param v The value component in the HSV color space.
     * @param r [out] The resulting red component in the RGB color space.
     * @param g [out] The resulting green component in the RGB color space.
     * @param b [out] The resulting blue component in the RGB color space.
     */
    void HSVtoRGB(double h, double s, double v, double& r, double& g, double& b);

    /**
     * @brief Converts an RGB color to HSI color space.
     *
     * @param r The red component of the RGB color.
     * @param g The green component of the RGB color.
     * @param b The blue component of the RGB color.
     * @param h [out] The resulting hue component in the HSI color space.
     * @param s [out] The resulting saturation component in the HSI color space.
     * @param i [out] The resulting intensity component in the HSI color space.
     */
    void RGBtoHSI(double r, double g, double b, double& h, double& s, double& i);

    /**
     * @brief Converts an HSI color to RGB color space.
     *
     * @param h The hue component in the HSI color space.
     * @param s The saturation component in the HSI color space.
     * @param i The intensity component in the HSI color space.
     * @param r [out] The resulting red component in the RGB color space.
     * @param g [out] The resulting green component in the RGB color space.
     * @param b [out] The resulting blue component in the RGB color space.
     */
    void HSItoRGB(double h, double s, double i, double& r, double& g, double& b);

    /**
     * @brief Adjusts the saturation of the image.
     *
     * @param saturation_factor The factor by which to adjust the saturation.
     */
    void adjustSaturation(double saturation_factor);

    /**
     * @brief Adjusts the intensity (brightness) of the image.
     *
     * @param intensity_factor The factor by which to adjust the intensity.
     */
    void adjustIntensity(double intensity_factor);

    /**
     * @brief Applies gamma correction to the BMP image.
     * Gamma correction adjusts the luminance of the image. This is a non-linear operation
     * that is used to encode and decode luminance or tristimulus values in video or still image systems.
     * A gamma value < 1 will lighten the image, while a gamma value > 1 will darken it.
     *
     * @param gamma The gamma correction value. Typically, it is in the range (0, ∞).
     */
    void applyGammaCorrection(float gamma);

    /**
     * @brief Adjusts the contrast of the image.
     *
     * @param contrastFactor The factor by which to adjust the contrast.
     */
    void adjustContrast(double contrastFactor);

    /**
     * @brief Adjusts the hue of the image.
     *
     * @param hue_adjustment The amount by which to adjust the hue.
     */
    void adjustHue(double hue_adjustment);
};
