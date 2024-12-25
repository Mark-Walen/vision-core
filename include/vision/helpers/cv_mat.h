//
// Created by Mark-Walen on 2024/12/16.
//

#ifndef STEREO_VISION_CV_MAT_H
#define STEREO_VISION_CV_MAT_H

#include <vector>
#include <array>
#include <stdexcept>
#include <opencv2/core/mat.hpp>

namespace vlue::utils
{

    class MatUtils
    {
    public:
        template <typename Tp, std::size_t Nm>
        static std::array<Tp, Nm> mat2Array(const cv::Mat &mat)
        {
            if (Nm != mat.total())
            {
                throw std::invalid_argument("Array size must match the number of elements in the matrix.");
            }
            std::array<Tp, Nm> arr;
            std::memcpy(arr.data(), mat.data, mat.total() * sizeof(Tp));
            return arr;
        }

        template <typename Tp, std::size_t Nm>
        static cv::Mat array2Mat(const std::array<Tp, Nm> &arr, unsigned int rows, unsigned int cols)
        {
            if (rows * cols != Nm)
            {
                throw std::invalid_argument("Matrix dimensions do not match the array size.");
            }

            cv::Mat mat(rows, cols, cv::DataType<Tp>::type, arr.data());
            return mat;
        }

        template <typename Tp, typename Alloc = std::allocator<Tp>>
        static std::vector<Tp, Alloc> matToVector(const cv::Mat &mat)
        {
            if (mat.empty())
            {
                throw std::invalid_argument("Input matrix is empty.");
            }
            if (mat.type() != cv::DataType<Tp>::type)
            {
                throw std::invalid_argument("Matrix type does not match the specified type.");
            }

            return std::vector<Tp, Alloc>(mat.begin<Tp>(), mat.end<Tp>());
        }

        template <typename Tp, typename Alloc = std::allocator<Tp>>
        static cv::Mat vectorToMat(const std::vector<Tp, Alloc> &vec, unsigned int rows, unsigned int cols)
        {
            if (rows * cols != vec.size())
            {
                throw std::invalid_argument("Matrix dimensions do not match the vector size.");
            }
            return cv::Mat(rows, cols, cv::DataType<Tp>::type, const_cast<Tp *>(vec.data()));
        }
    };

} // array
// vlue

#endif // STEREO_VISION_CV_MAT_H
