#ifndef PSNR_H
#define PSNR_H

#include <filesystem>
#include <tuple>

#include "bmp.h"
class PSNR
{
  public:
    PSNR(const BMPImage& original, const BMPImage& processed);
    PSNR(const std::filesystem::path& original_path, const std::filesystem::path& processed_path);

    std::tuple<double, double, double> calculateMSE();
    double calculatePSNR();

    void calPrint();

  private:
    const BMPImage original;
    const BMPImage processed;
};

#endif  // PSNR_H
