#ifndef CVMODULE_H
#define CVMODULE_H

#include <QObject>

#include <vector>
#include <string>

#include "labelcollector.h"

class CVModule : public QObject
{
    Q_OBJECT
    Q_PROPERTY(LabelCollector * labelCollector READ labelCollector WRITE setLabelCollector NOTIFY labelCollectorChanged)
    LabelCollector * m_labelCollector;
public:
    explicit CVModule(QObject *parent = nullptr);

    LabelCollector * labelCollector() const;
public:

    Q_INVOKABLE void GetCroppedImg(int labelIdx);


public slots:
    void setLabelCollector(LabelCollector * labelCollector);
    void GetOriginImg(QString imgSrc);
signals:

    void labelCollectorChanged(LabelCollector * labelCollector);
private:
    cv::Mat m_imgOri;
};

#endif // CVMODULE_H
