#ifndef CVMODULE_H
#define CVMODULE_H

#include "include/LableData.h"
#include "QItem/CvParam.h"

#include <QRectF>
#include <QString>
#include <QVector>
#include <QPointF>

#include "opencv2/core.hpp"

class CvModule
{
public:
    CvModule();
    void getCroppedImg(QRectF rect, qreal factor);
    void getPoly(QVector<LabelData*> &dataVec, int labelIdx, qreal factor, CvParam *param);
    void getOriginImg(QString imgSrc);
private:
    cv::Rect getRoiRect(QRectF rect, qreal factor);
    void setPoly(QVector<LabelData*> &dataVec, int labelIdx, std::vector<cv::Point> &contoursPoly, qreal factor);
private:
    cv::Mat m_originImg;
};

#endif // CVMODULE_H
