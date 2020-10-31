#include "cvmodule.h"

#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

CVModule::CVModule(QObject *parent) : QObject(parent)
{

}

LabelCollector *CVModule::labelCollector() const
{
    return m_labelCollector;
}

void CVModule::GetCroppedImg(int labelIdx)
{
    QVector<LabelData*> dataVecTmp = labelCollector()->dataVec();
    qreal factorScaled = labelCollector()->getFactorScaled();
    cv::Rect rectOri = cv::Rect(dataVecTmp.at(labelIdx)->rect.tl() * factorScaled,
                                dataVecTmp.at(labelIdx)->rect.br() * factorScaled);
    cv::Mat croppedImg = m_imgOri(rectOri);
    cv::namedWindow("Cropped");
    cv::imshow("Cropped", croppedImg);

}

void CVModule::GetOriginImg(QString imgSrc)
{
    qDebug()<< Q_FUNC_INFO << "imgSrc" << imgSrc;
    m_imgOri = cv::imread(imgSrc.toStdString(),1);
}

void CVModule::setLabelCollector(LabelCollector *labelCollector)
{
    qDebug()<< Q_FUNC_INFO << "Start";
    if (m_labelCollector == labelCollector)
        return;

    m_labelCollector = labelCollector;
    emit labelCollectorChanged(m_labelCollector);
    QObject::connect(m_labelCollector, &LabelCollector::imgSrcChanged, this, &CVModule::GetOriginImg);
}
