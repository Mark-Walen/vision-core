# CMakeList.txt: Stereo 的 CMake 项目，在此处包括源代码并定义
# 项目特定的逻辑。
#
cmake_minimum_required(VERSION 3.8)

# 如果支持，请为 MSVC 编译器启用热重载。
if (POLICY CMP0141)
    cmake_policy(SET CMP0141 NEW)
    cmake_policy(SET CMP0074 NEW)
    set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif ()

project("stereo_vision")

# Options
option(BUILD_TESTING "Build tests" ON)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find OpenCV
set(OpenCV_ROOT "${VCPKG_INSTALLED_DIR}/x64-mingw-dynamic/share/opencv4")
find_package(OpenCV REQUIRED)

# Include OpenCV and yaml-cpp headers
message(STATUS "OpenCV INCLUDE_DIRS: ${OpenCV_INCLUDE_DIRS}")
message(STATUS "OpenCV LIBRARIES: ${OpenCV_LIBS}")

# Find yaml-cpp
find_package(yaml-cpp CONFIG REQUIRED)

# Include OpenCV and yaml-cpp headers
message(STATUS "yaml-cpp INCLUDE_DIRS: ${YAML_CPP_INCLUDE_DIR}")
message(STATUS "yaml-cpp LIBRARIES: ${YAML_CPP_LIBRARIES}")

find_package(Eigen3 REQUIRED)
find_package(argparse CONFIG REQUIRED)

include_directories(${PROJECT_SOURCE_DIR}/include/
        ${OpenCV_INCLUDE_DIRS}
        ${YAML_CPP_INCLUDE_DIR}
        ${EIGEN3_INCLUDE_DIR}
)

# 将源代码添加到此项目的可执行文件。
add_library(${PROJECT_NAME}
        "src/vision/helpers/yaml.cc"
        "include/vision/helpers/yaml.h"
        include/vision/helpers/cv_mat.h
        src/vision/sensors/camera/camera.cpp
        include/vision/sensors/camera/camera.h
        src/vision/sensors/camera/stereo_camera.cpp
        include/vision/sensors/camera/stereo_camera.h
        src/vision/capture/stereo_capture.cpp
        include/vision/capture/stereo_capture.h
        src/settings/settings.cpp include/settings/settings.h
        include/vision/exceptions/exceptions.h
)

target_link_libraries(${PROJECT_NAME} PRIVATE ${OpenCV_LIBS} ${YAML_CPP_LIBRARIES} Eigen3::Eigen argparse::argparse)
target_include_directories(${PROJECT_NAME} PRIVATE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

if (CMAKE_VERSION VERSION_GREATER 3.12)
    set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 20)
endif ()

enable_testing()
message(STATUS "Test: ${BUILD_TESTING}")
if (BUILD_TESTING)
    find_package(GTest REQUIRED)
    include_directories(${GTest_DIR}/../../../include)
    message(STATUS "GTest Directory: ${GTest_DIR}")
    # TODO: 如有需要，请添加测试并安装目标。
    add_executable(test_yaml
            test/helper/yaml.cpp)
    target_link_libraries(test_yaml
            ${PROJECT_NAME}
            GTest::GTest GTest::Main)
    target_include_directories(test_yaml PRIVATE
            $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
            $<INSTALL_INTERFACE:include>
    )

    add_executable(test_sensors_camera
            test/sensors/test_sensors_camera.cpp
    )
    target_link_libraries(test_sensors_camera
            ${PROJECT_NAME}
            GTest::GTest GTest::Main)
    target_include_directories(test_sensors_camera PRIVATE
            $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
            $<INSTALL_INTERFACE:include>
    )

    add_executable(test_capture
            test/capture/test_capture.cpp
    )
    target_link_libraries(test_capture
            ${PROJECT_NAME})
    target_include_directories(test_capture PRIVATE
            $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
            $<INSTALL_INTERFACE:include>
    )

    add_executable(test_settings
            test/test_settings.cpp
    )
    target_link_libraries(test_settings
            ${PROJECT_NAME}
            GTest::GTest GTest::Main)
    target_include_directories(test_settings PRIVATE
            $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
            $<INSTALL_INTERFACE:include>
    )

endif ()
