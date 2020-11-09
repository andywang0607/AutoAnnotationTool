#ifndef LABELDATA_H
#define LABELDATA_H
#include <QPolygon>
#include <QPoint>

//OpenCV related header
#include <opencv2/core/core.hpp>

struct LabelData{
public:
    LabelData(const cv::Rect &m_rect):
        labelClass(""), rect(m_rect), penIdx(0), isSelect(false){}
    LabelData(const LabelData &e)
    {
        labelClass = e.labelClass;
        penIdx = e.penIdx;
        rect = e.rect;
        isSelect = e.isSelect;
        contoursPoly.assign(e.contoursPoly.begin(),e.contoursPoly.end());
        result.assign(e.result.begin(),e.result.end());
        resultPoly = QPolygon(e.resultPoly);
    }
    ~LabelData()
    {
    }
    QString labelClass;
    int penIdx; //0: normal, 1:highlight
    bool isSelect;
    cv::Rect rect;
    std::vector<cv::Point> contoursPoly;
    std::vector<QPoint> result; // record scaled point show in GUI
    QPolygon resultPoly;
};

#endif // LABELDATA_H
