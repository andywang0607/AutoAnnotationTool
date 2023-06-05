#ifndef TYPECONVERTER_H
#define TYPECONVERTER_H

#include <QRect>
#include <QPoint>

#include <opencv2/core.hpp>

namespace Type {

template<typename RectType>
static RectType toRect(const QRectF &rect)
{
    static_assert(true, "Type::toRect: RectType not support");
}

template<>
static cv::Rect toRect<cv::Rect>(const QRectF &rect)
{
    return cv::Rect(rect.topLeft().x(),rect.topLeft().y(),rect.width(),rect.height());
}

template<typename RectType>
static QRectF fromRect(const RectType &rect)
{
    static_assert(true, "Type::fromRect: RectType not support");
}

template<>
static QRectF fromRect<cv::Rect>(const cv::Rect &rect)
{
    QPoint tl(rect.tl().x, rect.tl().y);
    QPoint br(rect.br().x, rect.br().y);
    return QRectF(tl,br);
}

template<typename PointType>
static PointType toPoint(const QPoint &pt)
{
    static_assert(true, "Type::toPoint: PointType not support");
}

template<>
static cv::Point toPoint<cv::Point>(const QPoint &pt)
{
    return cv::Point(pt.x(), pt.y());
}

template<typename PointType>
static QPoint fromPoint(const PointType &pt)
{
    static_assert(true, "Type::fromPoint: PointType not support");
}

template<>
static QPoint fromPoint<cv::Point>(const cv::Point &pt)
{
    return QPoint(pt.x, pt.y);
}

} // namespace Type


#endif // TYPECONVERTER_H
