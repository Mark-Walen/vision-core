#include "vision/helpers/yaml.h"

#include <yaml-cpp/yaml.h>
#include <stdexcept>

using YAMLUtils = vlue::yaml::YAMLUtils;

YAML::Node YAMLUtils::loadYamlConfig(const std::string& yaml_path)
{
    YAML::Node config;
    try
    {
        config = YAML::LoadFile(yaml_path);
    }
    catch (const YAML::BadFile& e)
    {
        throw std::runtime_error("Failed to open YAML configuration file: " + yaml_path);
    }
    catch (const YAML::ParserException& e)
    {
        throw std::runtime_error("Failed to parse YAML file: " + yaml_path + "\n" + e.what());
    }
    return config;
}

template <typename Tp, std::size_t Nm>
std::array<Tp, Nm> YAMLUtils::yamlNodeToArray(const YAML::Node& node, int rows, int cols, FillType_ fill) {
    std::array<Tp, Nm> result;

    if (rows * cols != static_cast<int>(Nm)) {
        throw std::invalid_argument("Dimensions do not match the size of the array.");
    }
    if (!node) {
        if (fill == FillType_::Zeros) {
            result.fill(static_cast<Tp>(0));
        }
        else if (fill == FillType_::Ones) {
            result.fill(static_cast<Tp>(1));
        }
        else if (fill == FillType_::Eyes) {
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
        for (const auto& value : node) {
            if (idx >= static_cast<int>(Nm)) {
                break;
            }
            result[idx++] = value.as<Tp>();
        }
    }
    else
    {
        throw std::invalid_argument("");
    }

    return result;
}
