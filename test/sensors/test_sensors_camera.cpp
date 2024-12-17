//
// Created by wangxianggui on 2024/12/16.
//
#include "vision/sensors/camera/camera.h"

#include <gtest/gtest.h>
#include <fstream>
#include <yaml-cpp/node/impl.h>
#include <opencv2/core.hpp>

using namespace vlue::sensors;

class CameraTest : public ::testing::Test {
protected:
    static std::string create_test_config() {
        // Create a temporary YAML configuration file for testing
        std::string config_content = R"(
height: 480
width: 640
distortion_model: "plumb_bob"
d: [0.1, 0.2, 0.3, 0.4, 0.5]
k: [500.0, 0.0, 320.0, 0.0, 500.0, 240.0, 0.0, 0.0, 1.0]
R: [1.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 1.0]
P: [500.0, 0.0, 320.0, 0.0, 0.0, 500.0, 240.0, 0.0, 0.0, 0.0, 1.0, 0.0]
roi: [0, 0, 640, 480]
    )";

        // Write the YAML content to a temporary file
        std::string temp_file = "test_stereo_camera.yaml";
        std::ofstream file(temp_file);
        file << config_content;
        file.close();

        return temp_file;
    }
};

TEST_F(CameraTest, TestDefaultConstructor) {
    Camera c(480, 640);
    EXPECT_EQ(c.height, 480);
    EXPECT_EQ(c.width, 640);
    EXPECT_EQ(c.distortion_model, "plumb_bob");

    // Check default distortion coefficients
    EXPECT_EQ(c.d.rows, 5);
    EXPECT_EQ(c.d.cols, 1);
    for (int i = 0; i < c.d.rows; ++i) {
        EXPECT_DOUBLE_EQ(c.d.at<double>(i, 0), 0.0);
    }

    // Check intrinsic matrix (default identity matrix)
    cv::Mat identity = cv::Mat::eye(3, 3, CV_64F);
    EXPECT_TRUE(cv::norm(c.k, identity, cv::NORM_L2) == 0);
    EXPECT_TRUE(cv::norm(c.r, identity, cv::NORM_L2) == 0);

    // Check default distortion coefficients
    EXPECT_EQ(c.t.rows, 3);
    EXPECT_EQ(c.t.cols, 1);
    for (int i = 0; i < c.t.rows; ++i) {
        EXPECT_DOUBLE_EQ(c.t.at<double>(i, 0), 0.0);
    }
}

TEST_F(CameraTest, TestYAMLConfigPathConstructor) {
    std::string config_path = create_test_config();
    Camera c(config_path);

    EXPECT_EQ(c.height, 480);
    EXPECT_EQ(c.width, 640);
    EXPECT_EQ(c.distortion_model, "plumb_bob");

    // Check distortion coefficients
    EXPECT_DOUBLE_EQ(c.d.at<double>(0, 0), 0.1);
    EXPECT_DOUBLE_EQ(c.d.at<double>(1, 0), 0.2);
    EXPECT_DOUBLE_EQ(c.d.at<double>(2, 0), 0.3);
    EXPECT_DOUBLE_EQ(c.d.at<double>(3, 0), 0.4);
    EXPECT_DOUBLE_EQ(c.d.at<double>(4, 0), 0.5);

    // Check intrinsic matrix
    cv::Mat k_expected = (cv::Mat_<double>(3, 3) << 500.0, 0.0, 320.0, 0.0, 500.0, 240.0, 0.0, 0.0, 1.0);
    EXPECT_TRUE(cv::norm(c.k, k_expected, cv::NORM_L2) == 0);

    cv::Mat identity = cv::Mat::eye(3, 3, CV_64F);
    EXPECT_TRUE(cv::norm(c.r, identity, cv::NORM_L2) == 0);

    // Check default distortion coefficients
    for (int i = 0; i < c.t.rows; ++i) {
        EXPECT_DOUBLE_EQ(c.t.at<double>(i, 0), 0.0);
    }
}
