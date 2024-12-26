//
// Created by Mark-Walen on 2024/12/25.
//

#include "vision/disparity/sgbm.h"
#include "vision/pipeline/pipeline.h"
#include "vision/helpers/yaml.h"

#include <opencv2/ximgproc/disparity_filter.hpp>

using namespace vlue::utils;
using namespace vlue::processing;

namespace vlue::disparity {
    StereoSGBM::StereoSGBM(const std::string &disparity_param_path) : StereoSGBM(YAMLUtils::loadYamlConfig(disparity_param_path)) {}

    StereoSGBM::StereoSGBM(const YAML::Node &disparity_config) {
        int minDisparity = disparity_config["minDisparity"].as<int>(0);
        int numDisparities = disparity_config["numDisparities"].as<int>(16);
        int blockSize = disparity_config["blockSize"].as<int>(3);
        int P1 = disparity_config["P1"].as<int>(0);
        int P2 = disparity_config["P2"].as<int>(0);
        int disp12MaxDiff = disparity_config["disp12MaxDiff"].as<int>(0);
        int preFilterCap = disparity_config["preFilterCap"].as<int>(0);
        int uniquenessRatio = disparity_config["uniquenessRatio"].as<int>(0);
        int speckleWindowSize = disparity_config["speckleWindowSize"].as<int>(0);
        int speckleRange = disparity_config["speckleRange"].as<int>(0);
        int mode = disparity_config["mode"].as<int>(MODE_SGBM);

        m_MatcherLeft = cv::StereoSGBM::create(minDisparity, numDisparities, blockSize, P1, P2, disp12MaxDiff, preFilterCap, uniquenessRatio, speckleWindowSize, speckleRange, mode);
        m_MatcherRight = cv::ximgproc::createRightMatcher(cv::Ptr(m_MatcherLeft)).dynamicCast<cv::StereoSGBM>();
    }

    StereoSGBM::StereoSGBM(int minDisparity, int numDisparities, int blockSize, int P1, int P2, int disp12MaxDiff, int preFilterCap, int uniquenessRatio, int speckleWindowSize, int speckleRange, int mode) {
        m_MatcherLeft = cv::StereoSGBM::create(minDisparity, numDisparities, blockSize, P1, P2, disp12MaxDiff, preFilterCap, uniquenessRatio, speckleWindowSize, speckleRange, mode);
        m_MatcherRight = cv::ximgproc::createRightMatcher(cv::Ptr(m_MatcherLeft)).dynamicCast<cv::StereoSGBM>();
    }

    void StereoSGBM::registerPreprocessPipeline(const PipelinePtr &pipeline) {
        m_Preprocess.push_back(pipeline);
    }

    void StereoSGBM::registerPostprocessPipeline(const PipelinePtr &pipeline) {
        m_PostProcess.push_back(pipeline);
    }

    void StereoSGBM::computeDisparity(const cv::Mat &left, const cv::Mat& right, cv::Mat &leftDisparity, cv::Mat &rightDisparity, bool computeRight) const {
        cv::Mat m_Left = left.clone();
        cv::Mat m_Right = right.clone();
        if (left.empty() || right.empty()) {
            throw std::invalid_argument("Input images must not be empty.");
        }
        if (left.size() != right.size()) {
            throw std::invalid_argument("Left and right images must have the same size. Got left: "
                                         + std::to_string(left.rows) + "x" + std::to_string(left.cols)
                                         + ", right: " + std::to_string(right.rows) + "x" + std::to_string(right.cols));
        }
        preprocess(m_Left, m_Right);
        m_MatcherLeft->compute(m_Left, m_Right, leftDisparity);
        if (!right.empty()) {
            m_MatcherRight->compute(m_Right, m_Left, rightDisparity);
        }
    }

    void StereoSGBM::preprocess(cv::Mat &left, cv::Mat &right) const {
        for (const auto &pipeline : m_Preprocess) {
            left = pipeline->process(left);
            right = pipeline->process(right);
        }
    }

    void StereoSGBM::postprocess(cv::Mat &leftDisparity, const cv::Mat &leftView, cv::Mat &rightDisparity,
        const cv::Mat &rightView, bool filterRight) const {
        for (const auto &pipeline : m_PostProcess) {
            if (pipeline->getType() != PipelineType::DisparityMap) {
                continue;
            }
            if (auto m_Pipeline = std::dynamic_pointer_cast<DisparityFilterPipeline>(pipeline)) { // Check successful cast
                try {
                    leftDisparity = m_Pipeline->process(leftDisparity, leftView, leftView, rightView);
                    if (filterRight) {
                        rightDisparity = m_Pipeline->process(rightDisparity, rightView, leftDisparity, leftView);
                    }
                } catch (const std::exception &e) {
                    std::cerr << "Error in disparity processing pipeline: " << e.what() << std::endl;
                }
            } else {
                std::cerr << "Pipeline is not a valid DisparityFilterPipeline!" << std::endl;
            }
        }
    }
}
