//
// Created by Mark-Walen on 2024/12/16.
//
#include "vision/sensors/camera/camera.h"
#include "vision/helpers/yaml.h"
#include <yaml-cpp/yaml.h>
#include <iostream>
#include <opencv2/calib3d.hpp>

namespace vlue::sensors {
    Camera::Camera(const Camera::_height_type height, const Camera::_width_type width, const Camera::_distortion_model_type& distortion_model,
                   const Camera::_d_type &d, const Camera::_k_type &k, const Camera::_r_type &r, const Camera::_t_type &t,
                   const Camera::_p_type &p, const Camera::_roi_type &roi)
            : height(height), width(width),
              d(d.clone()),
              k(k.clone()),
              r(r.clone()),
              t(t.clone()),
              p(p.clone()),
              roi(roi.empty() ? cv::Rect(0, 0, static_cast<int>(width), static_cast<int>(height)) : roi) {
        this->distortion_model = distortion_model;
        if (this->distortion_model != "plumb_bob" && this->distortion_model != "rational_polynomial")
        {
            std::cout << this->distortion_model << std::endl;
            throw std::invalid_argument("Invalid distortion model. Use 'plumb_bob' or 'rational_polynomial'.");
        }
    }

    Camera::Camera(const YAML::Node &config)
    : height(config["height"].as<uint32_t>(480)),
      width(config["width"].as<uint32_t>(640)),
      distortion_model(config["distortion_model"].as<std::string>("plumb_bob"))
    {
        YAML::Node roiNode = config["roi"];

        if (distortion_model != "plumb_bob" && distortion_model != "rational_polynomial")
        {
            throw std::invalid_argument("Invalid distortion model. Use 'plumb_bob' or 'rational_polynomial'.");
        }
        this->d = yaml::YAMLUtils::yamlNodeToMat(config["d"], 5, 1, CV_64F);
        this->k = yaml::YAMLUtils::yamlNodeToMat(config["k"], 3, 3, CV_64F, yaml::MatFillType::Eyes);
        this->r = yaml::YAMLUtils::yamlNodeToMat(config["R"], 3, 3, CV_64F, yaml::MatFillType::Eyes);
        this->t = yaml::YAMLUtils::yamlNodeToMat(config["tvec"], 3, 1, CV_64F);
        this->p = yaml::YAMLUtils::yamlNodeToMat(config["P"], 3, 4, CV_64F);
        if(roiNode)
            this->roi = cv::Rect(roiNode[0].as<int>(), roiNode[1].as<int>(), roiNode[2].as<int>(), roiNode[3].as<int>());
        else
            this->roi = cv::Rect(0, 0, static_cast<int>(this->width), static_cast<int>(this->height));
    }

    Camera::Camera(const std::string &yaml_path) : Camera(yaml::YAMLUtils::loadYamlConfig(yaml_path)) {}

    void Camera::calc_optimal_new_camera_matrix(const double alpha, const bool update) {
        if (update || new_k.empty())
        {
            // Compute the optimal new camera matrix
            new_k = cv::getOptimalNewCameraMatrix(
                    k, // Intrinsic matrix
                    d, // Distortion coefficients
                    cv::Size(static_cast<int>(this->width), static_cast<int>(this->height)),
                    alpha,
                    cv::Size(static_cast<int>(this->width), static_cast<int>(this->height)),
                    &this->roi // ROI output
            );
        }
    }

    void Camera::init_undistort_rectify_map() {
        cv::initUndistortRectifyMap(k, d, r, p, cv::Size(width, height), CV_32FC1, map_x, map_y);
    }
}
