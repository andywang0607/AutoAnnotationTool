#ifndef CVMODULE_H
#define CVMODULE_H
#include "labeldata.h"

#include <QRectF>
#include <QString>
#include <QVector>
#include <QPointF>

#include "opencv2/core.hpp"

class CVModule
{
public:
    CVModule();
    void GetCroppedImg(QRectF rect, qreal factor);
    void GetContour(QVector<LabelData*> &dataVec, int labelIdx, qreal factor);
    void GetOriginImg(QString imgSrc);
private:
    cv::Rect GetROIRect(QRectF rect, qreal factor);
    void SetContours(QVector<LabelData*> &dataVec, int labelIdx, std::vector<cv::Point> &contoursPoly, qreal factor);
private:
    cv::Mat m_imgOri;
};

#endif // CVMODULE_H
