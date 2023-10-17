#include <iostream>
#include <stdexcept>
#include <string>
#include "bmp.h"

int main(int argc, char* argv[])
{
    // Check if the user provided the correct number of arguments.
    if (argc != 4)
    {
        std::cerr << "Usage: " << argv[0] << " <input_file> <output_file> <bit_depth>" << std::endl;
        return 1;
    }

    // Assign the input and output filenames.
    std::string input_filename(argv[1]);
    std::string output_filename(argv[2]);
    int bit_depth = std::stoi(argv[3]);

    try
    {
        BMPImage image(input_filename);
        image.printFileHeader();  // print the file header
        image.printInfoHeader();  // print the info header
        image.quantize(bit_depth);
        image.write(output_filename);
        image.printFileHeader();  // print the file header
        image.printInfoHeader();  // print the info header
    }
    catch (const std::runtime_error& e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    std::cout << "Processing completed successfully!" << std::endl;
    return 0;
}
