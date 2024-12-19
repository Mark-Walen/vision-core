//
// Created by Mark-Walen on 2024/12/17.
//

#ifndef STEREO_CAPTURE_H
#define STEREO_CAPTURE_H

#include <memory>
#include <string>
#include <opencv2/core/mat.hpp>

namespace cv {
    class VideoCapture;
    class Mat;
}

namespace vlue::capture {
    class StereoCapture {
    public:
        enum class CaptureFrameState_ {
            NoCapture,
            NoFrame,
            HasLeftFrame,
            HasRightFrame,
        };

        // Reference to the shared pointer for the left camera capture object.
        std::shared_ptr<cv::VideoCapture> &cap_left = cap_left_;
        std::shared_ptr<cv::VideoCapture> &cap_right = cap_right_;
        int width = 640;
        int height = 480;

    public:
        StereoCapture(const std::string &source, int width, int height);

        StereoCapture(int source, int width, int height);

        // two source id case
        StereoCapture(const std::string &source_left, const std::string &source_right, int width, int height);

        StereoCapture(int source_left, int source_right, int width, int height);

        ~StereoCapture();

        CaptureFrameState_ captureStereoFrame(cv::Mat &frame_left, cv::Mat &frame_right) const;
        void setupCapLeft(int source, int width = 640, int height = 480, int frameWidthScale=1);
        void setupCapLeft(const std::string &source, int width = 640, int height = 480, int frameWidthScale=1);
        void setupCapRight(int source, int width = 640, int height = 480, int frameWidthScale=1);
        void setupCapRight(const std::string &source, int width = 640, int height = 480, int frameWidthScale=1);

    private:
        // Shared pointers to video capture objects for left and right cameras.
        std::shared_ptr<cv::VideoCapture> cap_left_;
        std::shared_ptr<cv::VideoCapture> cap_right_;
        void cap_init_(const std::shared_ptr<cv::VideoCapture> &cap, int source, int width, int height, int frameWidthScale=1);
        void cap_init_(const std::shared_ptr<cv::VideoCapture> &cap, const std::string &source, int width, int height, int frameWidthScale=1);
    };
    using CaptureFrameState = StereoCapture::CaptureFrameState_;
}

#endif //STEREO_CAPTURE_H
