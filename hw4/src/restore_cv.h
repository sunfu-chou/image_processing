#ifndef RESTORE_CV_H
#define RESTORE_CV_H

#include <filesystem>

#include "opencv2/core/types.hpp"

namespace cv
{
class Mat;
}

void calcMotionPSF(cv::Mat& outputImg, cv::Size filterSize, const std::filesystem::path& output_path, int len, double theta);
void calcCirclePSF(cv::Mat& outputImg, cv::Size filterSize, const std::filesystem::path& output_path, int r);
void fftshift(const cv::Mat& inputImg, cv::Mat& outputImg);
void filter2DFreq(const cv::Mat& inputImg, cv::Mat& outputImg, const cv::Mat& H);
void calcWnrFilter(const cv::Mat& input_h_PSF, cv::Mat& output_G, double nsr);
void edgetaper(const cv::Mat& inputImg, cv::Mat& outputImg, double gamma = 5.0, double beta = 0.2);

void process_motion_blur(const std::filesystem::path& input_filename, const std::filesystem::path& output_filename,
                         const std::filesystem::path& output_psf_filename, int len, double theta, double snr);

void process_oof_blur(const std::filesystem::path& input_filename, const std::filesystem::path& output_filename,
                      const std::filesystem::path& output_psf_filename, int r, double snr);
#endif  // RESTORE_CV_H
