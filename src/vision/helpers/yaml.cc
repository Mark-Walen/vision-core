#include "vision/helpers/yaml.h"
#include "vision/exceptions/exceptions.h"
#include <opencv2/core/mat.hpp>
#include <stdexcept>

namespace vlue::utils {
    YAML::Node YAMLUtils::loadYamlConfig(const std::string &yaml_path) {
        YAML::Node config;
        try {
            config = YAML::Node(yaml_path);
        } catch ([[maybe_unused]] const exceptions::BadFile &e) {
            throw std::runtime_error("Failed to open YAML configuration file: " + yaml_path);
        }
        catch (const exceptions::ParserException &e) {
            throw std::runtime_error("Failed to parse YAML file: " + yaml_path + "\n" + e.what());
        }
        return config;
    }

    template<typename Tp, std::size_t Nm>
    std::array<Tp, Nm> YAMLUtils::yamlNodeToArray(const YAML::Node &node, int rows, int cols, FillType_ fill) {
        std::array<Tp, Nm> result;

        if (rows * cols != static_cast<int>(Nm)) {
            throw std::invalid_argument("Dimensions do not match the size of the array.");
        }
        if (!node) {
            if (fill == FillType_::Zeros) {
                result.fill(static_cast<Tp>(0));
            } else if (fill == FillType_::Ones) {
                result.fill(static_cast<Tp>(1));
            } else if (fill == FillType_::Eyes) {
                if (rows != cols) {
                    throw std::invalid_argument("Eye matrix requires square dimensions.");
                }
                result.fill(static_cast<Tp>(0));
                for (int i = 0; i < rows; ++i) {
                    result[i * cols + i] = static_cast<Tp>(1);
                }
            }
            return result;
        }

        if (node.IsSequence()) {
            int idx = 0;
            for (size_t i = 0; i < node.size(); i++) {
                YAML::Node element = node[i];
                if (idx >= static_cast<int>(Nm)) {
                    break;
                }
                result[idx++] = element.as<Tp>();
            }
        } else {
            throw std::invalid_argument("");
        }

        return result;
    }

    template<typename Tp>
    cv::Mat_<Tp> YAMLUtils::yamlNodeToMat(const YAML::Node &node, int rows, int cols, FillType_ fill) {
        cv::Mat_<Tp> mat(rows, cols);
        if (node)
            fill = FillType_::Custom;

        switch (fill) {
            case FillType_::Zeros:
                mat = cv::Mat_<Tp>::zeros(rows, cols);
                break;
            case FillType_::Ones:
                mat = cv::Mat_<Tp>::ones(rows, cols);
                break;
            case FillType_::Eyes:
                mat = cv::Mat_<Tp>::eye(rows, cols);
                break;
            case FillType_::Custom:
                for (int i = 0; i < rows; ++i) {
                    for (int j = 0; j < cols; ++j) {
                        mat(i, j) = node[i * cols + j].as<Tp>(); // Read data from YAML
                    }
                }
                break;
        }
        return mat;
    }

    cv::Mat YAMLUtils::yamlNodeToMat(const YAML::Node &node, int rows, int cols, int type, FillType_ fill) {
        cv::Mat mat(rows, cols, type);

        if (node)
            fill = FillType_::Custom;

        switch (fill) {
            case FillType_::Zeros:
                mat.setTo(cv::Scalar(0));
                break;
            case FillType_::Ones:
                mat.setTo(cv::Scalar(1));
                break;
            case FillType_::Eyes:
                mat = cv::Mat::eye(rows, cols, type);
                break;
            case FillType_::Custom:
                for (int i = 0; i < rows; ++i) {
                    for (int j = 0; j < cols; ++j) {
                        // Handle matrix types dynamically
                        switch (type) {
                            case CV_8U:
                                mat.at<uint8_t>(i, j) = node[i * cols + j].as<uint8_t>();
                                break;
                            case CV_8S:
                                mat.at<int8_t>(i, j) = node[i * cols + j].as<int8_t>();
                                break;
                            case CV_16U:
                                mat.at<uint16_t>(i, j) = node[i * cols + j].as<uint16_t>();
                                break;
                            case CV_16S:
                                mat.at<short>(i, j) = node[i * cols + j].as<int16_t>();
                                break;
                            case CV_32S:
                                mat.at<int>(i, j) = node[i * cols + j].as<int>();
                                break;
                            case CV_32F:
                                mat.at<float>(i, j) = node[i * cols + j].as<float>();
                                break;
                            case CV_64F:
                                mat.at<double>(i, j) = node[i * cols + j].as<double>();
                                break;
                            default:
                                throw std::runtime_error("Unsupported matrix type");
                        }
                    }
                }
                break;
        }
        return mat;
    }
}
