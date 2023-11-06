/**
 * @file hw2-3.cpp
 * @author sunfu-chou (sunfu-chou@gmail.com)
 * @brief This hw2-3
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
    if (argc != 5)
    {
        std::cerr << "Usage: " << argv[0] << " <input_file> <output_file> <kernel_size> <sigma>" << std::endl;
        return 1;
    }

    // Assign the input and output filenames, kernel size, and sigma.
    std::string input_filename(argv[1]);
    std::string output_filename(argv[2]);
    int kernel_size = std::stoi(argv[3]);  // convert string to int
    float sigma = std::stof(argv[4]);      // convert string to float

    try
    {
        BMPImage image(input_filename);  // read the input file
        // Apply Gaussian Smoothing to the image.
        image.applyGaussianSmoothing(kernel_size, sigma);
        image.write(output_filename);  // write to the output file
    }
    catch (const std::runtime_error& e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    std::cout << GREEN << "Gaussian smoothing completed successfully!" << RESET << std::endl;
    return 0;
}
