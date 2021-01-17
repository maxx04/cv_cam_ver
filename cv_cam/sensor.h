#pragma once
#include <vector>
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/features2d.hpp>
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
	cv::Mat img; // bildbereich in BGR format ??
	vector<cv::Point2i> key_points; // keypoints für den Sensor
	vector<segment> line_segments; // erzeugte linien 

public:
	const static String sensor_magnifyed_window; // CV Fenstername für aktive Sensor
	const static String sensor_magnifyed_window_last; // CV Fenstername für aktive Sensor letztes bild
	const static String sensor_result_window;	 // CV Fenstername für ergebniss aktiven Sensor
	static int size; // Groesse (Höhe) des Sensors ist gleich Breite

	sensor(Mat* parent_image, cv::Point2i p, int _size); // init
	~sensor();

	// analysieren pixeln und erzeugen segmente und linien
	virtual void proceed() = 0;


	// vergroesserte sensor abbilden
	virtual void draw_magnifyied();

	// markiert sensor auf dem bild **output image**
	virtual void draw(Mat* output_image);

	// setzt aktuelles bild	für die Bearbeitung
	void set_image(Mat* input_image);

	// gibt keypoints in gemeinsame vektor ab
	void get_keypoints(vector<Point2i>* global_points);	 

	// gibt abstand in pixeln vom bildposition zu sensormitte
	int get_distance_to_middle(int x, int y);  

	// gibt farbe vom pixel in sensorkoordinaten
	PixelColor get_color(int x, int y, const Mat* input); 

	// gibt sensorposition
	inline cv::Point2i get_position() { return position; } 

	// erhoeht Kontrast
	virtual void set_kontrast(cv::Mat& out); 

};

