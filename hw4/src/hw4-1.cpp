/**
 * @file hw4-1.cpp
 * @brief
 * @version 0.1
 * @date 2023-12-18
 *
 */

#include <filesystem>
#include <iomanip>
#include <iostream>
#include <stdexcept>
#include <string>

#include "bmp.h"
#include "psnr.h"
#include "restore_cv.h"

int main(int argc, char* argv[])
{
    if (argc != 2)
    {
        std::cerr << "Usage: " << argv[0] << " <input_file>" << std::endl;
        return 1;
    }
    // if_search_param is true if argv[1] is 1, false otherwise.
    bool if_search_param = (std::stoi(argv[1]) == 1);

    if (if_search_param)
    {
        std::cout << "searching parameters..." << std::endl;
    }
    else
    {
        std::cout << "using default parameters..." << std::endl;
    }

    // Assign the input and output filenames and gamma value.
    std::filesystem::path input1_filename("input/input1.bmp");
    std::filesystem::path input1_ori_filename("input/input1_ori.bmp");
    std::filesystem::path input2_filename("input/input2.bmp");

    std::filesystem::path output1_demotion_filename("output/output1_demotion.bmp");
    std::filesystem::path output1_filename("output/output1.bmp");
    std::filesystem::path output2_demotionfilename("output/output2_demotion.bmp");
    std::filesystem::path output2_filename("output/output2.bmp");

    std::filesystem::path psf_motion_1_filename("psf/psf_motion_1.bmp");
    std::filesystem::path psf_oof_1_filename("psf/psf_oof_1.bmp");
    std::filesystem::path psf_motion_2_filename("psf/psf_motion_2.bmp");
    std::filesystem::path psf_oof_2_filename("psf/psf_oof_2.bmp");

    try
    {
        BMPImage input1_image(input1_filename);
        BMPImage input1_ori_image(input1_ori_filename);
        BMPImage input2_image(input2_filename);

        PSNR psnr(input1_ori_image, input1_image);
        std::cout << input1_filename << ", " << input1_ori_filename << std::endl;
        psnr.calPrint();

        int len = 30;
        double theta = 45;
        // double snr1 = 23;
        // int r = 1;
        // double snr2 = 5.0;

        double max_snr1 = 21.09;
        int max_r = 1;
        double max_snr2 = 26.71;
        double max_psnr = 0.0;

        if (if_search_param)
        {
            for (double snr1 = 21.0; snr1 <= 21.2; snr1 += 0.01)
            {
                for (int r = 1; r <= 1; r += 1)
                {
                    for (double snr2 = 26.65; snr2 <= 26.85; snr2 += 0.01)
                    {
                        process_motion_blur(input1_filename, output1_demotion_filename, psf_motion_1_filename, len, theta, snr1);
                        process_oof_blur(output1_demotion_filename, output1_filename, psf_oof_1_filename, r, snr2);
                        PSNR psnr(input1_ori_filename, output1_filename);
                        double psnr_val = psnr.calculatePSNR();
                        std::cout << std::fixed << std::setprecision(2);
                        std::cout << "snr1: " << std::setw(5) << snr1 << ", r: " << std::setw(2) << r << ", snr2: " << std::setw(5) << snr2
                                  << ", psnr: " << std::setw(10) << std::setprecision(6) << psnr_val << std::endl;

                        if (psnr_val > max_psnr)
                        {
                            max_snr1 = snr1;
                            max_r = r;
                            max_snr2 = snr2;
                            max_psnr = psnr_val;
                        }
                    }
                }
            }
        }

        std::cout << "max_snr1: " << max_snr1 << ", max_r: " << max_r << ", max_snr2: " << max_snr2 << ", max_psnr: " << max_psnr << std::endl;

        process_motion_blur(input1_filename, output1_demotion_filename, psf_motion_1_filename, len, theta, max_snr1);
        process_oof_blur(output1_demotion_filename, output1_filename, psf_oof_1_filename, max_r, max_snr2);

        process_motion_blur(input2_filename, output2_demotionfilename, psf_motion_2_filename, len, theta, max_snr1);
        process_oof_blur(output2_demotionfilename, output2_filename, psf_oof_2_filename, max_r, max_snr2);

        // PSNR psnr_demotion(output1_demotion_filename, input1_ori_filename);
        // std::cout << "output1_demotion.bmp, " << input1_ori_filename << std::endl;
        // psnr_demotion.calPrint();

        PSNR psnr_result(output1_filename, input1_ori_filename);
        std::cout << "output1.bmp, " << input1_ori_filename << std::endl;
        psnr_result.calPrint();
    }
    catch (const std::runtime_error& e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
