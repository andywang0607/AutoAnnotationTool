#ifndef LABELCOLLECTOR_H
#define LABELCOLLECTOR_H

// Qt related header
#include <QObject>
#include <QQuickPaintedItem>
#include <QPainter>
#include <QImage>
#include <QVector>
#include <QPointF>
#include <QLineF>
#include <QPen>

// std related header
#include <vector>

//OpenCV related header
#include <opencv2/core/core.hpp>

struct LabelData{
public:
    LabelData(const cv::Rect &m_rect):
        rect(m_rect), penIdx(0), isSelect(false){}
    LabelData(const LabelData &e)
    {
        penIdx = e.penIdx;
        rect = e.rect;
        isSelect = e.isSelect;
        contours.assign(e.contours.begin(),e.contours.end());
        contoursPoly.assign(e.contoursPoly.begin(),e.contoursPoly.end());
        result.assign(e.result.begin(),e.result.end());
    }
    ~LabelData()
    {
    }
    int penIdx; //0: normal, 1:highlight
    bool isSelect;
    cv::Rect rect;
    std::vector<std::vector<cv::Point>> contours;
    std::vector<std::vector<cv::Point>> contoursPoly;
    std::vector<cv::Point> result;
};

class LabelCollector : public QQuickPaintedItem
{
    Q_OBJECT
    Q_PROPERTY(QImage image READ image WRITE setImage NOTIFY imageChanged)
    Q_PROPERTY(QString imgSrc READ imgSrc WRITE setImgSrc NOTIFY imgSrcChanged)

public:
    explicit LabelCollector(QQuickItem *parent = nullptr);
    void paint(QPainter *painter);
public:
    QImage image() const;
    QString imgSrc() const;

private:
    QImage m_image;
    QImage m_imageScaled;
    qreal factorScaled;
    int imageWidth;
    int imageHeight;
    QString m_imgSrc;

public slots:
    void setImage(const QImage &image);
    void setImgSrc(QString imgSrc);
public slots:
    void appendData(cv::Rect rect);

signals:

    void imageChanged();
    void imgSrcChanged(QString imgSrc);

    //mouse behavior
protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
private:
    bool m_mouseEnabled;
    bool m_mousePressed;
    bool m_mouseMoved;

    QPointF m_lastPoint;
    QPointF m_currentPoint;
    QPointF m_firstPoint;

    QPen m_penNormal;
    QPen m_penHighlight;
    QVector<QPen> m_penVec;

private:
    QVector<LabelData*> m_dataVec;
};

#endif // LABELCOLLECTOR_H
