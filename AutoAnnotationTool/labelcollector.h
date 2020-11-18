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
#include <QPolygon>
#include <QPen>
#include <QMenu>
#include <QAction>
#include <QtConcurrent>

// std related header
#include <vector>
#include <memory>

#include "labeldata.h"
#include "mouseselectresult.h"
#include "CV/cvmodule.h"
#include "CV/cvparam.h"

class LabelCollector : public QQuickPaintedItem
{
    Q_OBJECT
    Q_PROPERTY(QImage image READ image WRITE setImage NOTIFY imageChanged)
    Q_PROPERTY(QString imgSrc READ imgSrc WRITE setImgSrc NOTIFY imgSrcChanged)
    Q_PROPERTY(int fileIdx READ fileIdx WRITE setFileIdx NOTIFY fileIdxChanged)
    Q_PROPERTY(CVParam* cvParam READ cvParam WRITE setCvParam NOTIFY cvParamChanged)

public:
    explicit LabelCollector(QQuickItem *parent = nullptr);
    void paint(QPainter *painter);
public:
    Q_INVOKABLE void RemoveLabel(int idx);
private:
    bool GetExistLabel(QPointF pt);
    void RemoveAllLabel();
    double DistanceBetween2Point(QPointF p1, QPointF p2);
    double DistanceBetweenPointAndLine(QPointF lineStart, QPointF lineEnd, QPointF point);
public:
    QImage image() const;
    QString imgSrc() const;
    QVector<LabelData*> dataVec() const;
    bool setItemAt(int index, LabelData *item);
    qreal getFactorScaled() const;
    int fileIdx() const;

    CVParam *cvParam() const;

private:
    QImage m_image;
    QImage m_imageScaled;
    qreal factorScaled;
    int imageWidth;
    int imageHeight;
    QString m_imgSrc;
    QFileInfoList fileInfoList;
public slots:
    void setImage(const QImage &image);
    void setImgSrc(QString imgSrc);
public slots:
    void appendData(QRectF rect);

    void setFileIdx(int fileIdx);

    void setCvParam(CVParam* cvParam);

signals:
    void imageChanged();
    void imgSrcChanged(QString imgSrc);

    // Model related signal
signals:
    void preItemAppended();
    void postItemAppended();

    void preItemRemoved(int index);
    void postItemRemoved();

    void onModelChanged();

    void fileIdxChanged(int fileIdx);

    //mouse behavior
    void cvParamChanged(CVParam* cvParam);

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
    QPen m_penPoint;
    QPen m_penPoly;
    QVector<QPen> m_penVec;
    std::vector<int> m_selectLabelIdx;

    QMenu menu;
private:
    QVector<LabelData*> m_dataVec;
    // mouse select related
private:
    void setCursorIcon();
    bool RectBoundaryCheck(QRectF rect);
    void PosBoundaryCheck(QPointF &pos);
    void CheckRectValid();
    void GetPolygonSelectResult(QPointF currentPos);
    void GetRectCornerResult(QPointF currentPos);
    void GetRectEdgeResult(QPointF currentPos);
    PolygonSelectResult polySelectResult;
    RectCornerSelectResult rectCornerSelectResult;
    RectEdgeSelectResult rectEdgeSelectResult;
private:
    std::unique_ptr<CVModule> cvModule;
    QFuture<void> future;
    QFutureWatcher<void> watcher;
    int m_fileIdx;
    CVParam* m_cvParam;
};

#endif // LABELCOLLECTOR_H
