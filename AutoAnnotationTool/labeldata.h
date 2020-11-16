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
    LabelData(const LabelData &e)
    {
        labelClass = e.labelClass;
        penIdx = e.penIdx;
        rect = e.rect;
        isSelect = e.isSelect;
        resultPoly = QPolygon(e.resultPoly);
    }
    ~LabelData()
    {
    }
    QString labelClass;
    int penIdx; //0: normal, 1:highlight
    bool isSelect;
    QRectF rect;
    QPolygon resultPoly; // record scaled point show in GUI
};

#endif // LABELDATA_H
