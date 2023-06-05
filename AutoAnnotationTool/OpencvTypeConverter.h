#ifndef OPENCVTYPECONVERTER_H
#define OPENCVTYPECONVERTER_H

#include <QRect>
#include <QPoint>

#include <opencv2/core.hpp>

class OpencvTypeConverter
{
public:
    OpencvTypeConverter();
    static cv::Rect qrect2CvRect(QRectF rect);
    static QRectF qpoint2CvPoint(cv::Rect rect);
    static cv::Point qpoint2CvPoint(QPoint pt);
    static QPoint cvpoint2QPoint(cv::Point pt);
};

#endif // OPENCVTYPECONVERTER_H
