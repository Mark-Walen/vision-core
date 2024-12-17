//
// Created by Mark-Walen on 2024/12/16.
//

#ifndef STEREO_VISION_ARRAY_MAT_H
#define STEREO_VISION_ARRAY_MAT_H

#include <array>

namespace cv {
    class Mat;
}


namespace vlue::array {

    class ArrayMatUtils {
        template<typename Tp, std::size_t Nm>
        static std::array<Tp, Nm> mat2Array(cv::Mat mat);

        template<typename Tp, std::size_t Nm>
        static cv::Mat array2Mat(std::array<Tp, Nm> arr, unsigned int rows, unsigned int cols);
    };

} // array
// vlue

#endif //STEREO_VISION_ARRAY_MAT_H