#include <QFile>
#include <QFileInfo>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QJsonArray>
#include <QBuffer>

#include "annotationmanager.h"

AnnotationManager::AnnotationManager(QObject *parent) : QObject(parent)
{

}

LabelCollector *AnnotationManager::labelCollector() const
{
    return m_labelCollector;
}

QString AnnotationManager::getSavingPath()
{
    QString imgSrc = labelCollector()->imgSrc();
    QFileInfo info(imgSrc);
    QString savingPath = info.path() + "/" + info.baseName() + ".json";
    return savingPath;
}

QString AnnotationManager::getImagePath()
{
    QFileInfo info(labelCollector()->imgSrc());
    return info.fileName();
}

QString AnnotationManager::base64Encode()
{
    QByteArray ba;
    QBuffer buf(&ba);

    // get file extension
    QFileInfo info(labelCollector()->imgSrc());
    QString extension = info.suffix();
    labelCollector()->image().save(&buf, extension.toUtf8());
    QByteArray hexed = ba.toBase64();
    buf.close();
    return hexed;
}

int AnnotationManager::getImageHeight()
{
    return labelCollector()->image().height();
}

int AnnotationManager::getImageWidth()
{
    return labelCollector()->image().width();
}

void AnnotationManager::saveAnnotation(int mode)
{
    QString savingPath = getSavingPath();
    QFile file(savingPath);
    file.open(QIODevice::WriteOnly);
    QJsonParseError JsonParseError;
    QJsonDocument JsonDocument = QJsonDocument::fromJson(file.readAll(), &JsonParseError);
    file.close();
    QJsonObject RootObject = JsonDocument.object();
    RootObject.insert("version", "0.0.1");

    // Get imagePath
    RootObject.insert("imagePath",getImagePath());

    // Get imageData
    RootObject.insert("imageData", base64Encode());

    // Get m_imgHeight
    RootObject.insert("m_imgHeight", getImageHeight());

    // Get m_imgWidth
    RootObject.insert("m_imgWidth", getImageWidth());

    // flags
    RootObject.insert("flags",QJsonObject());

    // Save bounding box data
    int boxNum = labelCollector()->dataVec().size();
    QJsonArray shape;

    // rectangle data
    if(mode == 0 || mode ==1){
        qreal m_scaledRatio = labelCollector()->getFactorScaled();
        for(int i =0 ;i<boxNum;++i){
            QJsonObject rectInfo;
            rectInfo.insert("label",labelCollector()->dataVec().at(i)->labelClass);
            rectInfo.insert("group_id",QJsonValue::Null);
            rectInfo.insert("shape_type","rectangle");
            rectInfo.insert("flags",QJsonObject());
            QJsonArray ptArray;
            QJsonArray tlArray;
            tlArray.append(labelCollector()->dataVec().at(i)->rect.topLeft().x() * m_scaledRatio);
            tlArray.append(labelCollector()->dataVec().at(i)->rect.topLeft().y() * m_scaledRatio);
            QJsonArray brArray;
            brArray.append(labelCollector()->dataVec().at(i)->rect.bottomRight().x() * m_scaledRatio);
            brArray.append(labelCollector()->dataVec().at(i)->rect.bottomRight().y() * m_scaledRatio);
            ptArray.append(tlArray);
            ptArray.append(brArray);
            rectInfo.insert("points",ptArray);
            shape.append(rectInfo);
        }
    }
    // polygon data
    if(mode == 0 || mode ==2){
        for(int i =0 ;i<boxNum;++i){
            QJsonObject polyInfo;
            polyInfo.insert("flags",QJsonObject());
            polyInfo.insert("label",labelCollector()->dataVec().at(i)->labelClass);
            polyInfo.insert("group_id",QJsonValue::Null);
            polyInfo.insert("shape_type","polygon");
            QJsonArray ptArray;
            int ptNum = labelCollector()->dataVec().at(i)->poly.size();
            for(int j = 0; j<ptNum; ++j){
                QJsonArray curPtArray;
                curPtArray.append(labelCollector()->dataVec().at(i)->poly.at(j).x() * labelCollector()->getFactorScaled());
                curPtArray.append(labelCollector()->dataVec().at(i)->poly.at(j).y() * labelCollector()->getFactorScaled());
                ptArray.append(curPtArray);
            }
            polyInfo.insert("points",ptArray);
            shape.append(polyInfo);
        }
    }
    RootObject.insert("shapes", shape);

    // Write file
    JsonDocument.setObject(RootObject); // set to json document
    file.open(QFile::WriteOnly | QFile::Text | QFile::Truncate);
    file.write(JsonDocument.toJson());
    file.close();
    return;
}

void AnnotationManager::loadAnnotation(int mode)
{
    QFileInfo fi(getSavingPath());
    if(!fi.exists()) return;
    QFile file(getSavingPath());
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    QJsonParseError JsonParseError;
    QJsonDocument JsonDocument = QJsonDocument::fromJson(file.readAll(), &JsonParseError);
    file.close();

    QJsonObject rootObject = JsonDocument.object();
    QJsonValueRef ref = rootObject.find("shapes").value();
    QJsonObject m_addvalue = ref.toObject();
    QJsonArray shapeAll = ref.toArray();

    QStringList labelName;

    for (const auto& element : shapeAll) {
        QString label = element.toObject()["label"].toString();
        if(!labelName.contains(label)) labelName.push_back(label);
        QString shapeType = element.toObject()["shape_type"].toString();
        QJsonArray pointArray = element.toObject()["points"].toArray();
        if(shapeType == "rectangle"){
            QPointF tl(pointArray[0].toArray()[0].toDouble() / labelCollector()->getFactorScaled(),
                    pointArray[0].toArray()[1].toDouble() / labelCollector()->getFactorScaled());
            QPointF br(pointArray[1].toArray()[0].toDouble() / labelCollector()->getFactorScaled(),
                    pointArray[1].toArray()[1].toDouble() / labelCollector()->getFactorScaled());
            QRectF tmpRect(tl, br);
            labelCollector()->appendData(tmpRect, label);
        }
        else if(shapeType == "polygon"){
            QPolygonF tmpPoly;
            for (const auto& point : pointArray) {
                QJsonArray pointArray = point.toArray();
                QPointF polyPoint(pointArray[0].toDouble() / labelCollector()->getFactorScaled(),
                        pointArray[1].toDouble() / labelCollector()->getFactorScaled());
                tmpPoly.append(polyPoint.toPoint());
            }
            labelName.contains(label) ? labelCollector()->dataVec().at(labelName.indexOf(label))->poly.swap(tmpPoly) :
                                        labelCollector()->appendData(tmpPoly,label);
        }
    }
}

void AnnotationManager::setLabelCollector(LabelCollector *labelCollector)
{
    if (m_labelCollector == labelCollector)
        return;

    m_labelCollector = labelCollector;
    emit labelCollectorChanged(m_labelCollector);
}

