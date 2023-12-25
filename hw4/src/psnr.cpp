
#include "psnr.h"

#include <cmath>
#include <filesystem>
#include <iostream>
#include <stdexcept>

#include "bmp.h"

// Constructor
PSNR::PSNR(const BMPImage& original, const BMPImage& processed) : original(original), processed(processed)
{
}

PSNR::PSNR(const std::filesystem::path& original_path, const std::filesystem::path& processed_path)
  : original(original_path), processed(processed_path)
{
}
// Calculate Mean Squared Error (MSE)
std::tuple<double, double, double> PSNR::calculateMSE()
{
    if (original.getWidth() != processed.getWidth() || original.getHeight() != processed.getHeight())
    {
        throw std::invalid_argument("Images must be of the same size");
    }
    double mseR = 0.0, mseG = 0.0, mseB = 0.0;
    int width = original.getWidth();
    int height = original.getHeight();

    for (int y = 0; y < height; ++y)
    {
        for (int x = 0; x < width; ++x)
        {
            Pixel origPixel = original.getPixel(x, y);
            Pixel procPixel = processed.getPixel(x, y);

            mseR += std::pow(origPixel.r - procPixel.r, 2.0);
            mseG += std::pow(origPixel.g - procPixel.g, 2.0);
            mseB += std::pow(origPixel.b - procPixel.b, 2.0);
        }
    }

    mseR /= (width * height) * 3.0;
    mseG /= (width * height) * 3.0;
    mseB /= (width * height) * 3.0;

    return std::make_tuple(mseR, mseG, mseB);
}

// Calculate Peak Signal-to-Noise Ratio (PSNR) using the MSE of the channel with the highest error
double PSNR::calculatePSNR()
{
    double psnr = 0.0;
    auto [mseR, mseG, mseB] = calculateMSE();
    double mse = mseR + mseG + mseB;
    psnr = 10.0 * log10(255.0 * 255.0 / mse);

    return psnr;
}

void PSNR::calPrint()
{
    auto [mseR, mseG, mseB] = calculateMSE();
    std::cout << "MSE (R): " << mseR << std::endl;
    std::cout << "MSE (G): " << mseG << std::endl;
    std::cout << "MSE (B): " << mseB << std::endl;
    std::cout << "PSNR: " << calculatePSNR() << " dB" << std::endl;
}
