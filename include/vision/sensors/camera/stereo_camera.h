//
// Created by wangxianggui on 2024/12/17.
//

#ifndef STEREO_VISION_STEREO_CAMERA_H
#define STEREO_VISION_STEREO_CAMERA_H

#include <string>
#include "camera.h"

namespace vlue::sensors {
    class StereoCamera {
    public:
        Camera l, r;
        using _r_type = cv::Mat;
        _r_type R;
        using _t_type = cv::Mat;
        _t_type T;
        using _q_type = cv::Mat;
        _q_type Q;
        cv::Size size;

    public:
        StereoCamera(const Camera &left, const Camera &right,
                     const _r_type &R_ = cv::Mat::eye(3, 3, CV_64F),
                     const _t_type &T_ = cv::Mat::zeros(3, 1, CV_64F),
                     const _q_type &Q_ = cv::Mat::eye(4, 4, CV_64F));

        explicit StereoCamera(const std::string &camera_config_path);
        StereoCamera(const std::string &left_cam_config_path, const std::string &right_cam_config_path,
                     const _r_type &R_ = cv::Mat::eye(3, 3, CV_64F),
                     const _t_type &T_ = cv::Mat::zeros(3, 1, CV_64F),
                     const _q_type &Q_ = cv::Mat::eye(4, 4, CV_64F));
        ~StereoCamera() = default;

        void stereo_rectify(double alpha = 0.0f);

        void init_stereo_undistort_rectify_map();
    };
}


#endif //STEREO_VISION_STEREO_CAMERA_H
