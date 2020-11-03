#include "labelcollector.h"
#include <QtGlobal>

LabelCollector::LabelCollector(QQuickItem *parent) : QQuickPaintedItem(parent)
  , m_mouseEnabled(true)
  , m_mousePressed(false)
  , m_mouseMoved(false)
  , m_isLabelSelect(false)
  , m_penNormal(QPen(Qt::green, 3, Qt::SolidLine, Qt::SquareCap, Qt::MiterJoin))
  , m_penHighlight(QPen(Qt::red, 5, Qt::SolidLine, Qt::SquareCap, Qt::MiterJoin))
  , m_penPoint(QPen(Qt::black, 7, Qt::SolidLine, Qt::SquareCap, Qt::MiterJoin))
{
    setAcceptedMouseButtons(Qt::LeftButton);
    m_penVec.push_back(m_penNormal);
    m_penVec.push_back(m_penHighlight);
    m_penVec.push_back(m_penPoint);
}

void LabelCollector::paint(QPainter *painter){
    if(!this->m_imageScaled.isNull() ){
        qDebug() << Q_FUNC_INFO << " m_imageScaled width"<<m_imageScaled.width();
        qDebug() << Q_FUNC_INFO << " m_imageScaled height"<<m_imageScaled.height();
        QRect rect(m_imageScaled.rect());
        painter->drawImage(rect.topLeft(), m_imageScaled);
    }
    if(m_mouseMoved){
        painter->setPen(m_penNormal);
        painter->setRenderHint(QPainter::Antialiasing);
        painter->drawRect(QRectF(m_firstPoint,m_lastPoint));
    }
    if(m_dataVec.empty()) return;
    for(auto const &rect : m_dataVec){
        painter->setRenderHint(QPainter::Antialiasing);
        painter->setPen(m_penVec.at(rect->penIdx));
        painter->drawRect(QRectF(QPointF(rect->rect.tl().x,rect->rect.tl().y),QPointF(rect->rect.br().x,rect->rect.br().y)));
        qDebug() << Q_FUNC_INFO << "label class:"<<rect->labelClass;

        painter->setPen(m_penVec.at(2));
        for(auto& point : rect->result){
            painter->drawPoint(point);
            qDebug() << Q_FUNC_INFO << "draw point:"<<point;
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
    m_dataVec[labelIdx]->contoursPoly.assign(contoursPoly.begin(),contoursPoly.end());
    m_dataVec[labelIdx]->result.resize(m_dataVec[labelIdx]->contoursPoly.size());
    for(int i =0;i< m_dataVec[labelIdx]->contoursPoly.size();++i){
        cv::Point tmp = m_dataVec[labelIdx]->contoursPoly[i];
        m_dataVec[labelIdx]->result[i] = QPoint(tmp.x*(1.0f/getFactorScaled()),tmp.y*(1.0f/getFactorScaled()));
    }
    update();
    qDebug()<< Q_FUNC_INFO << "end";
}


bool LabelCollector::GetExistLabel(QPointF pt)
{
    bool res = false;
    cv::Point tmp(pt.x(),pt.y());
    m_selectLabelIdx.clear();
    QVector<LabelData*>::iterator it;
    for(it=m_dataVec.begin(); it!=m_dataVec.end(); it++){
        if((*it)->rect.contains(tmp)){
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
        qDebug() << Q_FUNC_INFO << " m_image width"<<m_image.width();
        qDebug() << Q_FUNC_INFO << " m_image height"<<m_image.height();
        qDebug() << Q_FUNC_INFO << "image scaled";
        qDebug() << Q_FUNC_INFO << " image width"<<image.width();
        qDebug() << Q_FUNC_INFO << " image height"<<image.height();
    }
    else{
        factorScaled = 1;
        this->m_image = image;
        qDebug() << Q_FUNC_INFO << "image not scaled";
    }
    imageWidth = m_imageScaled.width();
    imageHeight = m_imageScaled.height();
    qDebug() << Q_FUNC_INFO << " factor_scaled"<<factorScaled;
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
    }
    else
    {
        m_mousePressed = true;
        m_firstPoint = event->localPos();
        m_lastPoint = m_firstPoint;
        m_currentPoint = m_firstPoint;
        event->setAccepted(true);
    }
}

void LabelCollector::mouseMoveEvent(QMouseEvent *event)
{
    if(!m_mouseEnabled || !m_mousePressed)
    {
        QQuickPaintedItem::mousePressEvent(event);
    }
    else
    {
        m_mouseMoved = true;
        m_lastPoint = event->localPos();

        qDebug() << Q_FUNC_INFO << "m_firstPoint"<< m_firstPoint;
        qDebug() << Q_FUNC_INFO << "m_lastPoint"<< m_lastPoint;
        update();
    }
}

void LabelCollector::mouseReleaseEvent(QMouseEvent *event)
{
    if(!m_mouseEnabled || !(event->button() & acceptedMouseButtons()))
    {
        QQuickPaintedItem::mousePressEvent(event);
    }
    else
    {
        m_mousePressed = false;
        m_mouseMoved = false;

        if(m_firstPoint != m_lastPoint){
            cv::Point point_lt(qMin(m_firstPoint.x(),m_lastPoint.x()),qMin(m_firstPoint.y(),m_lastPoint.y()));
            cv::Point point_rb(qMax(m_firstPoint.x(),m_lastPoint.x()),qMax(m_firstPoint.y(),m_lastPoint.y()));
            cv::Rect  tmpRect(cv::Rect(point_lt,point_rb));
            qDebug() << "m_dataVec size: "<<m_dataVec.size();
            appendData(tmpRect);
            update();
            emit processRequest(m_dataVec.size()-1);
        }


    }
}

void LabelCollector::mouseDoubleClickEvent(QMouseEvent *event)
{
    qDebug() << Q_FUNC_INFO << " start";
    m_isLabelSelect = GetExistLabel(event->localPos());
    if(!m_isLabelSelect) return;
    qDebug() << "m_dataVec size: "<<m_dataVec.size();
    if(m_isLabelSelect){
        RemoveLabel(m_selectLabelIdx.front());
        m_selectLabelIdx.erase(m_selectLabelIdx.begin());
    }
    qDebug() << Q_FUNC_INFO << "finish";
}

void LabelCollector::appendData(cv::Rect rect)
{
    if(!(qAbs(rect.width)>2 && qAbs(rect.height)>2)) return;
    emit preItemAppended();
    LabelData *tmp = new LabelData(rect);
    m_dataVec.push_back(tmp);
    emit postItemAppended();
}
