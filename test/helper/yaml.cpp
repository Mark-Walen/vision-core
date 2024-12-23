//
// Created by Mark on 2024/12/14.
//
#include "helpers/yaml.h"

#include <gtest/gtest.h>
#include <fstream>
#include <yaml-cpp/yaml.h>

using namespace vlue::yaml;

class YAMLUtilsTest : public ::testing::Test {
protected:
    static std::string create_test_config() {
        // Create a temporary YAML configuration file for testing
        std::string config_content = R"(
left:
  width: 640
  height: 480
  distortion_model: "plumb_bob"
  d: [0.1, 0.2, 0.3, 0.4, 0.5]
  k: [500.0, 0.0, 320.0, 0.0, 500.0, 240.0, 0.0, 0.0, 1.0]
  roi: [0, 0, 640, 480]
right:
  width: 640
  height: 480
  distortion_model: "plumb_bob"
  d: [0.1, 0.2, 0.3, 0.4, 0.5]
  k: [500.0, 0.0, 320.0, 0.0, 500.0, 240.0, 0.0, 0.0, 1.0]
  roi: [0, 0, 640, 480]
R: [1.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 1.0]
tvec: [0.05, 0.0, 0.0]
    )";

        // Write the YAML content to a temporary file
        std::string temp_file = "test_stereo_camera.yaml";
        std::ofstream file(temp_file);
        file << config_content;
        file.close();

        return temp_file;
    }
};

TEST_F(YAMLUtilsTest, LoadYAMLConfig) {
    std::string config_path = create_test_config();

    YAML::Node node = YAMLUtils::loadYamlConfig(config_path);
    EXPECT_TRUE(node);
}

TEST_F(YAMLUtilsTest, LoadYAMLConfigBadFile) {
    std::string config_path = "./invalid_file.yaml";

    EXPECT_THROW({
                     YAMLUtils::loadYamlConfig(config_path);
                 }, std::runtime_error);
}

