//
// Created by Mark-Walen on 2024/12/26.
//
#include "vision/pipeline/pipeline.h"
#include <opencv2/ximgproc/disparity_filter.hpp>

namespace vlue::processing {
    cv::Mat DisparityFilterPipeline::process(const cv::Mat &inputImage) const {
        return inputImage.clone();
    }

    cv::Mat DisparityFilterPipeline::process(const cv::Mat &leftDisparity, const cv::Mat &leftView,
                                             const cv::Mat &rightDisparity, const cv::Mat &rightView) const {
        if (!isEnabled())
            return leftDisparity;
        return filter_(leftDisparity, leftView, rightDisparity, rightView);
    }

    DisparityWLSFilterPipeline::DisparityWLSFilterPipeline(double lambda, double sigmaColor, int LRCThresh, int discRadius, bool enable) {
        m_Filter = cv::ximgproc::createDisparityWLSFilterGeneric(true);
        m_Filter->setLambda(lambda);
        m_Filter->setSigmaColor(sigmaColor);
        m_Filter->setLRCthresh(LRCThresh);
        m_Filter->setDepthDiscontinuityRadius(discRadius);
        m_Enabled = enable;
    }

    cv::Mat DisparityWLSFilterPipeline::filter_(const cv::Mat &leftDisparity, const cv::Mat &leftView,
        const cv::Mat &rightDisparity, const cv::Mat &rightView) const {
        cv::Mat filteredDisparityMap;
        m_Filter->filter(leftDisparity, leftView, filteredDisparityMap, rightDisparity, cv::Rect(), rightView);

        return filteredDisparityMap;
    }
}
