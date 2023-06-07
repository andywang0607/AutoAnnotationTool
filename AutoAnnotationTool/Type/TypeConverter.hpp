#ifndef TYPECONVERTER_H
#define TYPECONVERTER_H

#include <QRect>
#include <QPoint>

#include <opencv2/core.hpp>

namespace Type {

template<typename RectType>
inline RectType toRect(const QRectF &rect)
{
    static_assert(true, "Type::toRect: RectType not support");
    return RectType();
}

template<>
inline cv::Rect toRect<cv::Rect>(const QRectF &rect)
{
    return cv::Rect(rect.topLeft().x(),rect.topLeft().y(),rect.width(),rect.height());
}

template<typename RectType>
inline QRectF fromRect(const RectType &rect)
{
    static_assert(true, "Type::fromRect: RectType not support");
    return QRectF();
}

template<>
inline QRectF fromRect<cv::Rect>(const cv::Rect &rect)
{
    QPoint tl(rect.tl().x, rect.tl().y);
    QPoint br(rect.br().x, rect.br().y);
    return QRectF(tl,br);
}

template<typename PointType>
inline PointType toPoint(const QPoint &pt)
{
    static_assert(true, "Type::toPoint: PointType not support");
    return PointType();
}

template<>
inline cv::Point toPoint<cv::Point>(const QPoint &pt)
{
    return cv::Point(pt.x(), pt.y());
}

template<typename PointType>
inline QPoint fromPoint(const PointType &pt)
{
    static_assert(true, "Type::fromPoint: PointType not support");
    return QPoint();
}

template<>
inline QPoint fromPoint<cv::Point>(const cv::Point &pt)
{
    return QPoint(pt.x, pt.y);
}

} // namespace Type


#endif // TYPECONVERTER_H
