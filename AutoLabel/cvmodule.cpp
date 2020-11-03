#include "cvmodule.h"

#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

CVModule::CVModule(QObject *parent) : QObject(parent)
{

}

LabelCollector *CVModule::labelCollector() const
{
    return m_labelCollector;
}

void CVModule::GetCroppedImg(int labelIdx)
{
    QVector<LabelData*> dataVecTmp = labelCollector()->dataVec();
    qreal factorScaled = labelCollector()->getFactorScaled();
    cv::Rect rectOri = cv::Rect(dataVecTmp.at(labelIdx)->rect.tl() * factorScaled,
                                dataVecTmp.at(labelIdx)->rect.br() * factorScaled);
    cv::Mat croppedImg = m_imgOri(rectOri);
    cv::namedWindow("Cropped");
    cv::imshow("Cropped", croppedImg);

}

void CVModule::GetContour(int labelIdx)
{
    qDebug()<< Q_FUNC_INFO << "start";
    // grabcut
    cv::Mat grab, bg, fg;
    cv::grabCut(m_imgOri, grab, GetROIRect(labelIdx), bg, fg, 5, cv::GC_INIT_WITH_RECT);

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
    labelCollector()->SetContours(labelIdx,contoursPoly.at(largestContourIndex));
    qDebug()<< Q_FUNC_INFO << "end";
}

cv::Rect CVModule::GetROIRect(int labelIdx)
{
    QVector<LabelData*> dataVecTmp = labelCollector()->dataVec();
    qreal factorScaled = labelCollector()->getFactorScaled();
    cv::Rect rectOri = cv::Rect(dataVecTmp.at(labelIdx)->rect.tl() * factorScaled,
                                dataVecTmp.at(labelIdx)->rect.br() * factorScaled);
    return rectOri;
}

void CVModule::GetOriginImg(QString imgSrc)
{
    qDebug()<< Q_FUNC_INFO << "imgSrc" << imgSrc;
    m_imgOri = cv::imread(imgSrc.toStdString(),1);
}

void CVModule::setLabelCollector(LabelCollector *labelCollector)
{
    qDebug()<< Q_FUNC_INFO << "Start";
    if (m_labelCollector == labelCollector)
        return;

    m_labelCollector = labelCollector;
    emit labelCollectorChanged(m_labelCollector);
    QObject::connect(m_labelCollector, &LabelCollector::imgSrcChanged, this, &CVModule::GetOriginImg);
    QObject::connect(m_labelCollector, &LabelCollector::processRequest, this, &CVModule::GetContour);
}
