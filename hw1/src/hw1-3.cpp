/**
 * @file hw1-3.cpp
 * @author sunfu-chou (sunfu-chou@gmail.com)
 * @brief This hw1-3
 * @version 0.1
 * @date 2023-10-18
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
    if (argc != 6)
    {
        std::cerr << "Usage: " << argv[0] << " <input_file> <output_file> <scale_rate> <is_upscaling> <crop>" << std::endl;
        return 1;
    }

    // Assign the input and output filenames and scaling rate.
    std::string input_filename(argv[1]);
    std::string output_filename(argv[2]);
    float scale_rate = std::stof(argv[3]);   // convert string to float
    bool is_upscaling = std::stoi(argv[4]);  // get scaling direction as bool
    bool crop = std::stoi(argv[5]);

    try
    {
        BMPImage image(input_filename);               // read the input file
        image.printFileHeader();                      // print the file header
        image.printInfoHeader();                      // print the info header
        image.scale(scale_rate, is_upscaling, crop);  // scale the image
        image.write(output_filename);                 // write to the output file
        image.printFileHeader();                      // print the file header
        image.printInfoHeader();                      // print the info header
    }
    catch (const std::runtime_error& e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    std::cout << "Processing completed successfully!" << std::endl;
    return 0;
}
