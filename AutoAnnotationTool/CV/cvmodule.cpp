#include "cvmodule.h"
#include "opencvtypeconverter.h"

#include <QPolygon>
#include <QDebug>

#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>


CvModule::CvModule()
{

}

void CvModule::getCroppedImg(QRectF rect, qreal factor)
{
    cv::Rect rectOri = OpencvTypeConverter::qrect2CvRect(QRectF(rect.topLeft() * factor,
                                                                rect.bottomRight() * factor));
    cv::Mat croppedImg = m_originImg(rectOri);
    cv::namedWindow("Cropped");
    cv::imshow("Cropped", croppedImg);
}

void CvModule::getPoly(QVector<LabelData *> &dataVec, int labelIdx, qreal factor, CvParam *param)
{
    qDebug()<< Q_FUNC_INFO << "start";
    if(m_originImg.empty()) return;

    // grabcut
    cv::Mat grab, bg, fg;
    cv::grabCut(m_originImg, grab, getRoiRect(dataVec.at(labelIdx)->rect, factor), bg, fg, param->iteration(), cv::GC_INIT_WITH_RECT);

    // equalizeHist
    equalizeHist(grab, grab);

    // binary
    cv::Mat gray(grab.size(), CV_8U, cv::Scalar(255));
    threshold(grab, gray, 200, 256, cv::THRESH_BINARY);//convert to binary

    std::vector<std::vector<cv::Point>> contours;
    cv::findContours(gray,
                     contours, // a vector of contours
                     cv::RETR_EXTERNAL, // retrieve the external contours
                     cv::CHAIN_APPROX_NONE); // retrieve all pixels of each contours

    // Print contours' length
    qDebug() << "Contours: " << contours.size();

    // find max contour area
    int largestArea = 0;
    int largestContourIndex = 0;
    for (int i = 0; i < contours.size(); i++) // Iterate through each contour
    {
        double area = contourArea(contours[i], false); // Find the area of contour
        if (area > largestArea) {
            largestArea = area;
            largestContourIndex = i; // Store the index of largest contour
        }
    }
    std::vector<std::vector<cv::Point>> contoursPoly(contours.size());
    approxPolyDP(cv::Mat(contours[largestContourIndex]), contoursPoly[largestContourIndex], param->epsilon(), true);
    setPoly(dataVec, labelIdx, contoursPoly.at(largestContourIndex), factor);
    qDebug()<< Q_FUNC_INFO << "end";
}

void CvModule::setPoly(QVector<LabelData*> &dataVec, int labelIdx, std::vector<cv::Point> &contoursPoly, qreal factor){
    qDebug()<< Q_FUNC_INFO << "start";
    QPolygon tmpPoly;

    for(int i =0;i< contoursPoly.size();++i){
        cv::Point tmp = contoursPoly[i];
        QPoint resultPoint = QPoint(tmp.x*(1.0f/factor),tmp.y*(1.0f/factor));
        tmpPoly.push_back(resultPoint);
    }
    dataVec[labelIdx]->poly = tmpPoly;
    qDebug()<< Q_FUNC_INFO << "end";
}

void CvModule::getOriginImg(QString imgSrc){
    m_originImg = cv::imread(imgSrc.toStdString(),1);
}

cv::Rect CvModule::getRoiRect(QRectF rect, qreal factor){
    QRectF rectOri(rect.topLeft() * factor,
                   rect.bottomRight() * factor);
    return OpencvTypeConverter::qrect2CvRect(rectOri);
}
