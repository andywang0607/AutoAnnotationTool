#ifndef LABELDATA_H
#define LABELDATA_H
#include <QPolygon>
#include <QPoint>

//OpenCV related header
#include <opencv2/core/core.hpp>

struct LabelData{
public:
    LabelData(const QRectF &m_rect):
        labelClass(""), rect(m_rect), penIdx(0), isSelect(false){}
    LabelData(const QRectF &m_rect, const QString m_labelClass):
        labelClass(m_labelClass), rect(m_rect), penIdx(0), isSelect(false){}
    LabelData(const QPolygonF &m_poly, const QString m_labelClass):
        labelClass(m_labelClass), poly(m_poly), penIdx(0), isSelect(false){}
    LabelData(const QRectF &m_rect, const QPolygonF &m_poly, const QString m_labelClass):
        labelClass(m_labelClass), rect(m_rect), poly(m_poly), penIdx(0), isSelect(false){}
    LabelData(const LabelData &e)
    {
        labelClass = e.labelClass;
        penIdx = e.penIdx;
        rect = e.rect;
        isSelect = e.isSelect;
        poly = QPolygonF(e.poly);
    }
    ~LabelData()
    {
    }
    QString labelClass;
    int penIdx; //0: normal, 1:highlight
    bool isSelect;
    QRectF rect;
    QPolygonF poly; // record scaled point show in GUI
};

#endif // LABELDATA_H
