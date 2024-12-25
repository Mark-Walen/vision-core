#ifndef VISION_SENSORS_CAMERA_CAMERA_H_
#define VISION_SENSORS_CAMERA_CAMERA_H_

#include <cstdint>
#include <string>
#include <opencv2/core/mat.hpp>

namespace YAML {
    class Node; // Forward declaration for YAML-cpp
}


namespace vlue::sensors {
    class Camera {
    public:
        // Field types and members
        using _height_type = uint32_t;
        _height_type height = 0;

        using _width_type = uint32_t;
        _width_type width = 0;

        using _distortion_model_type = std::string;
        _distortion_model_type distortion_model = "plumb_bob";

        using _d_type = cv::Mat;
        _d_type d = cv::Mat::zeros(5, 1, CV_64F); // 5x1 matrix for distortion coefficients

        using _k_type = cv::Mat;
        _k_type k = cv::Mat::eye(3, 3, CV_64F);     // 3x3 identity matrix for intrinsic parameters
        _k_type new_k = cv::Mat::eye(3, 3, CV_64F); // New camera matrix

        using _r_type = cv::Mat;
        _r_type r = cv::Mat::eye(3, 3, CV_64F); // 3x3 identity matrix for rectification

        using _t_type = cv::Mat;
        _t_type t = cv::Mat::zeros(3, 1, CV_64F); // 3x1 matrix for translation

        using _p_type = cv::Mat;
        _p_type p = cv::Mat::zeros(3, 4, CV_64F); // 3x4 matrix for projection

        using _roi_type = cv::Rect;
        _roi_type roi = cv::Rect(0, 0, 0, 0);

        using _map_type = cv::Mat;
        _map_type map_x, map_y;

    public:
        // Default constructor
        explicit Camera(_height_type height = 0, _width_type width = 0,
                        const _distortion_model_type &distortion_model = "plumb_bob",
                        const _d_type &d = cv::Mat::zeros(5, 1, CV_64F),
                        const _k_type &k = cv::Mat::eye(3, 3, CV_64F),
                        const _r_type &r = cv::Mat::eye(3, 3, CV_64F),
                        const _t_type &t = cv::Mat::zeros(3, 1, CV_64F),
                        const _p_type &p = cv::Mat::zeros(3, 4, CV_64F),
                        const _roi_type &roi = cv::Rect(0, 0, 0, 0));

        explicit Camera(const YAML::Node &config);

        explicit Camera(const std::string &yaml_path);

        void calc_optimal_new_camera_matrix(double alpha = 0.0f, bool update = false);

        void init_undistort_rectify_map();

    public:
        using RawPtr =
            Camera *;
        using ConstRawPtr =
            const Camera *;
        using SharedPtr =
            std::shared_ptr<Camera>;
        using ConstSharedPtr =
            std::shared_ptr<Camera const>;
    };
}


#endif
