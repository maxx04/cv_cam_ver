#pragma once
#include "sensor.h"
class b_sensor :
    public sensor


{
    public:
        b_sensor(Mat* parent_image, cv::Point2i p, int _size); // init

        void proceed(); // analysieren pixeln und erzeugen segmente und linien


protected:
    Mat last_image;
};

