#include "fft_cufft.h"

void cuFFT::init(unsigned width, unsigned height, unsigned num_of_feats, unsigned num_of_scales)
{
    m_width = width;
    m_height = height;
    m_num_of_feats = num_of_feats;
    m_num_of_scales = num_of_scales;
    std::cout << "FFT: cuFFT" << std::endl;
}

void cuFFT::set_window(const cv::Mat &window)
{
     m_window = window;
}

ComplexMat cuFFT::forward(const cv::Mat &input)
{
    cv::Mat complex_result;
    cv::dft(input, complex_result, cv::DFT_COMPLEX_OUTPUT);
    return ComplexMat(complex_result);
}

ComplexMat cuFFT::forward_window(const std::vector<cv::Mat> &input)
{
    int n_channels = input.size();
    ComplexMat result(input[0].rows, input[0].cols, n_channels);

    for (int i = 0; i < n_channels; ++i) {
        cv::Mat complex_result;
        cv::dft(input[i].mul(m_window), complex_result, cv::DFT_COMPLEX_OUTPUT);
        result.set_channel(i, complex_result);
    }
    return result;
}

cv::Mat cuFFT::inverse(const ComplexMat &inputf)
{
    cv::Mat real_result;
    if (inputf.n_channels == 1) {
        cv::dft(inputf.to_cv_mat(), real_result, cv::DFT_INVERSE | cv::DFT_REAL_OUTPUT | cv::DFT_SCALE);
    } else {
        std::vector<cv::Mat> mat_channels = inputf.to_cv_mat_vector();
        std::vector<cv::Mat> ifft_mats(inputf.n_channels);
        for (int i = 0; i < inputf.n_channels; ++i) {
            cv::dft(mat_channels[i], ifft_mats[i], cv::DFT_INVERSE | cv::DFT_REAL_OUTPUT | cv::DFT_SCALE);
        }
        cv::merge(ifft_mats, real_result);
    }
    return real_result;
}

cuFFT::~cuFFT()
{

}