#include "qtopencvconverter.h"

QtOpenCVConverter::QtOpenCVConverter(QObject *parent) : QObject(parent)
{

}

cv::Rect QtOpenCVConverter::QRect2CVRect(QRectF rect)
{
    cv::Rect ret(rect.topLeft().x(),rect.topLeft().y(),rect.width(),rect.height());
    return ret;
}

QRectF QtOpenCVConverter::CVRect2QRect(cv::Rect rect)
{
    QPoint tl(rect.tl().x, rect.tl().y);
    QPoint br(rect.br().x, rect.br().y);
    QRectF ret(tl,br);
    return ret;
}

QPoint QtOpenCVConverter::CVPoint2QPoint(cv::Point pt)
{
    QPoint ret(pt.x, pt.y);
    return ret;
}

cv::Point QtOpenCVConverter::QPoint2CVPoint(QPoint pt)
{
    cv::Point ret(pt.x(), pt.y());
    return ret;
}
