#include "cvparam.h"

CVParam::CVParam(QObject *parent) : QObject(parent)
  , m_iteration(4)
  , m_epsilon(1)
{

}

int CVParam::iteration() const
{
    return m_iteration;
}

qreal CVParam::epsilon() const
{
    return m_epsilon;
}

void CVParam::setIteration(int iteration)
{
    if (m_iteration == iteration)
        return;

    m_iteration = iteration;
    emit iterationChanged(m_iteration);
}

void CVParam::setEpsilon(qreal epsilon)
{
    if (qFuzzyCompare(m_epsilon, epsilon))
        return;

    m_epsilon = epsilon;
    emit epsilonChanged(m_epsilon);
}
