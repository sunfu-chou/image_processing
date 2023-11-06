/**
 * @file hw2-1.cpp
 * @author sunfu-chou (sunfu-chou@gmail.com)
 * @brief This hw2-1
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
        // Update usage instruction to include gamma value
        std::cerr << "Usage: " << argv[0] << " <input_file> <output_file> <gamma_value>" << std::endl;
        return 1;
    }

    // Assign the input and output filenames and gamma value.
    std::string input_filename(argv[1]);
    std::string output_filename(argv[2]);
    float gamma = std::stof(argv[3]);  // Convert the third argument to float for gamma

    try
    {
        BMPImage image(input_filename);  // Load the input BMP image
        // Apply gamma correction with the provided gamma value
        image.applyGammaCorrection(gamma);
        image.write(output_filename);  // Save the modified image to the output file
    }
    catch (const std::runtime_error& e)
    {
        std::cerr << "Error: " << e.what() << std::endl;  // Report any errors that occurred
        return 1;
    }

    std::cout << GREEN << "Gamma correction completed successfully!" << RESET << std::endl;  // Confirm successful processing
    return 0;
}
