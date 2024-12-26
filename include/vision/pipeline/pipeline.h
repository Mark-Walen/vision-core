//
// Created by Mark-Walen on 2024/12/26.
//

#ifndef PIPELINE_H
#define PIPELINE_H

#include <memory>

namespace cv {
    namespace ximgproc {
        class DisparityFilter;
        class DisparityWLSFilter;
    }
    class Mat;
}

namespace vlue::processing {
    class Pipeline {
    public:
        enum class Type {
            Image,
            DisparityMap,
        };
        Pipeline() = default; // Default constructor
        Pipeline(const Pipeline&) = default; // Copy constructor
        Pipeline(Pipeline&&) = default; // Move constructor
        Pipeline& operator=(const Pipeline&) = default; // Copy assignment
        Pipeline& operator=(Pipeline&&) = default; // Move assignment
        virtual ~Pipeline() = default; // Destructor

        [[nodiscard]] bool isEnabled() const { return m_Enabled; }
        void setEnabled(bool enabled) { m_Enabled = enabled; }
        [[nodiscard]] Type getType() const { return m_Type; }

        [[nodiscard]] virtual cv::Mat process(const cv::Mat &inputImage) const = 0;

    protected:
        bool m_Enabled = true;
        Type m_Type = Type::Image;
    };

    class DisparityFilterPipeline : public Pipeline {
    public:
        DisparityFilterPipeline() = default;
        DisparityFilterPipeline(const DisparityFilterPipeline&) = default;
        DisparityFilterPipeline(DisparityFilterPipeline&&) = default;
        DisparityFilterPipeline& operator=(const DisparityFilterPipeline&) = default;
        DisparityFilterPipeline& operator=(DisparityFilterPipeline&&) = default;

        ~DisparityFilterPipeline() override = default;

        [[nodiscard]] cv::Mat process(const cv::Mat &inputImage) const override;
        [[nodiscard]] cv::Mat process(const cv::Mat &leftDisparity, [[maybe_unused]] const cv::Mat &leftView, [[maybe_unused]] const cv::Mat &rightDisparity, [[maybe_unused]] const cv::Mat &rightView) const;

    protected:
        [[nodiscard]] virtual cv::Mat filter_(const cv::Mat &leftDisparity, [[maybe_unused]] const cv::Mat &leftView, [[maybe_unused]] const cv::Mat &rightDisparity, [[maybe_unused]] const cv::Mat &rightView) const = 0;

    protected:
        Type m_Type = Type::DisparityMap;
    };

    class DisparityWLSFilterPipeline : public DisparityFilterPipeline {
    private:
        std::shared_ptr<cv::ximgproc::DisparityWLSFilter> m_Filter;
    public:
        DisparityWLSFilterPipeline(double lambda, double sigmaColor, int LRCThresh, int discRadius, bool enable = true);

    protected:
        [[nodiscard]] cv::Mat filter_(const cv::Mat &leftDisparity, const cv::Mat &leftView, const cv::Mat &rightDisparity,
                        const cv::Mat &rightView) const override;
    };

    using PipelineType = Pipeline::Type;
}

#endif //PIPELINE_H
