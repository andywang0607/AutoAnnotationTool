#include "include/LableData.h"
#include "QItem/CvParam.h"
#include "Type/TypeConverter.hpp"

#include <QDebug>
#include <QPolygon>

#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

class Grabcut
{
public:
    Grabcut() = default;

    QPolygon getPoly(LabelData *labelData, qreal factor, CvParam *param)
    {
        qDebug() << Q_FUNC_INFO << "start";
        if (m_originImg.empty())
            return QPolygon();

        // grabcut
        cv::Mat grab, bg, fg;
        cv::grabCut(m_originImg, grab, getRoiRect(labelData->rect, factor), bg, fg, param->iteration(), cv::GC_INIT_WITH_RECT);

        // equalizeHist
        equalizeHist(grab, grab);

        // binary
        cv::Mat gray(grab.size(), CV_8U, cv::Scalar(255));
        threshold(grab, gray, 200, 256, cv::THRESH_BINARY); // convert to binary

        std::vector<std::vector<cv::Point>> contours;
        cv::findContours(gray,
                         contours,               // a vector of contours
                         cv::RETR_EXTERNAL,      // retrieve the external contours
                         cv::CHAIN_APPROX_NONE); // retrieve all pixels of each contours

        // Print contours' length
        qDebug() << "Contours: " << contours.size();

        // find max contour area
        int largestArea = 0;
        int largestContourIndex = 0;
        for (int i = 0; i < contours.size(); i++) // Iterate through each contour
        {
            double area = contourArea(contours[i], false); // Find the area of contour
            if (area > largestArea)
            {
                largestArea = area;
                largestContourIndex = i; // Store the index of largest contour
            }
        }
        std::vector<std::vector<cv::Point>> contoursPolys(contours.size());
        approxPolyDP(cv::Mat(contours[largestContourIndex]), contoursPolys[largestContourIndex], param->epsilon(), true);
        
        QPolygon result;
        const auto &contoursPoly = contoursPolys[largestContourIndex];

        for (const auto &point : contoursPoly)
        {
            result.push_back(Type::fromPoint<cv::Point>(point) * (1.0f / factor));
        }

        qDebug() << Q_FUNC_INFO << "end";

        return result;
    }

    void getOriginImg(const QString &imgSrc)
    {
        m_originImg = cv::imread(imgSrc.toStdString(), 1);
    }

    cv::Rect getRoiRect(const QRectF &rect, qreal factor)
    {
        const QRectF rectOri(rect.topLeft() * factor,
                             rect.bottomRight() * factor);
        return Type::toRect<cv::Rect>(rectOri);
    }

private:
    cv::Mat m_originImg;
};
