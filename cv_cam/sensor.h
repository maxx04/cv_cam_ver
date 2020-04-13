#pragma once
#include <vector>
#include <opencv2/core.hpp>
#include "core.h"
#include "segment.h"

using namespace cv;
using namespace std;


// allgemeine Klasse zu Sensoren
class sensor
{
protected:
	static uint sensors_number;	
	static Mat* parent_image;

	cv::Point2i position; // sensor position (obere-linke ecke)
	cv::Mat data; // bildbereich in BGR format ??
	vector<cv::Point2i> key_points; // keypoints für den Sensor
	vector<segment> line_segments; // erzeugte linien 

public:
	const static String sensor_magnifyed_window; // CV Fenstername für aktive Sensor
	const static String sensor_result_window;	 // CV Fenstername für ergebniss aktiven Sensor
	const static int size; // Groesse (Höhe) des Sensors ist gleich Breite

	sensor(Mat* parent_image, cv::Point2i p); // init
	~sensor();

	virtual void proceed(const cv::Mat* region_of_interest) = 0; // analysieren pixeln und erzeugen segmente und linien

	virtual void draw_magnifyied() = 0; // vergroesserte sensor abbilden 

	virtual void draw() = 0;	// markiert sensor auf dem bild **output image**

	// setzt aktuelles bild
	void set_image(Mat* input_image);

	void get_keypoints(vector<Point2i>* global_points);	// gibt keypoints in gemeinsame vektor ab 

	int get_distance_to_middle(int x, int y);  // gibt abstand in pixeln vom bildposition zu sensormitte

	PixelColor get_color(int x, int y, const Mat* input); // gibt farbe vom pixel in sensorkoordinaten


	inline cv::Point2i get_position() { return position; } // gibt sensorposition
};

