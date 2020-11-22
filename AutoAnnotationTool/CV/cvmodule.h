#ifndef CVMODULE_H
#define CVMODULE_H
#include "labeldata.h"
#include "CV/cvparam.h"

#include <QRectF>
#include <QString>
#include <QVector>
#include <QPointF>

#include "opencv2/core.hpp"

class CvModule
{
public:
    CvModule();
    void GetCroppedImg(QRectF rect, qreal factor);
    void GetContour(QVector<LabelData*> &dataVec, int labelIdx, qreal factor, CvParam *param);
    void GetOriginImg(QString imgSrc);
private:
    cv::Rect GetROIRect(QRectF rect, qreal factor);
    void SetContours(QVector<LabelData*> &dataVec, int labelIdx, std::vector<cv::Point> &contoursPoly, qreal factor);
private:
    cv::Mat m_imgOri;
};

#endif // CVMODULE_H
