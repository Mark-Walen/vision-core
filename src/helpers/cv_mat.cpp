//
// Created by Mark-Walen on 2024/12/16.
//
#include "helpers/cv_mat.h"
#include <opencv2/core/mat.hpp>

namespace vlue::helpers {
    template<typename Tp, std::size_t Nm>
    std::array<Tp, Nm> MatUtils::mat2Array(cv::Mat mat) {
        if (Nm != mat.total()) {
            throw std::invalid_argument("Array size must match the number of elements in the matrix.");
        }
        std::array<Tp, Nm> arr;
        std::memcpy(arr.data(), mat.data, mat.total() * sizeof(Tp));
        return arr;
    }

    template<typename Tp, std::size_t Nm>
    cv::Mat MatUtils::array2Mat(std::array<Tp, Nm> arr, unsigned int rows, unsigned int cols) {
        if (rows * cols != Nm) {
            throw std::invalid_argument("Matrix dimensions do not match the array size.");
        }

        cv::Mat mat(rows, cols, cv::DataType<Tp>::type, arr.data());
        return mat;
    }

    template<typename Tp>
    std::vector<Tp> MatUtils::matToVector(const cv::Mat &mat) {
        if (mat.empty()) {
            throw std::invalid_argument("Input matrix is empty.");
        }
        if (mat.type() != cv::DataType<Tp>::type) {
            throw std::invalid_argument("Matrix type does not match the specified type.");
        }

        return std::vector<Tp>(mat.begin<Tp>(), mat.end<Tp>());
    }

    template <typename Tp>
    cv::Mat MatUtils::vectorToMat(const std::vector<Tp> &vec, unsigned int rows, unsigned int cols)
    {
        if (rows * cols != vec.size()) {
            throw std::invalid_argument("Matrix dimensions do not match the vector size.");
        }
        return cv::Mat(rows, cols, cv::DataType<Tp>::type, const_cast<Tp *>(vec.data()));
    }
}
