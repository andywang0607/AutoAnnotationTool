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
    Q_PROPERTY(CvParam* cvParam READ cvParam WRITE setCvParam NOTIFY cvParamChanged)

public:
    explicit LabelCollector(QQuickItem *parent = nullptr);
    void paint(QPainter *painter);
public:
    Q_INVOKABLE void removeLabel(int idx);
private:
    bool getExistLabel(QPointF pt);
    void removeAllLabel();
public:
    QImage image() const;
    QString imgSrc() const;
    QVector<LabelData*> dataVec() const;
    bool setItemAt(int index, LabelData *item);
    qreal getFactorScaled() const;
    int fileIdx() const;

    CvParam *cvParam() const;

private:
    QImage m_image;
    QImage m_scaledImg;
    qreal m_scaledRatio;
    int m_imgWidth;
    int m_imgHeight;
    QString m_imgSrc;
    QFileInfoList m_fileInfoList;
public slots:
    void setImage(const QImage &image);
    void setImgSrc(QString imgSrc);
public slots:
    void appendData(QRectF rect);
    void appendData(QRectF rect, QString labelClass);
    void appendData(QPolygonF poly, QString labelClass);
    void appendData(QRectF rect, QPolygonF poly, QString labelClass);

    void setFileIdx(int fileIdx);

    void setCvParam(CvParam* cvParam);

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
    void cvParamChanged(CvParam* cvParam);

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

    QPen m_normalPen;
    QPen m_highlightPen;
    QPen m_pointPen;
    QPen m_polyPen;
    QVector<QPen> m_penVec;
    std::vector<int> m_selectLabelIdx;

    QMenu menu;
private:
    QVector<LabelData*> m_dataVec;
    // mouse select related
private:
    void setCursorIcon();
    PolygonSelectResult m_polySelectResult;
    RectCornerSelectResult m_rectCornerSelectResult;
    RectEdgeSelectResult m_rectEdgeSelectResult;
private:
    std::unique_ptr<CvModule> m_cvModule;
    QFuture<void> m_future;
    QFutureWatcher<void> m_watcher;
    int m_fileIdx;
    CvParam *m_cvParam;
};

#endif // LABELCOLLECTOR_H
