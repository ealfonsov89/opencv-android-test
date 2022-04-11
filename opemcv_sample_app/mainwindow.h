#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/dnn/dnn.hpp>

#include "recognition.h"
#include "detection.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    QImage Mat2QImage(cv::Mat const& src);
    cv::Mat QImage2Mat(QImage const& src);
    cv::Mat MonoCromaticMat(cv::Mat const& src);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    Recognition *recognitionModel;
    Detection *detectionModel;
};
#endif // MAINWINDOW_H
