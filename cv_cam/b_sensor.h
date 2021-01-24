#pragma once
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include "sensor.h"
#include "myBlobDetector.h"

/// <summary>       
/// Sensor Brutal force. Ermittelt sensor verschiebung in x und y
/// </summary>
class b_sensor : public sensor
{
public: 

        /// <summary>
        /// erstellt sensor
        /// </summary>
        /// <param name="parent_image">eingangsbild</param>
        /// <param name="p">position des sensors im bild von links / oben</param>
        /// <param name="_size">sensorgroesse in pixeln</param>         
        b_sensor(Mat* parent_image, Point2i p, int _size); // init

        /// <summary>
        /// analysieren pixeln und ermitteln bewegung
        /// aktuelles image in sensor drin  
        /// </summary>
        void proceed(); 
        // vergroesserte sensor abbilden
        virtual void draw_magnifyied();


protected:
          
    /// <summary>
    /// letzte bearbeitete image
    /// </summary>
    Mat last_image;
    void set_bw(Mat* gray);

    myBlobDetector* blob_detector;
};

