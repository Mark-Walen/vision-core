//
// Created by Mark-Walen on 2024/12/17.
//

#include "vision/capture/stereo_capture.h"

#include <iostream>
#include <opencv2/core/mat.hpp>
#include <opencv2/videoio.hpp>

namespace vlue::capture {
    void StereoCapture::cap_init_(const std::shared_ptr<cv::VideoCapture> &cap, int source, int width, int height, int frameWidthScale) {
        if (!cap->isOpened()) {
            throw std::runtime_error("Failed to open video sources: " + source);
        }
        uint32_t frameWidth = width * frameWidthScale;
        std::cout << "Info: set resolution to" << frameWidth << "x" << height << " instead." << std::endl;
        cap->set(cv::CAP_PROP_FRAME_WIDTH, frameWidth);
        cap->set(cv::CAP_PROP_FRAME_HEIGHT, height);
        this->width = width;
        this->height = height;

        // Check resolution status
        int actual_width = static_cast<int>(cap->get(cv::CAP_PROP_FRAME_WIDTH));
        int actual_height = static_cast<int>(cap->get(cv::CAP_PROP_FRAME_HEIGHT));
        if (actual_width != frameWidth || actual_height != height) {
            std::cerr << "Warning: Failed to set resolution. Using "
                      << actual_width << "x" << actual_height << " instead." << std::endl;
            this->width = actual_width / frameWidthScale;
            this->height = actual_height;
        }
    }

    void StereoCapture::cap_init_(const std::shared_ptr<cv::VideoCapture> &cap, const std::string &source, int width, int height, int frameWidthScale) {
        if (!cap->isOpened()) {
            throw std::runtime_error("Failed to open video sources: " + source);
        }
        uint32_t frameWidth = width * frameWidthScale;
        std::cout << "Info: set resolution to" << frameWidth << "x" << height << " instead." << std::endl;
        cap->set(cv::CAP_PROP_FRAME_WIDTH, frameWidth);
        cap->set(cv::CAP_PROP_FRAME_HEIGHT, height);
        this->width = width;
        this->height = height;

        // Check resolution status
        int actual_width = static_cast<int>(cap->get(cv::CAP_PROP_FRAME_WIDTH));
        int actual_height = static_cast<int>(cap->get(cv::CAP_PROP_FRAME_HEIGHT));
        if (actual_width != frameWidth || actual_height != height) {
            std::cerr << "Warning: Failed to set resolution. Using "
                      << actual_width << "x" << actual_height << " instead." << std::endl;
            this->width = actual_width / frameWidthScale;
            this->height = actual_height;
        }
    }
    StereoCapture::StereoCapture(int source, int width, int height) {
        cap_left_ = std::make_shared<cv::VideoCapture>(source);
        cap_init_(cap_left_, source, width, height, 2);
        cap_right_ = nullptr;
    }

    StereoCapture::StereoCapture(const std::string &source, int width, int height) {
        cap_init_(cap_left_, source, width, height, 2);
        cap_right_ = nullptr;
    }

    StereoCapture::StereoCapture(int source_left, int source_right, int width, int height) {
        setupCapLeft(source_left, width, height);
        setupCapRight(source_right, width, height);
    }

    StereoCapture::StereoCapture(const std::string &source_left, const std::string &source_right, int width, int height) {
        setupCapLeft(source_left, width, height);
        setupCapRight(source_right, width, height);
    }

    StereoCapture::~StereoCapture() {
        if (cap_left_ != nullptr) {
            cap_left_->release();
        }
        if (cap_right_ != nullptr) {
            cap_right_->release();
        }
    }

    StereoCapture::CaptureFrameState_ StereoCapture::captureStereoFrame(cv::Mat &frame_left, cv::Mat &frame_right) const {
        cv::Mat frame, left, right;

        // Check if the left camera is valid
        if (cap_left_ == nullptr) {
            return CaptureFrameState_::NoCapture;
        }

        // Read the frame from the left camera
        *cap_left_ >> frame;
        if (frame.empty()) {
            return CaptureFrameState_::NoFrame;
        }

        if (frame.cols == 2 * this->width) {
            frame_left = frame(cv::Rect(0, 0, this->width, this->height));
            frame_right = frame(cv::Rect(this->width, 0, this->width, this->height));
            return CaptureFrameState_::HasRightFrame;
        }

        frame_left = frame;
        if (cap_right_ != nullptr) {
            *cap_right_ >> frame_right;
            if (!frame_right.empty()) {
                return CaptureFrameState_::HasRightFrame;
            }
        }

        return CaptureFrameState_::HasLeftFrame;
    }

    void StereoCapture::setupCapLeft(int source, int width, int height, int frameWidthScale) {
        cap_left_ = std::make_shared<cv::VideoCapture>(source);
        cap_init_(cap_left_, source, width, height, frameWidthScale);
    }
    void StereoCapture::setupCapLeft(const std::string &source, int width, int height, int frameWidthScale){
        cap_left_ = std::make_shared<cv::VideoCapture>(source);
        cap_init_(cap_left_, source, width, height, frameWidthScale);
    }
    void StereoCapture::setupCapRight(int source, int width, int height, int frameWidthScale){
        cap_right_ = std::make_shared<cv::VideoCapture>(source);
        cap_init_(cap_right_, source, width, height, frameWidthScale);
    }
    void StereoCapture::setupCapRight(const std::string &source, int width, int height, int frameWidthScale){
        cap_right_ = std::make_shared<cv::VideoCapture>(source);
        cap_init_(cap_right_, source, width, height, frameWidthScale);
    }
}
