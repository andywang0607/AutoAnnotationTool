#ifndef LABELCOLLECTOR_H
#define LABELCOLLECTOR_H

// Qt related header
#include <QObject>
#include <QQuickPaintedItem>
#include <QPainter>
#include <QImage>
#include <QVector>
#include <QPointF>
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
    void paint(QPainter *painter) override;

    // mouse related event
protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

    // Function exposed to QML
public:
    Q_INVOKABLE void removeLabel(int idx);

    // public get function
public:
    QImage image() const;
    QString imgSrc() const;
    int fileIdx() const;
    CvParam *cvParam() const;

    QVector<LabelData*> dataVec() const;
    bool setItemAt(int index, LabelData *item);
    qreal getFactorScaled() const;

private:
    QImage m_image;
    QImage m_scaledImg;
    qreal m_scaledRatio;
    int m_fileIdx;
    int m_imgWidth;
    int m_imgHeight;
    QString m_imgSrc;
    QFileInfoList m_fileInfoList;
    CvParam *m_cvParam;

public slots:
    void setImage(const QImage &image);
    void setImgSrc(QString imgSrc);
    void setFileIdx(int fileIdx);
    void setCvParam(CvParam* cvParam);

public slots:
    void appendData(QRectF rect);
    void appendData(QRectF rect, QString labelClass);
    void appendData(QPolygonF poly, QString labelClass);
    void appendData(QRectF rect, QPolygonF poly, QString labelClass);

    // signal from Q_PROPERTY
signals:
    void imageChanged();
    void imgSrcChanged(QString imgSrc);
    void fileIdxChanged(int fileIdx);
    void cvParamChanged(CvParam* cvParam);

    // Model related signal
signals:
    void preItemAppended();
    void postItemAppended();
    void preItemRemoved(int index);
    void postItemRemoved();
    void onModelChanged();

private:
    bool getExistLabel(QPointF pt);
    void removeAllLabel();
    void setCursorIcon();

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
    QPen m_extensivePen;
    QVector<QPen> m_penVec;

    QVector<LabelData*> m_dataVec;

    QMenu menu;

    PolygonSelectResult m_polySelectResult;
    RectCornerSelectResult m_rectCornerSelectResult;
    RectEdgeSelectResult m_rectEdgeSelectResult;

    std::unique_ptr<CvModule> m_cvModule;
    QFuture<void> m_future;
    QFutureWatcher<void> m_watcher;

    std::vector<int> m_selectLabelIdx;
};

#endif // LABELCOLLECTOR_H
