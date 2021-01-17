#pragma once
#include "sensor.h"
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

/// <summary>       
/// Sensor Brutal force. Ermittelt sensor verschiebung in x und y
/// </summary>
class b_sensor :
    public sensor


{
public: 

    // vergroesserte sensor abbilden
    virtual void draw_magnifyied();

        /// <summary>
        /// erstellt sensor
        /// </summary>
        /// <param name="parent_image">eingangsbild</param>
        /// <param name="p">position des sensors im bild von links / oben</param>
        /// <param name="_size">sensorgroesse in pixeln</param>         
        b_sensor(Mat* parent_image, cv::Point2i p, int _size); // init

        /// <summary>
        /// analysieren pixeln und ermitteln bewegung
        /// aktuelles image in sensor drin  
        /// </summary>
        void proceed(); 


protected:
          
    /// <summary>
    /// letzte bearbeitete image
    /// </summary>
    Mat last_image;
};

