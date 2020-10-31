#ifndef CVMODULE_H
#define CVMODULE_H

#include <QObject>

class CVModule : public QObject
{
    Q_OBJECT
public:
    explicit CVModule(QObject *parent = nullptr);

signals:

};

#endif // CVMODULE_H
