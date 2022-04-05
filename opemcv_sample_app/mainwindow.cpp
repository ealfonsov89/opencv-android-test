#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFileDialog>
#include <QImage>

#include <opencv2/imgproc/imgproc.hpp>


int lowThreshold = 0;

const int ratio = 3;
const int kernel_size = 3;

cv::Mat MainWindow::CannyThreshold(cv::Mat src)
{
    cv::Mat src_gray;
    cv::Mat dst;
    cv::Mat detected_edges;
    cvtColor((cv::InputArray)src, (cv::OutputArray)src_gray, cv::COLOR_BGR2GRAY);
    dst.create(src.size(), src.type());
    cv::blur(src_gray, detected_edges, cv::Size(3, 3));
    cv::Canny((cv::InputArray)detected_edges, (cv::OutputArray)detected_edges, lowThreshold, lowThreshold * ratio, kernel_size);
    dst = cv::Scalar::all(0);
    src.copyTo((cv::OutputArray)dst, (cv::InputArray)detected_edges);
    return dst;
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QString fileName = QFileDialog::getOpenFileName(this,
         tr("Open Image"), "", tr("Image Files (*.png *.jpg *.bmp)"));

    QImage image(fileName);
    image.convertTo(QImage::Format_BGR888);
    cv::Mat src(image.height(), image.width(), CV_8UC3, (cv::Scalar*)image.scanLine(0));
    cv::Mat cannyMat = CannyThreshold(src);
    QImage cannyImage(cannyMat.data, cannyMat.cols, cannyMat.rows, QImage::Format_BGR888);

    QPixmap pixmap = QPixmap::fromImage(cannyImage);
    ui->label->setPixmap(pixmap);
    ui->label->setScaledContents( true );
    ui->label->setSizePolicy( QSizePolicy::Ignored, QSizePolicy::Ignored );
}

MainWindow::~MainWindow()
{
    delete ui;
}

