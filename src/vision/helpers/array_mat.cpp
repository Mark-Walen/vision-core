//
// Created by wangxianggui on 2024/12/16.
//
#include "vision/helpers/array_mat.h"
#include <opencv2/core/mat.hpp>

using namespace vlue::array;

template<typename Tp, std::size_t Nm>
std::array<Tp, Nm> ArrayMatUtils::mat2Array(cv::Mat mat) {
    static_assert(Nm == mat.total(), "Array size must match the number of elements in the matrix.");
    std::array<Tp, Nm> arr;
    std::memcpy(arr.data(), mat.data, mat.total() * sizeof(Tp));
    return arr;
}

template<typename Tp, std::size_t Nm>
cv::Mat ArrayMatUtils::array2Mat(std::array<Tp, Nm> arr, unsigned int rows, unsigned int cols) {
    cv::Mat mat(rows, cols, cv::DataType<Tp>::type, arr.data());
    return mat;
}
