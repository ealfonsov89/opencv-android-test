#include "recognition.h"
#include <QImage>
#include <QFile>
#include <QTextStream>

Recognition::Recognition()
{
    Model = GetOcrModel();
    SetOcrSettings(Model);
}
cv::dnn::TextRecognitionModel Recognition::GetOcrModel()
{
    cv::dnn::TextRecognitionModel model("C:/Users/ealfo/Downloads/DenseNet_CTC.onnx");
    model.setDecodeType("CTC-greedy");
    std::vector<std::string> vocabulary;

    QFile inputFile("C:/Users/ealfo/Downloads/alphabet_36.txt");
    if (inputFile.open(QIODevice::ReadOnly))
    {
       QTextStream in(&inputFile);
       while (!in.atEnd())
       {
          QString line = in.readLine();
          vocabulary.push_back(line.toStdString());

       }
       inputFile.close();
    }
    model.setVocabulary(vocabulary);
    return model;
}

void Recognition::SetOcrSettings(cv::dnn::TextRecognitionModel &model)
{
    // Normalization parameters
    double scale = 1.0 / 127.5;
    cv::Scalar mean = cv::Scalar(127.5, 127.5, 127.5);
    // The input shape
    cv::Size inputSize = cv::Size(100, 32);
    model.setInputParams(scale, inputSize, mean);
}

QString Recognition::GetTextFromMat(cv::Mat image) {
    cv::dnn::TextRecognitionModel model = GetOcrModel();
    SetOcrSettings(model);
    std::string recognitionResult = model.recognize(image);
    return QString::fromStdString(recognitionResult);
}
