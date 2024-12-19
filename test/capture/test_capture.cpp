//
// Created by Mark-Walen on 2024/12/18.
//
#include "vision/capture/stereo_capture.h"

#include <opencv2/opencv.hpp>

using namespace vlue::capture;

int test_capture() {
    StereoCapture stereo{0, 640, 480};

    cv::Mat frame, left, right;
    // Read a frame from the video source

    switch (stereo.captureStereoFrame(left, right)) {
        case CaptureFrameState::HasRightFrame:
            cv::imshow("right", right);
        case CaptureFrameState::HasLeftFrame:
            cv::imshow("left", left);
            break;
        case CaptureFrameState::NoFrame:
            std::cout << "NoFrame" << std::endl;
            return -1;
        case CaptureFrameState::NoCapture:
            std::cout << "NoCapture" << std::endl;
            return -1;
    }

    // Wait for a key press and check for 'q' to quit
    cv::waitKey(0);

    // Release the video capture and close windows
    cv::destroyAllWindows();
    return 0;
}

int test_imread() {
    cv::Mat image = cv::imread(R"(D:\workspace\python\opencv\apriltag\TAG36H11\tag36h11_0.png)");
    if (image.empty()) {
        std::cerr << "Failed to load image!" << std::endl;
        return -1;
    }

    // Apply a simple operation
    cv::Mat gray_image;
    cv::cvtColor(image, gray_image, cv::COLOR_BGR2GRAY);

    // Check if the operation works without crash
    std::cout << "Image processing successful!" << std::endl;

    return 0;
}

int main() {
    cv::setUseOptimized(false);
    return test_capture();
}
