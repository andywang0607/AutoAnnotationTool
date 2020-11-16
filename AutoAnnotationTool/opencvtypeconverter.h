#ifndef OPENCVTYPECONVERTER_H
#define OPENCVTYPECONVERTER_H

#include <QRect>
#include <QPoint>

#include <opencv2/core.hpp>

class OpenCVTypeConverter
{
public:
    OpenCVTypeConverter();
    static cv::Rect QRect2CVRect(QRectF rect);
    static QRectF CVRect2QRect(cv::Rect rect);
    static cv::Point QPoint2CVPoint(QPoint pt);
    static QPoint CVPoint2QPoint(cv::Point pt);
};

#endif // OPENCVTYPECONVERTER_H
