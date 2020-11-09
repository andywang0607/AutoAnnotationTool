#include <QFile>
#include <QFileInfo>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QJsonArray>
#include <QBuffer>

#include "datasaver.h"

DataSaver::DataSaver(QObject *parent) : QObject(parent)
{

}

LabelCollector *DataSaver::labelCollector() const
{
    return m_labelCollector;
}

QString DataSaver::GetSavingPath()
{
    QString imgSrc = labelCollector()->imgSrc();
    QFileInfo info(imgSrc);
    QString savingPath = info.path() + "/" + info.baseName() + ".json";
    qDebug()<< Q_FUNC_INFO << "savingPath"<<savingPath;
    return savingPath;
}

QString DataSaver::GetImagePath()
{
    QFileInfo info(labelCollector()->imgSrc());
    return info.fileName();
}

QString DataSaver::Base64Encode()
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

int DataSaver::GetImageHeight()
{
    return labelCollector()->image().height();
}

int DataSaver::GetImageWidth()
{
    return labelCollector()->image().width();
}

void DataSaver::SaveData(int mode)
{
    QString savingPath = GetSavingPath();
    QFile file(savingPath);
    file.open(QIODevice::WriteOnly);
    QJsonParseError JsonParseError;
    QJsonDocument JsonDocument = QJsonDocument::fromJson(file.readAll(), &JsonParseError);
    file.close();
    QJsonObject RootObject = JsonDocument.object();
    RootObject.insert("version", "0.0.1");

    // Get imagePath
    RootObject.insert("imagePath",GetImagePath());

    // Get imageData
    RootObject.insert("imageData", Base64Encode());

    // Get imageHeight
    RootObject.insert("imageHeight", GetImageHeight());

    // Get imageWidth
    RootObject.insert("imageWidth", GetImageWidth());

    // flags
    RootObject.insert("flags",QJsonObject());

    // Save bounding box data
    int boxNum = labelCollector()->dataVec().size();
    qDebug()<< Q_FUNC_INFO << "boxNum"<<boxNum;
    QJsonArray shape;

    // rectangle data
    if(mode == 0 || mode ==1){
        qreal factorScaled = labelCollector()->getFactorScaled();
        for(int i =0 ;i<boxNum;++i){
            QJsonObject rectInfo;
            rectInfo.insert("label",labelCollector()->dataVec().at(i)->labelClass);
            rectInfo.insert("group_id",QJsonValue::Null);
            rectInfo.insert("shape_type","rectangle");
            rectInfo.insert("flags",QJsonObject());
            QJsonArray ptArray;
            QJsonArray tlArray;
            tlArray.append(labelCollector()->dataVec().at(i)->rect.tl().x * factorScaled);
            tlArray.append(labelCollector()->dataVec().at(i)->rect.tl().y * factorScaled);
            QJsonArray brArray;
            brArray.append(labelCollector()->dataVec().at(i)->rect.br().x * factorScaled);
            brArray.append(labelCollector()->dataVec().at(i)->rect.br().y * factorScaled);
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
            int ptNum = labelCollector()->dataVec().at(i)->contoursPoly.size();
            for(int j = 0; j<ptNum; ++j){
                QJsonArray curPtArray;
                curPtArray.append(labelCollector()->dataVec().at(i)->contoursPoly.at(j).x);
                curPtArray.append(labelCollector()->dataVec().at(i)->contoursPoly.at(j).y);
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

void DataSaver::setLabelCollector(LabelCollector *labelCollector)
{
    if (m_labelCollector == labelCollector)
        return;

    m_labelCollector = labelCollector;
    emit labelCollectorChanged(m_labelCollector);
}

