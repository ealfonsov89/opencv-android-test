#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFileDialog>
#include <QImage>

#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/dnn/dnn.hpp>
#include <opencv2/imgcodecs/imgcodecs.hpp>

#include <string>
#include "recognition.h"
#include "detection.h"

int lowThreshold = 0;


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    this->recognitionModel = new Recognition();
    this->detectionModel = new Detection(false);

    ui->setupUi(this);

    QString fileName = QFileDialog::getOpenFileName(
        this,
        tr("Open Image"),
        "",
        tr("Image Files (*.png *.jpg *.bmp)")
    );

    cv::Mat src = cv::imread(fileName.toStdString(), cv::IMREAD_COLOR);
    this->detectionModel->setImage(MonoCromaticMat(src));

    QList<cv::Mat> cropedText = this->detectionModel->getCropedText();
    cv::Mat detectionImage = this->detectionModel->getImage();

    QImage image(MainWindow::Mat2QImage(detectionImage));

    QPixmap pixmap = QPixmap::fromImage(image);
    ui->label->setPixmap(pixmap);
    ui->label->setScaledContents( true );
    ui->label->setSizePolicy( QSizePolicy::Ignored, QSizePolicy::Ignored );

    QString recognicedText;
    cv::Mat crop;
    foreach(crop, cropedText)
    {
        recognicedText.append(this->recognitionModel->GetTextFromMat(crop));
    }



    ui->textDetected->setText(recognicedText);
}

QImage MainWindow::Mat2QImage(cv::Mat const& src)
{
     cv::Mat temp; // make the same cv::Mat
     cvtColor(src, temp,cv::COLOR_BGR2RGB); // cvtColor Makes a copt, that what i need
     QImage dest((const uchar *) temp.data, temp.cols, temp.rows, temp.step, QImage::Format_RGB888);
     dest.bits(); // enforce deep copy, see documentation
     // of QImage::QImage ( const uchar * data, int width, int height, Format format )
     return dest;
}

cv::Mat MainWindow::QImage2Mat(QImage const& src)
{
     cv::Mat tmp(src.height(),src.width(),CV_8UC3,(uchar*)src.bits(),src.bytesPerLine());
     cv::Mat result; // deep copy just in case (my lack of knowledge with open cv)
     cvtColor(tmp, result,cv::COLOR_BGR2RGB);
     return result;
}
cv::Mat MainWindow::MonoCromaticMat(cv::Mat const& src)
{
     cv::Mat result; // deep copy just in case (my lack of knowledge with open cv)
     cvtColor(src, result,cv::COLOR_BGR2GRAY);
     return result;
}

MainWindow::~MainWindow()
{
    delete ui;
}

