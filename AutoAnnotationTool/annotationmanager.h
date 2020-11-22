#ifndef ANNOTATIONMANAGER_H
#define ANNOTATIONMANAGER_H

#include <QObject>
#include <labelcollector.h>
class AnnotationManager : public QObject
{
    Q_OBJECT
    Q_PROPERTY(LabelCollector * labelCollector READ labelCollector WRITE setLabelCollector NOTIFY labelCollectorChanged)

public:
    explicit AnnotationManager(QObject *parent = nullptr);
    LabelCollector * labelCollector() const;
public:
    Q_INVOKABLE void saveAnnotation(int mode); //0:all, 1:rectangle, 2:polygon
    Q_INVOKABLE void loadAnnotation(int mode); //0:all, 1:rectangle, 2:polygon
private:
    QString getSavingPath();
    QString getImagePath();
    QString base64Encode();
    int getImageHeight();
    int getImageWidth();

public slots:
    void setLabelCollector(LabelCollector * labelCollector);

signals:
    void labelCollectorChanged(LabelCollector * labelCollector);

private:
    LabelCollector * m_labelCollector;


};

#endif // ANNOTATIONMANAGER_H
