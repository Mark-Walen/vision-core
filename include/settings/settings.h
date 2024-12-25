//
// Created by Mark-Walen on 2024/12/19.
//

#ifndef SETTINGS_H
#define SETTINGS_H

#include <cstdint>
#include <memory>
#include <string>
#include <variant>
#include <vector>

namespace YAML {
    class Node;
}

namespace vlue::settings {
    struct Config {
    public:
        virtual ~Config() = default;

        Config() = default;

        explicit Config([[maybe_unused]] const YAML::Node &config) {}

        virtual std::ostream &serialize(std::ostream &os, int layer) const = 0;

    public:
        template<typename Tp>
        using SharedPtr = std::shared_ptr<Tp>;

    protected:
        virtual void parseYAMLNode(const YAML::Node &config) = 0;
    };

    struct ROSConfig final : Config {
        std::string node_name{"default"};
        int qos{10};
        std::string namespace_name;

        struct Remapping {
            std::string from;
            std::string to;
        };

        std::vector<Remapping> remappings;

        ROSConfig() = default;
        // Constructor to parse YAML::Node
        explicit ROSConfig(const YAML::Node &config) : Config(config) {
            parseYAMLNode(config);
        }
        std::ostream &serialize(std::ostream &os, int layer) const override;

    protected:
        void parseYAMLNode(const YAML::Node &config) override;
    };

    // Camera configuration struct
    struct CameraConfig final : Config {
        std::string config_path;

        struct Settings {
            std::string exposure{"auto"};
            std::string white_balance{"auto"};
        } settings;

        CameraConfig() = default;
        explicit CameraConfig(const YAML::Node &config) : Config(config) {
            parseYAMLNode(config);
        }

        std::ostream &serialize(std::ostream &os, int layer) const override;

    protected:
        void parseYAMLNode(const YAML::Node &config) override;
    };

    // Capture configuration struct
    struct CaptureConfig final : Config {
        std::vector<std::variant<int, std::string>> sources{};
        bool replay{false};

        struct Framerate {
            int value{30};
            int fallback{15};
        } framerate;

        struct Resolution {
            int width;
            int height;
        } resolution{640, 480};

        struct Save {
            bool enable{false};
            std::string save_path;
            std::string format{"mp4"};
            std::string codec{"H264"};
            uint64_t max_file_size{1024 * 1024}; // In bytes
        } save;

        CaptureConfig() : sources(1, 0) {}
        explicit CaptureConfig(const YAML::Node &config) : Config(config) {
            parseYAMLNode(config);
        }

        std::ostream &serialize(std::ostream &os, int layer) const override;

    protected:
        void parseYAMLNode(const YAML::Node &config) override;
    };

    // Logging configuration struct
    struct LoggingConfig final : Config {
        std::string level{"info"};
        std::string file;

        LoggingConfig() = default;
        explicit LoggingConfig(const YAML::Node &config) : Config(config) {
            parseYAMLNode(config);
        }

        std::ostream &serialize(std::ostream &os, int layer) const override;

    protected:
        void parseYAMLNode(const YAML::Node &config) override;
    };

    // Main configuration struct
    struct AppConfig final : Config {
        AppConfig();
        explicit AppConfig(const std::string &app_config);
        explicit AppConfig(const YAML::Node &config) : Config(config) {
            parseYAMLNode(config);
        }
        std::ostream &serialize(std::ostream &os, int layer) const override;

        [[nodiscard]] ROSConfig &getROSConfig() const { return *ros_;}
        [[nodiscard]] CameraConfig &getCameraConfig() const { return *camera_;}
        [[nodiscard]] CaptureConfig &getCaptureConfig() const { return *capture_;}
        [[nodiscard]] LoggingConfig &getLoggingConfig() const { return *logging_;}

    protected:
        void parseYAMLNode(const YAML::Node &config) override;

    private:
        SharedPtr<ROSConfig> ros_;
        SharedPtr<CameraConfig> camera_;
        SharedPtr<CaptureConfig> capture_;
        SharedPtr<LoggingConfig> logging_;
    };

    // Overload << operator for Config
    inline std::ostream &operator<<(std::ostream &os, const Config &config) {
        return config.serialize(os, 0);
    }
} // namespace vlue::settings


#endif //SETTINGS_H
