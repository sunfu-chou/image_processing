/**
 * @file hw2-2.cpp
 * @author sunfu-chou (sunfu-chou@gmail.com)
 * @brief This hw2-2
 * @version 0.1
 * @date 2023-11-06
 *
 * @copyright Copyright (c) 2023
 *
 */
#include <iostream>
#include <stdexcept>
#include <string>
#include "bmp.h"

int main(int argc, char* argv[])
{
    // Check if the user provided the correct number of arguments.
    if (argc != 4)
    {
        std::cerr << "Usage: " << argv[0] << " <input_file> <output_file> <sharpen_intensity>" << std::endl;
        return 1;
    }

    // Assign the input and output filenames and the sharpen intensity.
    std::string input_filename(argv[1]);
    std::string output_filename(argv[2]);
    float sharpen_intensity = std::stof(argv[3]);

    try
    {
        BMPImage image(input_filename);
        // Apply sharpening with the provided intensity
        image.sharpen(sharpen_intensity);
        // Saving the sharpened image
        image.write(output_filename);
    }
    catch (const std::runtime_error& e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    std::cout << GREEN << "Sharpening completed successfully with intensity " << sharpen_intensity << "!" << RESET << std::endl;
    return 0;
}
