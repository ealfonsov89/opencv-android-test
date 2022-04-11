#include "detection.h"

#include <opencv2/dnn/dnn.hpp>
#include <opencv2/imgproc/imgproc.hpp>

float binThresh = 0.3;
float polyThresh = 0.5;
uint maxCandidates = 200;
double unclipRatio = 2.0;
double scale = 1.0 / 255.0;
cv::Scalar mean = cv::Scalar(122.67891434, 116.66876762, 104.00698793);
cv::Size inputSize = cv::Size(736, 736);

Detection::Detection(bool imreadRGB)
{
    this->imreadRGB = imreadRGB;
    this->ModelDB = cv::dnn::TextDetectionModel_DB("C:/Users/ealfo/Downloads/DB_TD500_resnet50.onnx");
    this->PreparePostProcessingParameters();
    this->ModelDB.setInputParams(scale, inputSize, mean);
}




void Detection::PreparePostProcessingParameters()
{
    this->ModelDB.setBinaryThreshold(binThresh)
         .setPolygonThreshold(polyThresh)
         .setMaxCandidates(maxCandidates)
         .setUnclipRatio(unclipRatio)
    ;
}


void Detection::setImage(const cv::Mat &newImage)
{
    this->image = newImage;
    ModelDB.detect(this->image, this->detResults);

    cv::Mat recInput;
    if (!imreadRGB) {
        cvtColor(this->image, recInput, cv::COLOR_BGR2GRAY);
    } else {
        recInput = this->image;
    }

    std::vector< std::vector<cv::Point> > contours;
    for (uint i = 0; i < detResults.size(); i++)
    {
        const auto& quadrangle = detResults[i];
        CV_CheckEQ(quadrangle.size(), (size_t)4, "");

        contours.emplace_back(quadrangle);

        std::vector<cv::Point2f> quadrangle_2f;
        for (int j = 0; j < 4; j++)
            quadrangle_2f.emplace_back(quadrangle[j]);

        cv::Mat cropped;
        fourPointsTransform(recInput, &quadrangle_2f[0], cropped);

        this->cropedText.append(cropped);
    }

    // Visualization
    cv::polylines(this->image, this->detResults, true, cv::Scalar(0, 255, 0), 2);
}


void Detection::fourPointsTransform(const cv::Mat& frame, const cv::Point2f vertices[], cv::Mat& result)
{
    const cv::Size outputSize = cv::Size(100, 32);

    cv::Point2f targetVertices[4] = {
        cv::Point(0, outputSize.height - 1),
        cv::Point(0, 0), cv::Point(outputSize.width - 1, 0),
        cv::Point(outputSize.width - 1, outputSize.height - 1)
    };
    cv::Mat rotationMatrix = getPerspectiveTransform(vertices, targetVertices);

    warpPerspective(frame, result, rotationMatrix, outputSize);
}

const QList<cv::Mat> &Detection::getCropedText() const
{
    return cropedText;
}

const cv::Mat &Detection::getImage() const
{
    return this->image;
}
