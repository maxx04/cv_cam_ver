#include "myBlobDetector.h"

myBlobDetector::myBlobDetector(const myBlobDetector::Params _parameters)
{
    parameters = _parameters; 

    p.thresholdStep = 50;
    p.minThreshold = 10;
    p.maxThreshold = 250;
    p.filterByArea = false;
    p.filterByCircularity = false;
    p.filterByColor = false;
    p.filterByConvexity = false;
    p.filterByInertia = false;

    sbd = cv::SimpleBlobDetector::create(p);

}

/// <summary>
/// Findet curculare Blobs
/// </summary>
/// <param name="image">eingang: graue image</param>
/// <param name="keypoints">ausgabe: keypoints (mitte vom Blob)</param>
void myBlobDetector::detect(cv::Mat image, std::vector<cv::KeyPoint>& keypoints)
{
    CV_Assert(image.type() == CV_8UC1);

    //blobs.LabelImage(image);
    //blobs.FilterByArea(10, 500);

    //cv::KeyPoint k;

    //for (auto& a_blob : blobs.get_BlobsList()) {

    //    k.pt = a_blob->get_Centroid();

    //    keypoints.push_back(k);
    //}

    sbd->detect(image, keypoints);

    //keypoints.push_back(cv::KeyPoint({ 10.0, 10.0 }, 3.0));

    // finde steigung
    // cirkular finde grenze

}

myBlobDetector::Params::Params()
{
    thresholdStep = 2;
    minThreshold = 10;
    maxThreshold = 100;
}
