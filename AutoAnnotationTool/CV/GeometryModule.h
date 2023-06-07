#ifndef GEOMETRYMODULE_H
#define GEOMETRYMODULE_H

#include <QPointF>
#include <QRectF>

#include "include/LableData.h"
#include "include/MouseSelectResult.h"

class GeometryModule
{
public:
    GeometryModule();
    static double distanceBetweenPoints(const QPointF p1, const QPointF p2);
    static double distanceBetweenPointAndLine(const QPointF lineStart, const QPointF lineEnd, const QPointF point);
    static bool rectBoundaryCheck(const QRectF rect, const int width, const int height);
    static void posBoundaryCheck(QPointF &pos, const int width, const int height);
    static void getPolygonSelectResult(const QPointF currentPos, const QVector<LabelData*> &dataVec, PolygonSelectResult &polygonResult);
    static void getRectCornerResult(const QPointF currentPos, const QVector<LabelData*> &dataVec, RectCornerSelectResult &rectCornerResult);
    static void getRectEdgeResult(const QPointF currentPos, const QVector<LabelData*> &dataVec, RectEdgeSelectResult &rectEdgeResult);
    static void preventInvalidRect(const QVector<LabelData*> &dataVec, const RectCornerSelectResult &rectCornerResult, const RectEdgeSelectResult &rectEdgeResult);
};

#endif // GEOMETRYMODULE_H
