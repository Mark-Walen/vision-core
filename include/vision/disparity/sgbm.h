//
// Created by Mark-Walen on 2024/12/25.
//

#ifndef VISION_DISPARITY_SGBM_H
#define VISION_DISPARITY_SGBM_H

#include <memory>
#include <vector>

namespace cv {
    class Mat;
    class StereoSGBM;
}

namespace YAML {
    class Node;
}

namespace vlue::processing {
    class Pipeline;
    using PipelinePtr = std::shared_ptr<Pipeline>;
}

namespace vlue::disparity {
    class StereoSGBM {
    private:
        std::vector<processing::PipelinePtr> m_Preprocess, m_PostProcess;
        std::shared_ptr<cv::StereoSGBM> m_MatcherLeft, m_MatcherRight;
    public:
        enum
        {
            MODE_SGBM = 0,
            MODE_HH   = 1,
            MODE_SGBM_3WAY = 2,
            MODE_HH4  = 3
        };
        explicit StereoSGBM(const std::string &disparity_param_path);
        explicit StereoSGBM(const YAML::Node &disparity_config);
        explicit StereoSGBM(int minDisparity=0, int numDisparities=16, int blockSize=3, int P1=0, int P2=0, int disp12MaxDiff=0, int preFilterCap=0, int uniquenessRatio=0, int speckleWindowSize=0, int speckleRange=0, int mode=MODE_SGBM);

        ~StereoSGBM() = default;

        void registerPreprocessPipeline(const processing::PipelinePtr &pipeline);
        void registerPostprocessPipeline(const processing::PipelinePtr &pipeline);
        void computeDisparity(const cv::Mat &left, const cv::Mat& right, cv::Mat &leftDisparity, cv::Mat &rightDisparity, bool computeRight=false) const;

    private:
        void preprocess(cv::Mat &left, cv::Mat &right) const;

        void postprocess(cv::Mat &leftDisparity, const cv::Mat &leftView, cv::Mat &rightDisparity, const cv::Mat &rightView, bool filterRight=false) const;
    };
}

#endif //SGBM_H
