//
// Created by Mark-Walen on 2024/12/19.
//

#include "settings/settings.h"
#include "vision/helpers/yaml.h"

#include <iostream>

namespace vlue::settings {
    static auto indent(int layer) {
        return std::string(layer * 2, ' ');
    }

    void ROSConfig::parseYAMLNode(const YAML::Node &config) {
        if (!config) {
            return;
        }
        YAML::Node m_Remappings = config["remappings"];
        node_name = config["node_name"].as<std::string>("default");
        qos = config["qos"].as<int>(10);
        namespace_name = config["namespace_name"].as<std::string>("");
        if (m_Remappings && m_Remappings.IsSequence()) {
            for (const auto m_Remapping: m_Remappings) {
                if (m_Remapping.IsMap()) {
                    remappings.emplace_back(Remapping{
                        m_Remapping["from"].as<std::string>(),
                        m_Remapping["to"].as<std::string>()
                    });
                }
            }
        }
    }

    // Override the serialize method
    std::ostream &ROSConfig::serialize(std::ostream &os, int layer) const {
        os << indent(layer) << "ROSConfig:\n";
        os << indent(layer + 1) << "node_name: " << node_name << "\n";
        os << indent(layer + 1) << "qos: " << qos << "\n";
        os << indent(layer + 1) << "namespace_name: " << namespace_name << "\n";
        os << indent(layer + 1) << "remappings:\n";
        for (const auto &[from, to]: remappings) {
            os << indent(layer + 2) << "from: " << from << ", to: " << to << "\n";
        }
        return os;
    }

    void CameraConfig::parseYAMLNode(const YAML::Node &config) {
        config_path = config["config"].as<std::string>("");

        YAML::Node settings_ = config["settings"];
        if (settings_)
            return;
        settings.exposure = settings_["exposure"].as<std::string>();
        settings.white_balance = settings_["white_balance"].as<std::string>();
    }

    std::ostream &CameraConfig::serialize(std::ostream &os, int layer) const {
        os << indent(layer) << "CameraConfig:\n"
                << indent(layer + 1) << "config_path: " << config_path << "\n"
                << indent(layer + 1) << "settings:\n"
                << indent(layer + 2) << "exposure: " << settings.exposure << "\n"
                << indent(layer + 2) << "white_balance: " << settings.white_balance << "\n";
        return os;
    }

    void CaptureConfig::parseYAMLNode(const YAML::Node &config) {
        if (!config) {
            return;
        }
        YAML::Node sources_ = config["sources"];
        YAML::Node framerate_ = config["framerate"];
        YAML::Node resolution_ = config["resolution"];
        YAML::Node save_ = config["save"];
        if (sources_) {
            for (size_t idx = 0; idx < sources.size(); ++idx) {
                YAML::Node source = sources_[idx];
                try {
                    sources.emplace_back(source.as<int>());
                } catch (...) {
                    sources.emplace_back(source.as<std::string>());
                }
            }
        }
        if (sources.empty()) {
            sources.emplace_back(0);
        }
        replay = config["replay"].as<bool>(false);
        if (resolution_) {
            resolution.width = resolution_["width"].as<int>(640);
            resolution.height = resolution_["height"].as<int>(480);
        }
        if (save_) {
            save.enable = save_["enable"].as<bool>(false);
            save.save_path = save_["save_path"].as<std::string>("");
            save.format = save_["format"].as<std::string>("mp4");
            save.codec = save_["codec"].as<std::string>("H264");
            save.max_file_size = save_["max_file_size"].as<uint64_t>(1024 * 1024);
        }
    }

    std::ostream &CaptureConfig::serialize(std::ostream &os, int layer) const {
        os << indent(layer) << "CaptureConfig:\n";
        os << indent(layer + 1) << "sources: [";
        for (size_t i = 0; i < sources.size(); ++i) {
            if (std::holds_alternative<int>(sources[i])) {
                os << std::get<int>(sources[i]);
            } else {
                os << std::get<std::string>(sources[i]);
            }
            if (i < sources.size() - 1) os << ", ";
        }
        os << "]\n"
                << indent(layer + 1) << "replay: " << (replay ? "true" : "false") << "\n"
                << indent(layer + 1) << "framerate:\n"
                << indent(layer + 2) << "value: " << framerate.value << "\n"
                << indent(layer + 2) << "fallback: " << framerate.fallback << "\n"
                << indent(layer + 1) << "resolution:\n"
                << indent(layer + 2) << "width: " << resolution.width << "\n"
                << indent(layer + 2) << "height: " << resolution.height << "\n"
                << indent(layer + 1) << "save:\n"
                << indent(layer + 2) << "enable: " << (save.enable ? "true" : "false") << "\n"
                << indent(layer + 2) << "save_path: " << save.save_path << "\n"
                << indent(layer + 2) << "format: " << save.format << "\n"
                << indent(layer + 2) << "codec: " << save.codec << "\n"
                << indent(layer + 2) << "max_file_size: " << save.max_file_size << "\n";
        return os;
    }

    void LoggingConfig::parseYAMLNode(const YAML::Node &config) {
        level = config["level"].as<std::string>("info");
        file = config["file"].as<std::string>("");
    }

    std::ostream &LoggingConfig::serialize(std::ostream &os, int layer) const {
        os << indent(layer) << "LoggingConfig:\n"
                << indent(layer + 1) << "level: " << level << "\n"
                << indent(layer + 1) << "file: " << file << "\n";
        return os;
    }

    AppConfig::AppConfig() {
        ros_ = std::make_shared<ROSConfig>();
        camera_ = std::make_shared<CameraConfig>();
        capture_ = std::make_shared<CaptureConfig>();
        logging_ = std::make_shared<LoggingConfig>();
    }

    AppConfig::AppConfig(const std::string &app_config) : AppConfig(utils::YAMLUtils::loadYamlConfig(app_config)) {
    }

    void AppConfig::parseYAMLNode(const YAML::Node &config) {
        if (!config) {
            return;
        }
        ros_ = std::make_shared<ROSConfig>(config["ros"]);
        camera_ = std::make_shared<CameraConfig>(config["camera"]);
        capture_ = std::make_shared<CaptureConfig>(config["capture"]);
        logging_ = std::make_shared<LoggingConfig>(config["logging"]);
    }

    std::ostream &AppConfig::serialize(std::ostream &os, int layer) const {
        os << indent(layer) << "AppConfig:\n";
        ros_->serialize(os, layer + 1);
        camera_->serialize(os, layer + 1);
        capture_->serialize(os, layer + 1);
        logging_->serialize(os, layer + 1);
        return os;
    }
}
