#include "cvparam.h"

CvParam::CvParam(QObject *parent) : QObject(parent)
  , m_iteration(4)
  , m_epsilon(1)
{

}

int CvParam::iteration() const
{
    return m_iteration;
}

qreal CvParam::epsilon() const
{
    return m_epsilon;
}

void CvParam::setIteration(int iteration)
{
    if (m_iteration == iteration)
        return;

    m_iteration = iteration;
    emit iterationChanged(m_iteration);
}

void CvParam::setEpsilon(qreal epsilon)
{
    if (qFuzzyCompare(m_epsilon, epsilon))
        return;

    m_epsilon = epsilon;
    emit epsilonChanged(m_epsilon);
}
