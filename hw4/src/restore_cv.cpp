#include "restore_cv.h"

#include <math.h>

#include <algorithm>
#include <vector>

#include "opencv2/core.hpp"
#include "opencv2/core/base.hpp"
#include "opencv2/core/cvdef.h"
#include "opencv2/core/fast_math.hpp"
#include "opencv2/core/hal/interface.h"
#include "opencv2/core/mat.hpp"
#include "opencv2/core/mat.inl.hpp"
#include "opencv2/core/matx.hpp"
#include "opencv2/core/operations.hpp"
#include "opencv2/core/traits.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/imgproc.hpp"

void calcMotionPSF(cv::Mat& outputImg, cv::Size filterSize, const std::filesystem::path& output_path, int len, double theta)
{
    cv::Mat h(filterSize, CV_32F, cv::Scalar(0));
    cv::Point point(filterSize.width / 2, filterSize.height / 2);
    cv::ellipse(h, point, cv::Size(0, cvRound(float(len) / 2.0)), 90.0 - theta, 0, 360, cv::Scalar(255), cv::FILLED);
    cv::Scalar summa = cv::sum(h);
    outputImg = h / summa[0];
    h.convertTo(h, CV_8U);
    cv::imwrite(output_path, h);
}

void calcCirclePSF(cv::Mat& outputImg, cv::Size filterSize, const std::filesystem::path& output_path, int r)
{
    cv::Mat h(filterSize, CV_32F, cv::Scalar(0));
    cv::Point point(filterSize.width / 2, filterSize.height / 2);
    cv::circle(h, point, r, 255, -1, 8);
    cv::Scalar summa = sum(h);
    outputImg = h / summa[0];
    h.convertTo(h, CV_8U);
    cv::imwrite(output_path, h);
}

void calcWnrFilter(const cv::Mat& input_h_PSF, cv::Mat& output_G, double nsr)
{
    cv::Mat h_PSF_shifted;
    fftshift(input_h_PSF, h_PSF_shifted);
    cv::Mat planes[2] = { cv::Mat_<float>(h_PSF_shifted.clone()), cv::Mat::zeros(h_PSF_shifted.size(), CV_32F) };
    cv::Mat complexI;
    cv::merge(planes, 2, complexI);
    cv::dft(complexI, complexI);
    cv::split(complexI, planes);
    cv::Mat denom;
    cv::pow(cv::abs(planes[0]), 2, denom);
    denom += nsr;
    cv::divide(planes[0], denom, output_G);
}

void edgetaper(const cv::Mat& inputImg, cv::Mat& outputImg, double gamma, double beta)
{
    int Nx = inputImg.cols;
    int Ny = inputImg.rows;
    cv::Mat w1(1, Nx, CV_32F, cv::Scalar(0));
    cv::Mat w2(Ny, 1, CV_32F, cv::Scalar(0));

    float* p1 = w1.ptr<float>(0);
    float* p2 = w2.ptr<float>(0);
    float dx = float(2.0 * CV_PI / Nx);
    float x = float(-CV_PI);
    for (int i = 0; i < Nx; i++)
    {
        p1[i] = float(0.5 * (tanh((x + gamma / 2) / beta) - tanh((x - gamma / 2) / beta)));
        x += dx;
    }
    float dy = float(2.0 * CV_PI / Ny);
    float y = float(-CV_PI);
    for (int i = 0; i < Ny; i++)
    {
        p2[i] = float(0.5 * (tanh((y + gamma / 2) / beta) - tanh((y - gamma / 2) / beta)));
        y += dy;
    }
    cv::Mat w = w2 * w1;
    cv::multiply(inputImg, w, outputImg);
}

void fftshift(const cv::Mat& inputImg, cv::Mat& outputImg)
{
    outputImg = inputImg.clone();
    int cx = outputImg.cols / 2;
    int cy = outputImg.rows / 2;
    cv::Mat q0(outputImg, cv::Rect(0, 0, cx, cy));
    cv::Mat q1(outputImg, cv::Rect(cx, 0, cx, cy));
    cv::Mat q2(outputImg, cv::Rect(0, cy, cx, cy));
    cv::Mat q3(outputImg, cv::Rect(cx, cy, cx, cy));
    cv::Mat tmp;
    q0.copyTo(tmp);
    q3.copyTo(q0);
    tmp.copyTo(q3);
    q1.copyTo(tmp);
    q2.copyTo(q1);
    tmp.copyTo(q2);
}

void filter2DFreq(const cv::Mat& inputImg, cv::Mat& outputImg, const cv::Mat& H)
{
    cv::Mat planes[2] = { cv::Mat_<float>(inputImg.clone()), cv::Mat::zeros(inputImg.size(), CV_32F) };
    cv::Mat complexI;
    cv::merge(planes, 2, complexI);
    cv::dft(complexI, complexI, cv::DFT_SCALE);

    cv::Mat planesH[2] = { cv::Mat_<float>(H.clone()), cv::Mat::zeros(H.size(), CV_32F) };
    cv::Mat complexH;
    cv::merge(planesH, 2, complexH);
    cv::Mat complexIH;
    cv::mulSpectrums(complexI, complexH, complexIH, 0);

    cv::idft(complexIH, complexIH);
    cv::split(complexIH, planes);
    outputImg = planes[0];
}

void process_motion_blur(const std::filesystem::path& input_filename, const std::filesystem::path& output_filename,
                         const std::filesystem::path& output_psf_filename, int len, double theta, double snr)
{
    cv::Mat imgIn;
    imgIn = cv::imread(input_filename, cv::IMREAD_COLOR);

    std::vector<cv::Mat> channels(3);
    cv::split(imgIn, channels);

    std::vector<cv::Mat> processedChannels;
    for (int i = 0; i < 3; i++)
    {
        cv::Mat channel = channels[i].clone();
        cv::Mat processedChannel;

        cv::Rect roi = cv::Rect(0, 0, channel.cols & -2, channel.rows & -2);

        cv::Mat Hw, h;
        calcMotionPSF(h, roi.size(), output_psf_filename, len, theta);
        calcWnrFilter(h, Hw, 1.0 / double(snr));

        channel.convertTo(channel, CV_32F);
        // edgetaper(channel, channel, 6, 10);
        filter2DFreq(channel(roi), processedChannel, Hw);
        processedChannel.convertTo(processedChannel, CV_8U);
        cv::normalize(processedChannel, processedChannel, 0, 255, cv::NORM_MINMAX);

        processedChannels.push_back(processedChannel);
    }

    cv::Mat processedImage;
    cv::merge(processedChannels, processedImage);
    cv::imwrite(output_filename, processedImage);
}

void process_oof_blur(const std::filesystem::path& input_filename, const std::filesystem::path& output_filename,
                      const std::filesystem::path& output_psf_filename, int r, double snr)
{
    cv::Mat imgIn;
    imgIn = cv::imread(input_filename, cv::IMREAD_COLOR);

    std::vector<cv::Mat> channels(3);
    cv::split(imgIn, channels);

    std::vector<cv::Mat> processedChannels;
    for (int i = 0; i < 3; i++)
    {
        cv::Mat channel = channels[i].clone();
        cv::Mat processedChannel;

        cv::Rect roi = cv::Rect(0, 0, channel.cols & -2, channel.rows & -2);

        cv::Mat Hw, h;
        calcCirclePSF(h, roi.size(), output_psf_filename, r);
        calcWnrFilter(h, Hw, 1.0 / double(snr));

        channel.convertTo(channel, CV_32F);
        // edgetaper(channel, channel, 5, 10);
        filter2DFreq(channel(roi), processedChannel, Hw);
        processedChannel.convertTo(processedChannel, CV_8U);
        cv::normalize(processedChannel, processedChannel, 0, 255, cv::NORM_MINMAX);

        processedChannels.push_back(processedChannel);
    }

    cv::Mat processedImage;
    cv::merge(processedChannels, processedImage);
    cv::imwrite(output_filename, processedImage);
}
