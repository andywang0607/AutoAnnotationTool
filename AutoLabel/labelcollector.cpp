#include "labelcollector.h"

LabelCollector::LabelCollector(QQuickItem *parent) : QQuickPaintedItem(parent)
{

}

void LabelCollector::paint(QPainter *painter){
    if(!this->m_imageScaled.isNull() ){
        qDebug() << Q_FUNC_INFO << " m_imageScaled width"<<m_imageScaled.width();
        qDebug() << Q_FUNC_INFO << " m_imageScaled height"<<m_imageScaled.height();
        QRect rect(m_imageScaled.rect());
        painter->drawImage(rect.topLeft(), m_imageScaled);
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
