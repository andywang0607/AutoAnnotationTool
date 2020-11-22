#include "opencvtypeconverter.h"

OpencvTypeConverter::OpencvTypeConverter()
{

}

cv::Rect OpencvTypeConverter::qrect2CvRect(QRectF rect)
{
    return cv::Rect(rect.topLeft().x(),rect.topLeft().y(),rect.width(),rect.height());
}

QRectF OpencvTypeConverter::qpoint2CvPoint(cv::Rect rect)
{
    QPoint tl(rect.tl().x, rect.tl().y);
    QPoint br(rect.br().x, rect.br().y);
    QRectF ret(tl,br);
    return QRectF(tl,br);
}

cv::Point OpencvTypeConverter::qpoint2CvPoint(QPoint pt)
{
    return cv::Point(pt.x(), pt.y());
}

QPoint OpencvTypeConverter::cvpoint2QPoint(cv::Point pt)
{
    return QPoint(pt.x, pt.y);
}
