/**
 * @file hw3-1.cpp
 * @author sunfu-chou (sunfu-chou@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2023-11-17
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#include "bmp.h"
#include <filesystem>
#include <iostream>
#include <stdexcept>

int main(int argc, char* argv[])
{
    // Check if the user provided the correct number of arguments.
    if (argc != 3)
    {
        // Update usage instruction to include gamma value
        std::cerr << "Usage: " << argv[0] << " <input_file> <output_file>" << std::endl;
        return 1;
    }

    // Assign the input and output filenames and gamma value.
    std::filesystem::path input_filename(argv[1]);
    std::filesystem::path output_filename(argv[2]);

    try
    {
        BMPImage image(input_filename);  // Load the input BMP image
        image.adjustWhiteBalance();      // Adjust the white balance of the image
        image.write(output_filename);    // Save the modified image to the output file
    }
    catch (const std::runtime_error& e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    std::cout << GREEN << "White balance adjustment completed successfully!" << RESET << std::endl;
    return 0;
}
