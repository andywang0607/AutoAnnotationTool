#include "LabelCollector.h"
#include "CV/GeometryModule.h"

#include <QtGlobal>

static const int thresDistance = 5;

LabelCollector::LabelCollector(QQuickItem *parent) : QQuickPaintedItem(parent)
  , m_mouseEnabled(true)
  , m_mousePressed(false)
  , m_mouseMoved(false)
  , m_normalPen(QPen(Qt::green, 3, Qt::SolidLine, Qt::SquareCap, Qt::MiterJoin))
  , m_highlightPen(QPen(Qt::red, 5, Qt::SolidLine, Qt::SquareCap, Qt::MiterJoin))
  , m_pointPen(QPen(QColor(220,118,51), 7, Qt::SolidLine, Qt::SquareCap, Qt::MiterJoin))
  , m_polyPen(QPen(Qt::yellow , 3, Qt::SolidLine, Qt::SquareCap, Qt::MiterJoin))
  , m_extensivePen(QPen(QColor("#556b2f"), 1, Qt::DashLine, Qt::SquareCap, Qt::MiterJoin)) // darkolivegreen
  , m_cvModule(std::make_unique<CvModule>())
{
    setAcceptedMouseButtons(Qt::AllButtons);
    m_penVec.push_back(m_normalPen);
    m_penVec.push_back(m_highlightPen);
    m_penVec.push_back(m_pointPen);
    m_penVec.push_back(m_polyPen);

    connect(&m_watcher, &QFutureWatcher<void>::finished, this, [&]() {
        this->update();
        setCursor(QCursor(Qt::ArrowCursor));
    });

    menu.addAction(QStringLiteral("Get Polygon"),this, [&]() {
        setCursor(QCursor(Qt::BusyCursor));
        m_future = QtConcurrent::run(m_cvModule.get(), &CvModule::getPoly, m_dataVec, m_selectLabelIdx.front(), getFactorScaled(), m_cvParam);
        m_watcher.setFuture(m_future);
    });

}

void LabelCollector::paint(QPainter *painter)
{
    if (!this->m_scaledImg.isNull()) {
        QRect rect(m_scaledImg.rect());
        painter->drawImage(rect.topLeft(), m_scaledImg);
    }
    if (m_selectLabelIdx.empty()) {
        if (m_mouseMoved) {
            // Draw a horizontal and vertical dash line to show cursor point
            painter->setPen(m_extensivePen);
            painter->drawLine(m_lastPoint.x(), 0, m_lastPoint.x(), m_scaledImg.height());
            painter->drawLine(0, m_lastPoint.y(), m_scaledImg.width(), m_lastPoint.y());
            painter->setPen(m_normalPen);
            painter->setRenderHint(QPainter::Antialiasing);
            painter->drawRect(QRectF(m_firstPoint,m_lastPoint));
        }
    }
    if (m_dataVec.empty())
        return;
    // Draw a horizontal and vertical dash line to show cursor point while adjust rectangle
    if(!m_polySelectResult.isSelect && m_rectCornerSelectResult.isSelect) {
        painter->setPen(m_extensivePen);
        painter->drawLine(m_lastPoint.x(), 0, m_lastPoint.x(), m_scaledImg.height());
        painter->drawLine(0, m_lastPoint.y(), m_scaledImg.width(), m_lastPoint.y());
    } else if (!m_polySelectResult.isSelect && !m_rectCornerSelectResult.isSelect && m_rectEdgeSelectResult.isSelect) {
        painter->setPen(m_extensivePen);
        switch (m_rectEdgeSelectResult.line) {
        case 0:
            painter->drawLine(m_lastPoint.x(), 0, m_lastPoint.x(), m_scaledImg.height());
            break;
        case 1:
            painter->drawLine(0, m_lastPoint.y(), m_scaledImg.width(), m_lastPoint.y());
            break;
        case 2:
            painter->drawLine(m_lastPoint.x(), 0, m_lastPoint.x(), m_scaledImg.height());
            break;
        case 3:
            painter->drawLine(0, m_lastPoint.y(), m_scaledImg.width(), m_lastPoint.y());
            break;
        default:
            break;
        }
    }

    QVector<LabelData*>::iterator iter;
    for (iter=m_dataVec.begin();iter!=m_dataVec.end();iter++) {
        // Draw bounding box
        painter->setRenderHint(QPainter::Antialiasing);
        painter->setPen(m_penVec.at((*iter)->penIdx));
        painter->drawRect((*iter)->rect);
        // Draw result polygon
        painter->setPen(m_penVec.at(3));
        painter->drawPolygon((*iter)->poly);

        // Draw result point
        painter->setPen(m_penVec.at(2));
        QVector<QPointF>::iterator pointIter;
        int dataIdx = std::distance(m_dataVec.begin(),iter);
        for (pointIter=(*iter)->poly.begin();pointIter!=(*iter)->poly.end();pointIter++) {
            int pointIdx = std::distance((*iter)->poly.begin(),pointIter);
            if (dataIdx == m_polySelectResult.boxIdx && pointIdx == m_polySelectResult.polyIdx) {
                QRadialGradient gradient(*pointIter,5,*pointIter);
                gradient.setColorAt(0,QColor(220,118,51));
                gradient.setColorAt(0.5,QColor(229,152,102));
                gradient.setColorAt(1,QColor(237,187,153));
                painter->setPen(QColor(237,187,153));
                painter->setBrush(QBrush(gradient));
                painter->drawEllipse(*pointIter,5,5);
            } else {
                painter->setPen(m_penVec.at(2));
                painter->setBrush(QBrush(Qt::NoBrush));
                painter->drawPoint(*pointIter);
            }
        }
    }
}

void LabelCollector::mousePressEvent(QMouseEvent *event)
{
    m_mouseMoved = false;
    if (!(event->button() & acceptedMouseButtons())){
        QQuickPaintedItem::mousePressEvent(event);
        return;
    }
    if (m_scaledImg.isNull())
        return;
    getExistLabel(event->localPos());
    if (Qt::LeftButton == event->button()) {
        m_mousePressed = true;
        GeometryModule::getPolygonSelectResult(event->localPos(), m_dataVec, m_polySelectResult);
        GeometryModule::getRectCornerResult(event->localPos(), m_dataVec, m_rectCornerSelectResult);
        GeometryModule::getRectEdgeResult(event->localPos(), m_dataVec, m_rectEdgeSelectResult);
        m_firstPoint = event->localPos();
        m_lastPoint = m_firstPoint;
        m_currentPoint = m_firstPoint;
        setCursorIcon();
        event->setAccepted(true);
    }
    if (Qt::RightButton == event->button()) {
        if(m_selectLabelIdx.empty())
            return;
        menu.exec(QCursor::pos());
    }
}

void LabelCollector::mouseMoveEvent(QMouseEvent *event)
{
    if (!m_mouseEnabled || !m_mousePressed) {
        QQuickPaintedItem::mousePressEvent(event);
        return;
    }
    if (Qt::RightButton == event->button())
        return;
    m_lastPoint = event->localPos();
    GeometryModule::posBoundaryCheck(m_lastPoint, m_scaledImg.width(), m_scaledImg.height());
    if (m_polySelectResult.isSelect) {
        m_dataVec.at(m_polySelectResult.boxIdx)->poly[m_polySelectResult.polyIdx] = m_lastPoint;
    } else if (!m_polySelectResult.isSelect && m_rectCornerSelectResult.isSelect) {
        switch (m_rectCornerSelectResult.corner) {
        case 0:
            m_dataVec.at(m_rectCornerSelectResult.boxIdx)->rect.setTopLeft(m_lastPoint);
            break;
        case 1:
            m_dataVec.at(m_rectCornerSelectResult.boxIdx)->rect.setTopRight(m_lastPoint);
            break;
        case 2:
            m_dataVec.at(m_rectCornerSelectResult.boxIdx)->rect.setBottomRight(m_lastPoint);
            break;
        case 3:
            m_dataVec.at(m_rectCornerSelectResult.boxIdx)->rect.setBottomLeft(m_lastPoint);
            break;
        default:
            break;
        }
    } else if (!m_polySelectResult.isSelect && !m_rectCornerSelectResult.isSelect && m_rectEdgeSelectResult.isSelect) {
        switch (m_rectEdgeSelectResult.line){
        case 0:
            m_dataVec.at(m_rectEdgeSelectResult.boxIdx)->rect.setLeft(m_lastPoint.x());
            break;
        case 1:
            m_dataVec.at(m_rectEdgeSelectResult.boxIdx)->rect.setTop(m_lastPoint.y());
            break;
        case 2:
            m_dataVec.at(m_rectEdgeSelectResult.boxIdx)->rect.setRight(m_lastPoint.x());
            break;
        case 3:
            m_dataVec.at(m_rectEdgeSelectResult.boxIdx)->rect.setBottom(m_lastPoint.y());
            break;
        default:
            break;
        }
    } else if (!m_selectLabelIdx.empty()) {
        QPointF offset = m_lastPoint - m_currentPoint;
        m_currentPoint = m_lastPoint;
        for(auto const &idx : m_selectLabelIdx){
            m_dataVec.at(idx)->rect.translate(offset);
            if (!GeometryModule::rectBoundaryCheck(m_dataVec.at(idx)->rect, m_scaledImg.width(), m_scaledImg.height()))
                m_dataVec.at(idx)->rect.translate(-offset);
        }
    } else {
        m_mouseMoved = true;
        m_lastPoint = event->localPos();
    }
    update();
}

void LabelCollector::mouseReleaseEvent(QMouseEvent *event)
{
    if (!m_mouseEnabled || !(event->button() & acceptedMouseButtons())) {
        QQuickPaintedItem::mousePressEvent(event);
        return;
    }
    setCursor(QCursor(Qt::ArrowCursor));
    if (Qt::RightButton == event->button())
        return;
    m_mousePressed = false;
    m_mouseMoved = false;
    GeometryModule::preventInvalidRect(m_dataVec, m_rectCornerSelectResult, m_rectEdgeSelectResult);
    if (m_polySelectResult.isSelect || m_rectEdgeSelectResult.isSelect || m_rectCornerSelectResult.isSelect) {
        m_polySelectResult.isSelect = false;
        m_rectEdgeSelectResult.isSelect = false;
        m_rectCornerSelectResult.isSelect = false;
        update();
        return;
    }
    if (!m_selectLabelIdx.empty())
        return;
    if (m_firstPoint != m_lastPoint){
        QPoint point_lt(qMin(m_firstPoint.x(),m_lastPoint.x()),qMin(m_firstPoint.y(),m_lastPoint.y()));
        QPoint point_rb(qMax(m_firstPoint.x(),m_lastPoint.x()),qMax(m_firstPoint.y(),m_lastPoint.y()));
        appendData(QRectF(point_lt,point_rb));
        update();
    }
}

void LabelCollector::removeLabel(int idx)
{
    emit preItemRemoved(idx);
    delete m_dataVec.at(idx);
    m_dataVec[idx] = nullptr;
    m_dataVec.erase(m_dataVec.begin()+idx);
    emit postItemRemoved();
    update();
}

bool LabelCollector::getExistLabel(QPointF pt)
{
    bool res = false;
    m_selectLabelIdx.clear();
    QVector<LabelData*>::iterator it;
    for (it=m_dataVec.begin(); it!=m_dataVec.end(); it++) {
        if((*it)->rect.contains(pt.toPoint())){
            m_selectLabelIdx.push_back(std::distance(m_dataVec.begin(),it));
            (*it)->isSelect = true;
            res = true;
        }
    }
    return res;
}

void LabelCollector::removeAllLabel()
{
    int size = m_dataVec.size();
    for (int i=0;i<size;++i)
        removeLabel(0);
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
    return m_scaledRatio;
}

int LabelCollector::fileIdx() const
{
    return m_fileIdx;
}

CvParam *LabelCollector::cvParam() const
{
    return m_cvParam;
}

void LabelCollector::setImage(const QImage &image){
    if (image.isNull())
        return;
    m_image = image;
    if (image.width() != this->width() && image.height() != this->height()) {
        m_scaledImg = image.scaled(this->width(), this->height(), Qt::KeepAspectRatio);
        m_scaledRatio = qMin((float)image.width() / (float)m_scaledImg.width(),
                             (float)image.height() / (float)m_scaledImg.height());
    } else{
        m_scaledRatio = 1;
        this->m_image = image;
    }
    m_imgWidth = m_scaledImg.width();
    m_imgHeight = m_scaledImg.height();
    // Redraw the image
    update();
    // Remove all label while iamge change
    removeAllLabel();
    emit imageChanged();
}

void LabelCollector::setImgSrc(QString imgSrc)
{
    if (m_imgSrc == imgSrc)
        return;
    m_imgSrc = imgSrc;
    if (m_imgSrc.left(4) == "file"){
#ifdef _WIN32
        m_imgSrc = m_imgSrc.remove(0,8);
#else
        m_imgSrc = m_imgSrc.remove(0,7);
#endif
    }
    QFileInfo fi(m_imgSrc);
    if (fi.isFile()) {
        setImage(QImage(m_imgSrc));
    } else {
        QDir dir(m_imgSrc);
        QStringList filters;
        filters << "*.jpg" << "*.png" << "*.bmp";
        dir.setNameFilters(filters);
        m_fileInfoList = dir.entryInfoList(QDir::Files | QDir::Readable);
        setFileIdx(0);
        setImgSrc(m_fileInfoList.at(fileIdx()).absoluteFilePath());
    }
    emit imgSrcChanged(m_imgSrc);
    m_cvModule->getOriginImg(m_imgSrc);
}

void LabelCollector::setCursorIcon()
{
    if (m_polySelectResult.isSelect){
        setCursor(QCursor(Qt::ArrowCursor));
    } else if (m_rectCornerSelectResult.isSelect){
        switch (m_rectCornerSelectResult.corner){
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
    else if (m_rectEdgeSelectResult.isSelect){
        switch (m_rectEdgeSelectResult.line){
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
    else if (!m_selectLabelIdx.empty()){
        setCursor(QCursor(Qt::ClosedHandCursor));
    }
    else{
        setCursor(QCursor(Qt::CrossCursor));
    }
}

void LabelCollector::appendData(QRectF rect)
{
    if (!(qAbs(rect.width())>2 && qAbs(rect.height())>2))
        return;
    emit preItemAppended();
    LabelData *tmp = new LabelData(rect);
    m_dataVec.push_back(tmp);
    emit postItemAppended();
}

void LabelCollector::appendData(QRectF rect, QString labelClass)
{
    if (!(qAbs(rect.width())>2 && qAbs(rect.height())>2))
        return;
    emit preItemAppended();
    LabelData *tmp = new LabelData(rect, labelClass);
    m_dataVec.push_back(tmp);
    emit postItemAppended();
}

void LabelCollector::appendData(QPolygonF poly, QString labelClass)
{
    emit preItemAppended();
    LabelData *tmp = new LabelData(poly, labelClass);
    m_dataVec.push_back(tmp);
    emit postItemAppended();
}

void LabelCollector::appendData(QRectF rect, QPolygonF poly, QString labelClass)
{
    emit preItemAppended();
    LabelData *tmp = new LabelData(rect, poly, labelClass);
    m_dataVec.push_back(tmp);
    emit postItemAppended();
}

void LabelCollector::setFileIdx(int fileIdx)
{
    if (m_fileIdx == fileIdx)
        return;
    if (fileIdx < 0)
        return;
    if (fileIdx >= m_fileInfoList.count())
        return;
    m_fileIdx = fileIdx;
    emit fileIdxChanged(m_fileIdx);
    setImgSrc(m_fileInfoList.at(m_fileIdx).absoluteFilePath());
}

void LabelCollector::setCvParam(CvParam *cvParam)
{
    if (m_cvParam == cvParam)
        return;

    m_cvParam = cvParam;
    emit cvParamChanged(m_cvParam);
}
