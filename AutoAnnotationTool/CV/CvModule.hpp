#ifndef CVMODULE_H
#define CVMODULE_H

#include "SegmentationPolicy/Grabcut.hpp"

template <typename SegmentationPolicy = Grabcut>
class CvModule : public SegmentationPolicy
{
public:
    void updatePolyToLabelData(LabelData *labelData, qreal factor, CvParam *param)
    {
        labelData->poly = this->getPoly(labelData, factor, param);
        qDebug() << Q_FUNC_INFO << "finish";
    }
};

#endif // CVMODULE_H
