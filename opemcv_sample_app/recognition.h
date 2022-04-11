#ifndef RECOGNITION_H
#define RECOGNITION_H

#include <opencv2/dnn/dnn.hpp>
#include <QString>

class Recognition
{
public:
    Recognition();
    QString GetTextFromMat(cv::Mat image);
private:
    cv::dnn::TextRecognitionModel Model;

    cv::dnn::TextRecognitionModel GetOcrModel();
    void SetOcrSettings(cv::dnn::TextRecognitionModel &model);
};

#endif // RECOGNITION_H
