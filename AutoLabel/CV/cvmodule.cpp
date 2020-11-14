#include "cvmodule.h"
#include "opencvtypeconverter.h"

#include <QPolygon>
#include <QDebug>

#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>


CVModule::CVModule()
{

}

void CVModule::GetCroppedImg(QRectF rect, qreal factor)
{
    cv::Rect rectOri = OpenCVTypeConverter::QRect2CVRect(QRectF(rect.topLeft() * factor,
                                                                rect.bottomRight() * factor));
    cv::Mat croppedImg = m_imgOri(rectOri);
    cv::namedWindow("Cropped");
    cv::imshow("Cropped", croppedImg);
}

void CVModule::GetContour(QVector<LabelData *> &dataVec, int labelIdx, qreal factor)
{
    qDebug()<< Q_FUNC_INFO << "start";
    if(m_imgOri.empty()) return;

    // grabcut
    cv::Mat grab, bg, fg;
    cv::grabCut(m_imgOri, grab, GetROIRect(dataVec.at(labelIdx)->rect, factor), bg, fg, 5, cv::GC_INIT_WITH_RECT);

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
    approxPolyDP(cv::Mat(contours[largestContourIndex]), contoursPoly[largestContourIndex], 1, true);
    SetContours(dataVec, labelIdx, contoursPoly.at(largestContourIndex), factor);
    qDebug()<< Q_FUNC_INFO << "end";
}

void CVModule::SetContours(QVector<LabelData*> &dataVec, int labelIdx, std::vector<cv::Point> &contoursPoly, qreal factor){
    qDebug()<< Q_FUNC_INFO << "start";
    QPolygon tmpPoly;
    dataVec[labelIdx]->contoursPoly.assign(contoursPoly.begin(),contoursPoly.end());
    dataVec[labelIdx]->result.resize(dataVec[labelIdx]->contoursPoly.size());
    for(int i =0;i< dataVec[labelIdx]->contoursPoly.size();++i){
        cv::Point tmp = dataVec[labelIdx]->contoursPoly[i];
        QPoint resultPoint = QPoint(tmp.x*(1.0f/factor),tmp.y*(1.0f/factor));
        dataVec[labelIdx]->result[i] = resultPoint;
        tmpPoly.push_back(resultPoint);
    }
    dataVec[labelIdx]->resultPoly = tmpPoly;
    qDebug()<< Q_FUNC_INFO << "end";
}

void CVModule::GetOriginImg(QString imgSrc){
    m_imgOri = cv::imread(imgSrc.toStdString(),1);
}

cv::Rect CVModule::GetROIRect(QRectF rect, qreal factor){
    QRectF rectOri(rect.topLeft() * factor,
                   rect.bottomRight() * factor);
    return OpenCVTypeConverter::QRect2CVRect(rectOri);
}
