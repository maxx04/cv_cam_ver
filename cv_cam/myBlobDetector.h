#pragma once
#include "opencv2/core.hpp"


class myBlobDetector
{
public:
    struct /*CV_EXPORTS_W_SIMPLE*/ Params
    {
        CV_WRAP Params();
        CV_PROP_RW float thresholdStep;
        CV_PROP_RW float minThreshold;
        CV_PROP_RW float maxThreshold;
        //CV_PROP_RW size_t minRepeatability;
        //CV_PROP_RW float minDistBetweenBlobs;

        //CV_PROP_RW bool filterByColor;
        //CV_PROP_RW uchar blobColor;

        //CV_PROP_RW bool filterByArea;
        //CV_PROP_RW float minArea, maxArea;

        //CV_PROP_RW bool filterByCircularity;
        //CV_PROP_RW float minCircularity, maxCircularity;

        //CV_PROP_RW bool filterByInertia;
        //CV_PROP_RW float minInertiaRatio, maxInertiaRatio;

        //CV_PROP_RW bool filterByConvexity;
        //CV_PROP_RW float minConvexity, maxConvexity;

        //void read(const FileNode& fn);
        //void write(FileStorage& fs) const;
    } parameters;

    //CV_WRAP static Ptr<myBlobDetector> create(const myBlobDetector::Params& parameters = myBlobDetector::Params());

    myBlobDetector(const myBlobDetector::Params parameters);
    void detect(cv::InputArray image, std::vector<cv::KeyPoint>& keypoints); // , cv::InputArray mask = cv::noArray());

    //CV_WRAP virtual String getDefaultName() const CV_OVERRIDE;
};

