#ifndef THINNINGMULTITHREAD_H
#define THINNINGMULTITHREAD_H

#include <QObject>
#include "preprocessing_config.h"
#include "thinning.h"

class ThinningMultithread : public QObject
{
    Q_OBJECT
public:
    explicit ThinningMultithread(QObject *parent = nullptr);
    void setParams(QVector<cv::Mat1b> binaryImages);
    void thin();

    QVector<cv::Mat1b> getSkeletons;
private:
    int threadsFinished;
    QVector<QThread*> threads;
    //Input
    QVector<cv::Mat1b> binaryImages;
    //Output
    QVector<cv::Mat1b> skeletons;
signals:
    void thinningMultithreadDone();
public slots:
    void oneThinningThreadDone();
};

#endif // THINNINGMULTITHREAD_H
