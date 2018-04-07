#ifndef THINNING_H
#define THINNING_H

#include <QObject>

#include <deque>

#include "opencv2/opencv.hpp"
#include "imagecontour.h"

typedef bool (*VoronoiFn)(uchar*  skeldata, int iter, int col, int row, int cols);

class Thinning : public QObject
{
    Q_OBJECT

public:
    explicit Thinning(QObject *parent = nullptr);

    static const int NOLIMIT = INT_MAX;

    bool thinGuoHallFast(const cv::Mat1b& img, bool inverted, bool crop_img_before = true, int max_iters = NOLIMIT);

    //getNset
    cv::Mat getImgSkeleton() const;
    cv::Mat getImgSkeletonInverted() const;

private:

    cv::Mat1b imgSkeleton;
    cv::Mat1b imgSkeletonInverted;

    cv::Rect _bbox;
    ImageContour skelcontour;

    std::deque<int> cols_to_set;
    std::deque<int> rows_to_set;
    bool _has_converged;

    bool thin_fast_custom_voronoi_fn(const cv::Mat1b& img, bool inverted, VoronoiFn voronoi_fn, bool crop_img_before = true, int max_iters = NOLIMIT);
    cv::Rect copy_bounding_box_plusone(const cv::Mat1b& img, cv::Mat1b& out, bool crop_img_before = true);
    template<class _T> cv::Rect boundingBox(const cv::Mat_<_T> & img);
    cv::Mat invertColor(const cv::Mat img);

    cv::Mat thinningGuoHallIteration(cv::Mat imgThin, int iter);
};

#endif // THINNING_H