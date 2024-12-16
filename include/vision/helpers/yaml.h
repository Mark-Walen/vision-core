#ifndef VISION_HELPERS_YAML_H_
#define VISION_HELPERS_YAML_H_

#include <string>
#include <array>

namespace YAML {
    class Node; // Forward declaration for YAML-cpp
}


namespace vlue::yaml {
    class YAMLUtils {
    public:
        enum class FillType_ {
            Zeros,
            Eyes,
            Ones
        };

        static YAML::Node loadYamlConfig(const std::string &yaml_path);

        template<typename Tp, size_t Nm>
        static std::array<Tp, Nm> yamlNodeToArray(const YAML::Node& node, int rows, int cols, FillType_ fill = FillType_::Zeros);
    };

    using ArrayFillType = YAMLUtils::FillType_;
    using MatFillType = YAMLUtils::FillType_;
}


#endif
