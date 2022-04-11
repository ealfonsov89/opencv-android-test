#ifndef DETECTION_H
#define DETECTION_H

#include <opencv2/dnn/dnn.hpp>
#include <QList>


class Detection
{
public:
    Detection(bool imreadRGB);
    const cv::Mat &getImage() const;
    void setImage(const cv::Mat &newImage);
    const QList<cv::Mat> &getCropedText() const;

private:
    cv::dnn::TextDetectionModel_DB ModelDB;
    cv::dnn::TextDetectionModel_EAST ModelEAST;
    std::vector<std::vector<cv::Point>> detResults;
    cv::Mat image;
    QList<cv::Mat> cropedText;
    bool imreadRGB;

    void PreparePostProcessingParameters();
    void fourPointsTransform(const cv::Mat& frame, const cv::Point2f vertices[], cv::Mat& result);
};

#endif // DETECTION_H
