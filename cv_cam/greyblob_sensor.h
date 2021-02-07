#pragma once
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include "sensor.h"
#include "myBlobDetector.h"

/// <summary>       
/// Sensor Brutal force. Ermittelt sensor verschiebung in x und y
/// </summary>
class greyblob_sensor : public sensor
{

    static myBlobDetector* blob_detector;

    static Ptr<FeatureDetector> orb_detector;

protected:
          
    /// <summary>
    /// letzte bearbeitete image
    /// </summary>
    Mat last_image;

    cv::Point2i shift;

    // Storage for blobs
    vector<cv::KeyPoint> keypoints;

    void set_bw(Mat* gray);

    // vorbereite aktuelles bild zur verarbeitung
    void prepare_frame(cv::Mat* frame);

public:

    PixelData min_val;
    PixelData max_val;

    /// <summary>
    /// erstellt sensor
    /// </summary>
    /// <param name="parent_image">eingangsbild</param>
    /// <param name="p">position des sensors im bild von links / oben</param>
    /// <param name="_size">sensorgroesse in pixeln</param>         
    greyblob_sensor(Mat* parent_image, Point2i p, int _size); // init

    /// <summary>
    /// analysieren pixeln und ermitteln bewegung
    /// aktuelles image in sensor drin  
    /// </summary>
    void proceed();
    // vergroesserte sensor abbilden
    virtual void draw_magnifyied();
    float calculate_mean(cv::Mat image);
    float sub(cv::Mat image1, cv::Mat image2, cv::Mat* out_image);
    cv::Point2i find_shift();
};

