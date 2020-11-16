#include "opencvtypeconverter.h"

OpenCVTypeConverter::OpenCVTypeConverter()
{

}

cv::Rect OpenCVTypeConverter::QRect2CVRect(QRectF rect)
{
    return cv::Rect(rect.topLeft().x(),rect.topLeft().y(),rect.width(),rect.height());
}

QRectF OpenCVTypeConverter::CVRect2QRect(cv::Rect rect)
{
    QPoint tl(rect.tl().x, rect.tl().y);
    QPoint br(rect.br().x, rect.br().y);
    QRectF ret(tl,br);
    return QRectF(tl,br);
}

cv::Point OpenCVTypeConverter::QPoint2CVPoint(QPoint pt)
{
    return cv::Point(pt.x(), pt.y());
}

QPoint OpenCVTypeConverter::CVPoint2QPoint(cv::Point pt)
{
    return QPoint(pt.x, pt.y);
}
