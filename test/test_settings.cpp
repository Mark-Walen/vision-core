//
// Created by Mark-Walen on 2024/12/19.
//
#include <gtest/gtest.h>
#include "settings/settings.h"
#include <iostream>

using namespace vlue::settings;

class SettingsTest : public ::testing::Test {
protected:
    static std::string getTestFilePath(const std::string &filename) {
        // Adjust path based on the project structure
        return "../resources/" + filename;
    }
};

TEST_F(SettingsTest, DefaultConstructor) {
    const AppConfig app_config{};
    std::cout << app_config << std::endl;
}

TEST_F(SettingsTest, YAMLFileConstructor) {
    const std::string settings_file = getTestFilePath("settings.yaml");

    const AppConfig app_config{settings_file};
    std::cout << app_config << std::endl;
}
