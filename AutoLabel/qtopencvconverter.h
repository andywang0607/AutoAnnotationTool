#ifndef QTOPENCVCONVERTER_H
#define QTOPENCVCONVERTER_H

#include <QObject>

#include <QRect>
#include <QPoint>

#include <opencv2/core.hpp>
class QtOpenCVConverter : public QObject
{
    Q_OBJECT
public:
    explicit QtOpenCVConverter(QObject *parent = nullptr);
    cv::Rect QRect2CVRect(QRectF rect);
    QRectF CVRect2QRect(cv::Rect rect);
    cv::Point QPoint2CVPoint(QPoint pt);
    QPoint CVPoint2QPoint(cv::Point pt);
signals:

};

#endif // QTOPENCVCONVERTER_H
