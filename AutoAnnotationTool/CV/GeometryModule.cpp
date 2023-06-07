#include "GeometryModule.h"

#include <math.h>

GeometryModule::GeometryModule()
{

}

static const int thresDistance = 5;

double GeometryModule::distanceBetweenPoints(const QPointF p1, const QPointF p2)
{
    QPointF p12Vec = p2-p1;
    return std::sqrt(std::pow(p12Vec.x(), 2) + std::pow(p12Vec.y(), 2));
}

double GeometryModule::distanceBetweenPointAndLine(const QPointF lineStart, const QPointF lineEnd, const QPointF point)
{
    double normalLength = hypot(lineEnd.x() - lineStart.x(), lineEnd.y() - lineStart.y());
    double distance = (double)((point.x() - lineStart.x()) * (lineEnd.y() - lineStart.y()) -
                               (point.y() - lineStart.y()) * (lineEnd.x() - lineStart.x())) / normalLength;
    return abs(distance);
}

bool GeometryModule::rectBoundaryCheck(QRectF rect, int width, int height)
{
    if (rect.top() < 0)
        return false;
    if (rect.right() > width)
        return false;
    if (rect.bottom() > height)
        return false;
    if (rect.left() < 0)
        return false;
    return true;
}

void GeometryModule::posBoundaryCheck(QPointF &pos, const int width, const int height)
{
    if (pos.x() < 0)
        pos.setX(0);
    if (pos.x() > width)
        pos.setX(width);
    if (pos.y() > height)
        pos.setY(height);
    if (pos.y() < 0)
        pos.setY(0);
}

void GeometryModule::getPolygonSelectResult(const QPointF currentPos, const QVector<LabelData *> &dataVec, PolygonSelectResult &polygonResult)
{
    PolygonSelectResult res;
    QVector<LabelData*>::const_iterator it;
    for (it=dataVec.begin(); it!=dataVec.end(); it++) {
        QPolygonF checkedPoly = ((*it)->poly);
        QPolygonF::iterator polyIter;
        for (polyIter = checkedPoly.begin(); polyIter != checkedPoly.end(); polyIter++) {
            if (distanceBetweenPoints(*polyIter,currentPos) < thresDistance) {
                polygonResult.boxIdx = std::distance(dataVec.begin(), it);
                polygonResult.polyIdx = std::distance(checkedPoly.begin(), polyIter);
                polygonResult.isSelect = true;
                return;
            }
        }
    }
    polygonResult = res;
}

void GeometryModule::getRectCornerResult(const QPointF currentPos, const QVector<LabelData *> &dataVec, RectCornerSelectResult &rectCornerResult)
{
    RectCornerSelectResult res;
    rectCornerResult = res;
    QVector<LabelData*>::const_iterator it;
    for (it=dataVec.begin(); it!=dataVec.end(); it++) {
        double length = 0.0;
        // topleft
        QPointF tmp = (*it)->rect.topLeft() - currentPos;
        length = std::sqrt(std::pow(tmp.x(), 2) + std::pow(tmp.y(), 2));
        if (length < thresDistance) {
            rectCornerResult.isSelect = true;
            rectCornerResult.corner = 0;
            rectCornerResult.boxIdx = std::distance(dataVec.begin(),it);
            return;
        }
        // topRight
        tmp = (*it)->rect.topRight() - currentPos;
        length = std::sqrt(std::pow(tmp.x(), 2) + std::pow(tmp.y(), 2));
        if (length < thresDistance) {
            rectCornerResult.isSelect = true;
            rectCornerResult.corner = 1;
            rectCornerResult.boxIdx = std::distance(dataVec.begin(),it);
            return;
        }
        // bottomRight
        tmp = (*it)->rect.bottomRight() - currentPos;
        length = std::sqrt(std::pow(tmp.x(), 2) + std::pow(tmp.y(), 2));
        if (length < thresDistance) {
            rectCornerResult.isSelect = true;
            rectCornerResult.corner = 2;
            rectCornerResult.boxIdx = std::distance(dataVec.begin(),it);
            return;
        }
        // bottomLeft
        tmp = (*it)->rect.bottomLeft() - currentPos;
        length = std::sqrt(std::pow(tmp.x(), 2) + std::pow(tmp.y(), 2));
        if (length < thresDistance) {
            rectCornerResult.isSelect = true;
            rectCornerResult.corner = 3;
            rectCornerResult.boxIdx = std::distance(dataVec.begin(),it);
            return;
        }
    }
}

void GeometryModule::getRectEdgeResult(const QPointF currentPos, const QVector<LabelData *> &dataVec, RectEdgeSelectResult &rectEdgeResult)
{
    RectEdgeSelectResult res;
    rectEdgeResult = res;
    QVector<LabelData*>::const_iterator it;
    for (it=dataVec.begin(); it!=dataVec.end(); it++) {
        double length = 0.0;
        // left
        length = distanceBetweenPointAndLine((*it)->rect.bottomLeft(),(*it)->rect.topLeft(),currentPos);
        if (length < thresDistance) {
            if ((*it)->rect.bottom() > currentPos.y() && currentPos.y() > (*it)->rect.top()) {
                rectEdgeResult.isSelect = true;
                rectEdgeResult.line = 0;
                rectEdgeResult.boxIdx = std::distance(dataVec.begin(),it);
                return;
            }
        }

        // top
        length = distanceBetweenPointAndLine((*it)->rect.topLeft(),(*it)->rect.topRight(),currentPos);
        if (length < thresDistance){
            if ((*it)->rect.left() < currentPos.x() && currentPos.x() < (*it)->rect.right()) {
                rectEdgeResult.isSelect = true;
                rectEdgeResult.line = 1;
                rectEdgeResult.boxIdx = std::distance(dataVec.begin(),it);
                return;
            }
        }

        // right
        length = distanceBetweenPointAndLine((*it)->rect.topRight(),(*it)->rect.bottomRight(),currentPos);
        if (length < thresDistance) {
            if ((*it)->rect.bottom() > currentPos.y() && currentPos.y() > (*it)->rect.top()) {
                rectEdgeResult.isSelect = true;
                rectEdgeResult.line = 2;
                rectEdgeResult.boxIdx = std::distance(dataVec.begin(),it);
                return;
            }
        }

        // bottom
        length = distanceBetweenPointAndLine((*it)->rect.bottomLeft(),(*it)->rect.bottomRight(),currentPos);
        if (length < thresDistance){
            if ((*it)->rect.left() < currentPos.x() && currentPos.x() < (*it)->rect.right()) {
                rectEdgeResult.isSelect = true;
                rectEdgeResult.line = 3;
                rectEdgeResult.boxIdx = std::distance(dataVec.begin(),it);
                return;
            }
        }
    }
}

void GeometryModule::preventInvalidRect(const QVector<LabelData*> &dataVec, const RectCornerSelectResult &rectCornerResult, const RectEdgeSelectResult &rectEdgeResult)
{
    if(rectCornerResult.isSelect) {
        QPointF topLeft;
        QPointF bottomRight;
        topLeft.setX(qMin(dataVec.at(rectCornerResult.boxIdx)->rect.left(), dataVec.at(rectCornerResult.boxIdx)->rect.right()));
        topLeft.setY(qMin(dataVec.at(rectCornerResult.boxIdx)->rect.top(), dataVec.at(rectCornerResult.boxIdx)->rect.bottom()));

        bottomRight.setX(qMax(dataVec.at(rectCornerResult.boxIdx)->rect.left(), dataVec.at(rectCornerResult.boxIdx)->rect.right()));
        bottomRight.setY(qMax(dataVec.at(rectCornerResult.boxIdx)->rect.top(), dataVec.at(rectCornerResult.boxIdx)->rect.bottom()));

        dataVec.at(rectCornerResult.boxIdx)->rect.setTopLeft(topLeft);
        dataVec.at(rectCornerResult.boxIdx)->rect.setBottomRight(bottomRight);
    } else if(rectEdgeResult.isSelect) {

        QPointF topLeft;
        QPointF bottomRight;
        topLeft.setX(qMin(dataVec.at(rectEdgeResult.boxIdx)->rect.left(), dataVec.at(rectEdgeResult.boxIdx)->rect.right()));
        topLeft.setY(qMin(dataVec.at(rectEdgeResult.boxIdx)->rect.top(), dataVec.at(rectEdgeResult.boxIdx)->rect.bottom()));

        bottomRight.setX(qMax(dataVec.at(rectEdgeResult.boxIdx)->rect.left(), dataVec.at(rectEdgeResult.boxIdx)->rect.right()));
        bottomRight.setY(qMax(dataVec.at(rectEdgeResult.boxIdx)->rect.top(), dataVec.at(rectEdgeResult.boxIdx)->rect.bottom()));

        dataVec.at(rectEdgeResult.boxIdx)->rect.setTopLeft(topLeft);
        dataVec.at(rectEdgeResult.boxIdx)->rect.setBottomRight(bottomRight);
    }
}
