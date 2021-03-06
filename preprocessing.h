#ifndef PREPROCESSING_H
#define PREPROCESSING_H
#define _USE_MATH_DEFINES

#include "preprocessing_global.h"
#include "preprocessing_config.h"

#include "orientationmap.h"
#include "gaborfiltermultithread.h"
#include "gaborfiltergpu.h"
#include "binarization.h"
#include "contrastenhancement.h"
#include "thinning.h"
#include "frequencymap.h"
#include "mask.h"
#include "qualitymap.h"

//batch mode
#include "contrastbatch.h"
#include "maskbatch.h"
#include "binarizationbatch.h"
#include "thinningmultithread.h"

typedef struct preprocessing_all_results {
    cv::Mat imgOriginal;
    cv::Mat imgContrastEnhanced;
    cv::Mat imgEnhanced; // obrazok po prefiltrovani
    cv::Mat imgOrientationMap; // obrazok smerovej mapy
    cv::Mat imgBinarized;
    cv::Mat imgSkeleton;
    cv::Mat imgSkeletonInverted;
    cv::Mat imgMask;
    cv::Mat imgFrequencyMap;
    cv::Mat imgQualityMap;
    cv::Mat frequencyMap;
    cv::Mat orientationMap;
    cv::Mat qualityMap;
} PREPROCESSING_ALL_RESULTS;



#ifndef PREPROCESSING_RESULTS_DEFINED
typedef struct preprocessing_results {
    cv::Mat imgOriginal;
    cv::Mat imgSkeleton;
    cv::Mat imgSkeletonInverted;
    cv::Mat qualityMap;
    cv::Mat orientationMap;
} PREPROCESSING_RESULTS;
#define PREPROCESSING_RESULTS_DEFINED
#endif

//duration in ms
typedef struct preprocessing_durations {
    int contrastEnhancement;
    float orientationMap;
    float gaborFilter;
    int binarization;
    int removingHoles;
    int thinning;
    int mask;
    int qualityMap;
    int frequencyMap;
} PREPROCESSING_DURATIONS;

#ifndef BATCH_RESULTS_DEFINED
typedef struct batchPreprocessingResults{
    QVector<cv::Mat> original;
    QVector<cv::Mat> enhanced;
    QVector<cv::Mat> mask;
    QVector<cv::Mat> oMap;
    QVector<cv::Mat> Gabor;
    QVector<cv::Mat> binary;
    QVector<cv::Mat> skeleton;
} BATCH_RESULTS;
#define BATCH_RESULTS_DEFINED
#endif

class PREPROCESSINGSHARED_EXPORT Preprocessing : public QObject
{
    Q_OBJECT

public:
    Preprocessing();
    void start();

    int loadInput(cv::Mat imgOriginal);
    int loadInput(QVector<cv::Mat> imgOriginals);
    int loadInput(QString inputPath);

    int setPreprocessingParams(int blockSize = 13, double gaborLambda = 9, double gaborSigma = 3, int gaussBlockBasic = 1, double gaussSigmaBasic = 1.0, int gaussBlockAdvanced = 121, double gaussSigmaAdvanced = 10.0, int holeSize = 20);
    int setFeatures(bool useAdvancedMode, bool useContrastEnhancement = true, bool useAdvancedOrientationMap = true, bool useHoleRemover = true, bool generateInvertedSkeleton = true, bool useQualityMap = true, bool useMask = false, bool useFrequencyMap = false);
    int setFrequencyMapParams(CAFFE_FILES freqFiles, int blockSize, int exBlockSize);
    int setMaskParams(CAFFE_FILES maskFiles, int blockSize, int exBlockSize, bool useSmooth);
    int setCPUOnly(bool enabled, int threadNum = 0);
    void setBatchModeON(bool value);


private:

    ContrastEnhancement contrast;
    OrientationMap oMap;
    QualityMap qMap;
    GaborFilterMultiThread gaborMultiThread; // objekt na paralelne filtrovanie odtlacku
    GaborFilterGPU gaborGPU;
    Binarization binarization;
    Thinning thinning;
    ThinningMultithread thinningMultiThread;
    Mask mask;
    FrequencyMap fMap;

    //batch mode classes:
    contrastBatch contrast_batch;
    maskBatch mask_batch;
    binarizationBatch binary_batch;
    //----------------------------------------------------------------------------------
    bool BatchMode;//defines if batch mode is on(true), or off (false)



    QTime timer;

    bool preprocessingIsRunning;

    // INPUT
    INPUT_PARAMS inputParams;

    // PARAMS
    OMAP_PARAMS omapParams;
    QMAP_PARAMS qmapParams;
    FMAP_PARAMS fmapParams;
    MASK_PARAMS maskParams;
    CONTRAST_PARAMS contrastParams;
    GABOR_PARAMS gaborParams;
    BINARIZATION_PARAMS binarizationParams;

    PREPROCESSING_FEATURES features;
    PREPROCESSING_GENERAL general;

    // OUTPUT
    af::array orientationMapAF;
    PREPROCESSING_ALL_RESULTS results;
    QMap<QString, PREPROCESSING_RESULTS> resultsMap;
    QMap<QString, PREPROCESSING_ALL_RESULTS> allResultsMap;

    BATCH_RESULTS batchAllResults;

    PREPROCESSING_DURATIONS durations;

    // PRIVATE FUNCTIONS
    void continueAfterGabor();
    void preprocessingError(int errorcode);
    void cleanResults();
    void cleanInput();
    void cleanDurations();
    void startProcess(const cv::Mat &imgOriginal);
    //batch mode
    void startBatchProcess(QVector<cv::Mat> imgOriginal);
    af::array createBatch(QVector<cv::Mat> MatImages,bool isFloat); // create af::array Batch fort Batch mode
    QVector<cv::Mat> decomposeBatch(af::array batch,bool isFloat); // turn af::array Batch to QVector for storing

private slots:
    void allGaborThreadsFinished();
    void allThinningThreadsFinished();

signals:
    void preprocessingDoneSignal(PREPROCESSING_ALL_RESULTS results);
    void preprocessingDoneSignal(PREPROCESSING_RESULTS results);

    void preprocessingSequenceDoneSignal(QMap<QString, PREPROCESSING_ALL_RESULTS> results);
    void preprocessingSequenceDoneSignal(QMap<QString, PREPROCESSING_RESULTS> results);

    void preprocessingDurationSignal(PREPROCESSING_DURATIONS durations);

    void preprocessingBatchDoneSignal(BATCH_RESULTS batchAllResults);

    void preprocessingProgressSignal(int progress);
    void preprocessingErrorSignal(int errorcode);
};

#endif // PREPROCESSING_H
