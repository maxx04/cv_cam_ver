#include "myBlobDetector.h"

myBlobDetector::myBlobDetector(const myBlobDetector::Params _parameters)
{
    parameters = _parameters;
}

/// <summary>
/// Findet curculare Blobs
/// </summary>
/// <param name="image">eingang: graue image</param>
/// <param name="keypoints">ausgabe: keypoints (mitte vom Blob)</param>
void myBlobDetector::detect(cv::InputArray image, std::vector<cv::KeyPoint>& keypoints)
{
    CV_Assert(image.channels() == 1);

    keypoints.push_back(cv::KeyPoint({ 10.0, 10.0 }, 3.0));

    // finde steigung
    // cirkular finde grenze

}

myBlobDetector::Params::Params()
{
    thresholdStep = 2;
    minThreshold = 10;
    maxThreshold = 100;
}
