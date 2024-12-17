//
// Created by wangxianggui on 2024/12/17.
//

#include "vision/sensors/camera/stereo_camera.h"
#include "vision/helpers/yaml.h"
#include <yaml-cpp/yaml.h>
#include <opencv2/calib3d.hpp>

using namespace vlue::sensors;
using namespace vlue::yaml;

StereoCamera::StereoCamera(Camera &left, Camera &right, const StereoCamera::_r_type &R_,
                           const StereoCamera::_t_type &T_, const StereoCamera::_q_type &Q_)
        : l(left), r(right), R(R_.clone()), T(T_.clone()), Q(Q_.clone()), size(static_cast<int>(left.width), static_cast<int>(left.height)) {
    // Ensure the image sizes of the left and right cameras match
    if (left.width != right.width || left.height != right.height)
    {
        throw std::invalid_argument("Left and right camera image sizes do not match.");
    }
}

StereoCamera::StereoCamera(const std::string &camera_config_path) {
    YAML::Node config = YAMLUtils::loadYamlConfig(camera_config_path);
    // Parse left and right camera configurations
    l = Camera(config["left"]);
    r = Camera(config["right"]);
    // Ensure the image sizes of the left and right cameras match
    if (l.width != r.width || l.height != r.height)
    {
        throw std::invalid_argument("Left and right camera image sizes do not match.");
    }
    size = cv::Size(static_cast<int>(l.width), static_cast<int>(l.height));
    R = YAMLUtils::yamlNodeToMat(config["R"], 3, 3, CV_64F, yaml::MatFillType::Eyes);
    T = YAMLUtils::yamlNodeToMat(config["tvec"], 3, 1, CV_64F);
    Q = YAMLUtils::yamlNodeToMat(config["Q"], 4, 4, CV_64F, yaml::MatFillType::Eyes);
}

StereoCamera::StereoCamera(const std::string &left_cam_config_path, const std::string &right_cam_config_path,
                           const StereoCamera::_r_type &R_, const StereoCamera::_t_type &T_,
                           const StereoCamera::_q_type &Q_)
                           : l(Camera(YAMLUtils::loadYamlConfig(left_cam_config_path))),
                             r(Camera(YAMLUtils::loadYamlConfig(right_cam_config_path))),
                             R(R_.clone()), T(T_.clone()), Q(Q_.clone()){
    // Ensure the image sizes of the left and right cameras match
    if (l.width != r.width || l.height != r.height)
    {
        throw std::invalid_argument("Left and right camera image sizes do not match.");
    }
    size = cv::Size(static_cast<int>(l.width), static_cast<int>(l.height));
}

void StereoCamera::stereo_rectify(double alpha) {
    cv::stereoRectify(l.k, l.d,
                      r.k, r.d,
                      size,
                      R, T,
                      l.r, r.r, l.p, r.p, Q,
                      cv::CALIB_ZERO_DISPARITY,
                      alpha);
}

void StereoCamera::init_stereo_undistort_rectify_map() {
    l.init_undistort_rectify_map();
    r.init_undistort_rectify_map();
}
