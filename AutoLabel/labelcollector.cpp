#include "labelcollector.h"

#include <QtGlobal>

static const int thresDistance = 5;

LabelCollector::LabelCollector(QQuickItem *parent) : QQuickPaintedItem(parent)
  , m_mouseEnabled(true)
  , m_mousePressed(false)
  , m_mouseMoved(false)
  , m_penNormal(QPen(Qt::green, 3, Qt::SolidLine, Qt::SquareCap, Qt::MiterJoin))
  , m_penHighlight(QPen(Qt::red, 5, Qt::SolidLine, Qt::SquareCap, Qt::MiterJoin))
  , m_penPoint(QPen(QColor(220,118,51), 7, Qt::SolidLine, Qt::SquareCap, Qt::MiterJoin))
  , m_penPoly(QPen(Qt::yellow , 3, Qt::SolidLine, Qt::SquareCap, Qt::MiterJoin))
{
    setAcceptedMouseButtons(Qt::AllButtons);
    m_penVec.push_back(m_penNormal);
    m_penVec.push_back(m_penHighlight);
    m_penVec.push_back(m_penPoint);
    m_penVec.push_back(m_penPoly);

    menu.addAction(QStringLiteral("Get Polygon"),this, [=](){emit processRequest(m_selectLabelIdx.front());});
}

void LabelCollector::paint(QPainter *painter){
    if(!this->m_imageScaled.isNull() ){
        QRect rect(m_imageScaled.rect());
        painter->drawImage(rect.topLeft(), m_imageScaled);
    }
    if(m_selectLabelIdx.empty()){
        if(m_mouseMoved){
            painter->setPen(m_penNormal);
            painter->setRenderHint(QPainter::Antialiasing);
            painter->drawRect(QRectF(m_firstPoint,m_lastPoint));
        }
    }
    if(m_dataVec.empty()) return;
    QVector<LabelData*>::iterator iter;
    for(iter=m_dataVec.begin();iter!=m_dataVec.end();iter++){
        // Draw bounding box
        painter->setRenderHint(QPainter::Antialiasing);
        painter->setPen(m_penVec.at((*iter)->penIdx));
        painter->drawRect((*iter)->rect);
        // Draw result polygon
        painter->setPen(m_penVec.at(3));
        painter->drawPolygon((*iter)->resultPoly);

        // Draw result point
        painter->setPen(m_penVec.at(2));
        std::vector<QPoint>::iterator pointIter;
        int dataIdx = std::distance(m_dataVec.begin(),iter);
        for(pointIter=(*iter)->result.begin();pointIter!=(*iter)->result.end();pointIter++){
            int pointIdx = std::distance((*iter)->result.begin(),pointIter);
            if(dataIdx == polySelectResult.boxIdx && pointIdx == polySelectResult.polyIdx){
                QRadialGradient gradient(*pointIter,5,*pointIter);
                gradient.setColorAt(0,QColor(220,118,51));
                gradient.setColorAt(0.5,QColor(229,152,102));
                gradient.setColorAt(1,QColor(237,187,153));
                painter->setPen(QColor(237,187,153));
                painter->setBrush(QBrush(gradient));
                painter->drawEllipse(*pointIter,5,5);
            }
            else{
                painter->setPen(m_penVec.at(2));
                painter->setBrush(QBrush(Qt::NoBrush));
                painter->drawPoint(*pointIter);
            }
        }
    }
}

void LabelCollector::RemoveLabel(int idx)
{
    emit preItemRemoved(idx);
    delete m_dataVec.at(idx);
    m_dataVec[idx] = nullptr;
    m_dataVec.erase(m_dataVec.begin()+idx);
    emit postItemRemoved();
    update();
}

void LabelCollector::SetContours(int labelIdx, std::vector<cv::Point> &contoursPoly)
{
    qDebug()<< Q_FUNC_INFO << "start";
    QPolygon tmpPoly;
    m_dataVec[labelIdx]->contoursPoly.assign(contoursPoly.begin(),contoursPoly.end());
    m_dataVec[labelIdx]->result.resize(m_dataVec[labelIdx]->contoursPoly.size());
    for(int i =0;i< m_dataVec[labelIdx]->contoursPoly.size();++i){
        cv::Point tmp = m_dataVec[labelIdx]->contoursPoly[i];
        QPoint resultPoint = QPoint(tmp.x*(1.0f/getFactorScaled()),tmp.y*(1.0f/getFactorScaled()));
        m_dataVec[labelIdx]->result[i] = resultPoint;
        tmpPoly.push_back(resultPoint);
    }
    m_dataVec[labelIdx]->resultPoly = tmpPoly;
    update();
    qDebug()<< Q_FUNC_INFO << "end";
}


bool LabelCollector::GetExistLabel(QPointF pt)
{
    bool res = false;
    m_selectLabelIdx.clear();
    QVector<LabelData*>::iterator it;
    for(it=m_dataVec.begin(); it!=m_dataVec.end(); it++){
        if((*it)->rect.contains(pt.toPoint())){
            m_selectLabelIdx.push_back(std::distance(m_dataVec.begin(),it));
            (*it)->isSelect = true;
            res = true;
        }
    }
    return res;
}

void LabelCollector::RemoveAllLabel()
{
    int size = m_dataVec.size();
    for(int i=0;i<size;++i){
        RemoveLabel(0);
    }
}

double LabelCollector::DistanceBetween2Point(QPointF p1, QPointF p2)
{
    QPointF p12Vec = p2-p1;
    return std::sqrt(std::pow(p12Vec.x(), 2) + std::pow(p12Vec.y(), 2));
}

double LabelCollector::DistanceBetweenPointAndLine(QPointF lineStart, QPointF lineEnd, QPointF point)
{
    double normalLength = hypot(lineEnd.x() - lineStart.x(), lineEnd.y() - lineStart.y());
    double distance = (double)((point.x() - lineStart.x()) * (lineEnd.y() - lineStart.y()) -
                               (point.y() - lineStart.y()) * (lineEnd.x() - lineStart.x())) / normalLength;
    return abs(distance);
}

QImage LabelCollector::image() const
{
    return m_image;
}

QString LabelCollector::imgSrc() const
{
    return m_imgSrc;
}

QVector<LabelData *> LabelCollector::dataVec() const
{
    return m_dataVec;
}

bool LabelCollector::setItemAt(int index, LabelData *item)
{
    if (index < 0 || index >= m_dataVec.size())
        return false;
    LabelData *oldItem = m_dataVec.at(index);
    if (item->labelClass == oldItem->labelClass
            && item->isSelect == oldItem->isSelect)
        return false;
    m_dataVec[index] = item;
    return true;
}

qreal LabelCollector::getFactorScaled() const
{
    return factorScaled;
}

void LabelCollector::setImage(const QImage &image){
    if(image.isNull()){
        qDebug() << Q_FUNC_INFO << "image is Null";
        return;
    }
    m_image = image;
    if(image.width() != this->width() && image.height() != this->height()){
        m_imageScaled = image.scaled(this->width(), this->height(), Qt::KeepAspectRatio);
        factorScaled = (float)image.width() / (float)m_imageScaled.width();
    }
    else{
        factorScaled = 1;
        this->m_image = image;
    }
    imageWidth = m_imageScaled.width();
    imageHeight = m_imageScaled.height();
    // Redraw the image
    update();
    emit imageChanged();
    // Remove all label while iamge change
    RemoveAllLabel();

}

void LabelCollector::setImgSrc(QString imgSrc)
{
    if (m_imgSrc == imgSrc)
        return;
    m_imgSrc = imgSrc;
    qDebug() << Q_FUNC_INFO << " m_imgSrc"<<m_imgSrc;
    if(m_imgSrc.left(4) == "file"){
#ifdef _WIN32
        m_imgSrc = m_imgSrc.remove(0,8);
#else
        m_imgSrc = m_imgSrc.remove(0,7);
#endif
    }
    setImage(QImage(m_imgSrc));
    emit imgSrcChanged(m_imgSrc);
}

void LabelCollector::mousePressEvent(QMouseEvent *event)
{
    m_mouseMoved = false;
    if(!(event->button() & acceptedMouseButtons()))
    {
        QQuickPaintedItem::mousePressEvent(event);
        return;
    }
    GetExistLabel(event->localPos());
    if(Qt::LeftButton == event->button()){
        m_mousePressed = true;
        GetPolygonSelectResult(event->localPos());
        GetRectCornerResult(event->localPos());
        GetRectEdgeResult(event->localPos());
        m_firstPoint = event->localPos();
        m_lastPoint = m_firstPoint;
        m_currentPoint = m_firstPoint;
        setCursorIcon();
        event->setAccepted(true);
    }
    if(Qt::RightButton == event->button()){
        if(m_selectLabelIdx.empty()) return;
        menu.exec(QCursor::pos());
    }
}

void LabelCollector::mouseMoveEvent(QMouseEvent *event)
{
    if(!m_mouseEnabled || !m_mousePressed)
    {
        QQuickPaintedItem::mousePressEvent(event);
        return;
    }
    if(Qt::RightButton == event->button()) return;
    m_lastPoint = event->localPos();
    if(polySelectResult.isSelect){
        m_dataVec.at(polySelectResult.boxIdx)->resultPoly.setPoint(polySelectResult.polyIdx, m_lastPoint.toPoint());
        m_dataVec.at(polySelectResult.boxIdx)->result.at(polySelectResult.polyIdx) = m_lastPoint.toPoint();
    }
    else if(!polySelectResult.isSelect && rectCornerSelectResult.isSelect){
        switch (rectCornerSelectResult.corner){
        case 0:
            m_dataVec.at(rectCornerSelectResult.boxIdx)->rect.setTopLeft(m_lastPoint);
            break;
        case 1:
            m_dataVec.at(rectCornerSelectResult.boxIdx)->rect.setTopRight(m_lastPoint);
            break;
        case 2:
            m_dataVec.at(rectCornerSelectResult.boxIdx)->rect.setBottomRight(m_lastPoint);
            break;
        case 3:
            m_dataVec.at(rectCornerSelectResult.boxIdx)->rect.setBottomLeft(m_lastPoint);
            break;
        default:
            break;
        }
    }
    else if(!polySelectResult.isSelect && !rectCornerSelectResult.isSelect && rectEdgeSelectResult.isSelect){
        switch (rectEdgeSelectResult.line){
        case 0:
            m_dataVec.at(rectEdgeSelectResult.boxIdx)->rect.setLeft(m_lastPoint.x());
            break;
        case 1:
            m_dataVec.at(rectEdgeSelectResult.boxIdx)->rect.setTop(m_lastPoint.y());
            break;
        case 2:
            m_dataVec.at(rectEdgeSelectResult.boxIdx)->rect.setRight(m_lastPoint.x());
            break;
        case 3:
            m_dataVec.at(rectEdgeSelectResult.boxIdx)->rect.setBottom(m_lastPoint.y());
            break;
        default:
            break;
        }
    }
    else if(!m_selectLabelIdx.empty()){
        QPointF offset = m_lastPoint - m_currentPoint;
        m_currentPoint = m_lastPoint;
        for(auto const &idx : m_selectLabelIdx){
            m_dataVec.at(idx)->rect.translate(offset);
        }
    }
    else
    {
        m_mouseMoved = true;
        m_lastPoint = event->localPos();
    }
    update();
}

void LabelCollector::mouseReleaseEvent(QMouseEvent *event)
{
    if(!m_mouseEnabled || !(event->button() & acceptedMouseButtons()))
    {
        QQuickPaintedItem::mousePressEvent(event);
        return;
    }
    setCursor(QCursor(Qt::ArrowCursor));
    if(Qt::RightButton == event->button()) return;
    m_mousePressed = false;
    m_mouseMoved = false;
    if(polySelectResult.isSelect){
        PolygonSelectResult defaultResult;
        polySelectResult = defaultResult;
        update();
        return;
    }
    if(polySelectResult.isSelect || rectEdgeSelectResult.isSelect || rectCornerSelectResult.isSelect)
        return;
    if(!m_selectLabelIdx.empty())
        return;
    if(m_firstPoint != m_lastPoint){
        QPoint point_lt(qMin(m_firstPoint.x(),m_lastPoint.x()),qMin(m_firstPoint.y(),m_lastPoint.y()));
        QPoint point_rb(qMax(m_firstPoint.x(),m_lastPoint.x()),qMax(m_firstPoint.y(),m_lastPoint.y()));
        appendData(QRectF(point_lt,point_rb));
        update();
    }
}

void LabelCollector::setCursorIcon()
{
    if(rectCornerSelectResult.isSelect){
        switch (rectCornerSelectResult.corner){
        case 0:
            setCursor(QCursor(Qt::SizeFDiagCursor));
            break;
        case 1:
            setCursor(QCursor(Qt::SizeBDiagCursor));
            break;
        case 2:
            setCursor(QCursor(Qt::SizeFDiagCursor));
            break;
        case 3:
            setCursor(QCursor(Qt::SizeBDiagCursor));
            break;
        }
    }
    else if(rectEdgeSelectResult.isSelect){
        switch (rectEdgeSelectResult.line){
        case 0:
            setCursor(QCursor(Qt::SizeHorCursor));
            break;
        case 1:
            setCursor(QCursor(Qt::SizeVerCursor));
            break;
        case 2:
            setCursor(QCursor(Qt::SizeHorCursor));
            break;
        case 3:
            setCursor(QCursor(Qt::SizeVerCursor));
            break;
        }
    }
    else if(!m_selectLabelIdx.empty()){
        setCursor(QCursor(Qt::ClosedHandCursor));
    }
    else{
        setCursor(QCursor(Qt::CrossCursor));
    }
}

void LabelCollector::GetPolygonSelectResult(QPointF currentPos)
{
    PolygonSelectResult res;
    QVector<LabelData*>::iterator it;
    for(it=m_dataVec.begin(); it!=m_dataVec.end(); it++){
        QPolygon checkedPoly = ((*it)->resultPoly);
        QPolygon::iterator polyIter;
        for(polyIter = checkedPoly.begin(); polyIter != checkedPoly.end(); polyIter++){
            if(DistanceBetween2Point(*polyIter,currentPos) < thresDistance){
                polySelectResult.boxIdx = std::distance(m_dataVec.begin(), it);
                polySelectResult.polyIdx = std::distance(checkedPoly.begin(), polyIter);
                polySelectResult.isSelect = true;
                qDebug() << Q_FUNC_INFO << "finish";
                return;
            }
        }
    }
    polySelectResult = res;
}

void LabelCollector::GetRectCornerResult(QPointF pt)
{
    RectCornerSelectResult res;
    rectCornerSelectResult = res;
    QVector<LabelData*>::iterator it;
    for(it=m_dataVec.begin(); it!=m_dataVec.end(); it++){
        double length = 0.0;
        // topleft
        QPointF tmp = (*it)->rect.topLeft() - pt;
        length = std::sqrt(std::pow(tmp.x(), 2) + std::pow(tmp.y(), 2));
        if(length < thresDistance){
            rectCornerSelectResult.isSelect = true;
            rectCornerSelectResult.corner = 0;
            rectCornerSelectResult.boxIdx = std::distance(m_dataVec.begin(),it);
            return;
        }
        // topRight
        tmp = (*it)->rect.topRight() - pt;
        length = std::sqrt(std::pow(tmp.x(), 2) + std::pow(tmp.y(), 2));
        if(length < thresDistance){
            rectCornerSelectResult.isSelect = true;
            rectCornerSelectResult.corner = 1;
            rectCornerSelectResult.boxIdx = std::distance(m_dataVec.begin(),it);
            return;
        }
        // bottomRight
        tmp = (*it)->rect.bottomRight() - pt;
        length = std::sqrt(std::pow(tmp.x(), 2) + std::pow(tmp.y(), 2));
        if(length < thresDistance){
            rectCornerSelectResult.isSelect = true;
            rectCornerSelectResult.corner = 2;
            rectCornerSelectResult.boxIdx = std::distance(m_dataVec.begin(),it);
            return;
        }
        // bottomLeft
        tmp = (*it)->rect.bottomLeft() - pt;
        length = std::sqrt(std::pow(tmp.x(), 2) + std::pow(tmp.y(), 2));
        if(length < thresDistance){
            rectCornerSelectResult.isSelect = true;
            rectCornerSelectResult.corner = 3;
            rectCornerSelectResult.boxIdx = std::distance(m_dataVec.begin(),it);
            return;
        }
    }
}

void LabelCollector::GetRectEdgeResult(QPointF currentPos)
{
    RectEdgeSelectResult res;
    rectEdgeSelectResult = res;
    QVector<LabelData*>::iterator it;
    for(it=m_dataVec.begin(); it!=m_dataVec.end(); it++){
        double length = 0.0;
        // left
        length = DistanceBetweenPointAndLine((*it)->rect.bottomLeft(),(*it)->rect.topLeft(),currentPos);
        if(length < thresDistance){
            if((*it)->rect.bottom() > currentPos.y() && currentPos.y() > (*it)->rect.top()){
                rectEdgeSelectResult.isSelect = true;
                rectEdgeSelectResult.line = 0;
                rectEdgeSelectResult.boxIdx = std::distance(m_dataVec.begin(),it);
                return;
            }
        }

        // top
        length = DistanceBetweenPointAndLine((*it)->rect.topLeft(),(*it)->rect.topRight(),currentPos);
        if(length < thresDistance){
            if((*it)->rect.left() < currentPos.x() && currentPos.x() < (*it)->rect.right()){
                rectEdgeSelectResult.isSelect = true;
                rectEdgeSelectResult.line = 1;
                rectEdgeSelectResult.boxIdx = std::distance(m_dataVec.begin(),it);
                return;
            }
        }

        // right
        length = DistanceBetweenPointAndLine((*it)->rect.topRight(),(*it)->rect.bottomRight(),currentPos);
        if(length < thresDistance){
            if((*it)->rect.bottom() > currentPos.y() && currentPos.y() > (*it)->rect.top()){
                rectEdgeSelectResult.isSelect = true;
                rectEdgeSelectResult.line = 2;
                rectEdgeSelectResult.boxIdx = std::distance(m_dataVec.begin(),it);
                return;
            }
        }

        // bottom
        length = DistanceBetweenPointAndLine((*it)->rect.bottomLeft(),(*it)->rect.bottomRight(),currentPos);
        if(length < thresDistance){
            if((*it)->rect.left() < currentPos.x() && currentPos.x() < (*it)->rect.right()){
                rectEdgeSelectResult.isSelect = true;
                rectEdgeSelectResult.line = 3;
                rectEdgeSelectResult.boxIdx = std::distance(m_dataVec.begin(),it);
                return;
            }
        }
    }
}

void LabelCollector::appendData(QRectF rect)
{
    if(!(qAbs(rect.width())>2 && qAbs(rect.height())>2)) return;
    emit preItemAppended();
    LabelData *tmp = new LabelData(rect);
    m_dataVec.push_back(tmp);
    emit postItemAppended();
}
