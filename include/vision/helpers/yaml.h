#ifndef VISION_HELPERS_YAML_H_
#define VISION_HELPERS_YAML_H_

#include <string>
#include <array>

#include <yaml-cpp/yaml.h>

namespace cv {
    template<typename Tp> class Mat_;
    class Mat;
}

namespace YAML {
    class Node;
}

namespace vlue::utils {
    class YAMLUtils {
    public:
        enum class FillType_ {
            Zeros,
            Eyes,
            Ones,
            Custom
        };

        static YAML::Node loadYamlConfig(const std::string &yaml_path);

        template<typename Tp, std::size_t Nm>
        static std::array<Tp, Nm> yamlNodeToArray(const YAML::Node& node, int rows, int cols, FillType_ fill = FillType_::Zeros);

        template<typename Tp>
        static cv::Mat_<Tp> yamlNodeToMat(const YAML::Node& node, int rows, int cols, FillType_ fill = FillType_::Zeros);

        static cv::Mat yamlNodeToMat(const YAML::Node& node, int rows, int cols, int type, FillType_ fill = FillType_::Zeros);
    };

    using ArrayFillType = YAMLUtils::FillType_;
    using MatFillType = YAMLUtils::FillType_;
}


#endif
