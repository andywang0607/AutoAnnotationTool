#ifndef DATASAVER_H
#define DATASAVER_H

#include <QObject>
#include <labelcollector.h>
class DataSaver : public QObject
{
    Q_OBJECT
    Q_PROPERTY(LabelCollector * labelCollector READ labelCollector WRITE setLabelCollector NOTIFY labelCollectorChanged)

public:
    explicit DataSaver(QObject *parent = nullptr);
    LabelCollector * labelCollector() const;
public:
    Q_INVOKABLE void SaveData(int mode); //0:rectangle, 1:polygon
private:
    QString GetSavingPath();
    QString GetImagePath();
    QString Base64Encode();
    int GetImageHeight();
    int GetImageWidth();

public slots:
    void setLabelCollector(LabelCollector * labelCollector);

signals:
    void labelCollectorChanged(LabelCollector * labelCollector);

private:
    LabelCollector * m_labelCollector;


};

#endif // DATASAVER_H
