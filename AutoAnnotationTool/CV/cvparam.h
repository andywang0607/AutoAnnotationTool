#ifndef CVPARAM_H
#define CVPARAM_H

#include <QObject>

class CVParam : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int iteration READ iteration WRITE setIteration NOTIFY iterationChanged)
    Q_PROPERTY(qreal epsilon READ epsilon WRITE setEpsilon NOTIFY epsilonChanged)

public:
    explicit CVParam(QObject *parent = nullptr);

    int iteration() const;

    qreal epsilon() const;

public slots:
    void setIteration(int iteration);

    void setEpsilon(qreal epsilon);

signals:

    void iterationChanged(int iteration);
    void epsilonChanged(qreal epsilon);

private:
    int m_iteration;
    qreal m_epsilon;
};

#endif // CVPARAM_H
