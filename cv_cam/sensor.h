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
	static Mat out; // ausgabebild fuer sensor
	static Mat prepared_frame; // gesamtframe die war vorbereited fuer aktuelle bearbeitung

	cv::Point2i position; // sensor position (obere-linke ecke)
	cv::Mat img; // bildbereich fuer sensor
	vector<cv::Point2i> key_points; // keypoints f�r den Sensor	in Koordinaten
	vector<segment> line_segments; // erzeugte linien 

public:
	const static String sensor_magnifyed_window; // CV Fenstername f�r aktive Sensor
	const static String sensor_magnifyed_window_last; // CV Fenstername f�r aktive Sensor letztes bild
	const static String sensor_result_window;	 // CV Fenstername f�r ergebniss aktiven Sensor
	static int size; // Groesse (H�he) des Sensors ist gleich Breite

	sensor(Mat* parent_image, cv::Point2i p, int _size); // init
	~sensor();

	// analysieren pixeln und erzeugen segmente und linien
	virtual void proceed() = 0;

	// vergroesserte sensor abbilden
	virtual void draw_magnifyied();

	// markiert sensor auf dem bild **output image**
	virtual void draw(Mat* output_image);

	void draw_shifts(Mat* output_image);

	// setzt aktuelles bild	f�r die Bearbeitung
	void set_image(Mat* input_image);

	// gibt keypoints in gemeinsame vektor ab
	void get_keypoints(vector<Point2i>* global_points);	 

	// gibt abstand in pixeln vom bildposition zu sensormitte
	int get_distance_to_middle(int x, int y);  

	// gibt farbe vom pixel in sensorkoordinaten
	PixelColor get_color(int x, int y); 

	// gibt sensorposition
	inline cv::Point2i get_position() { return position; } 

	/// <summary>
	/// Normalise image
	/// gleiche ist cv::normalise
	/// </summary>
	/// <param name="out">verweis auf das Bild der soll normalisiert werden</param>
	virtual void set_kontrast(cv::Mat& out);
	virtual PixelData max_pixel(cv::Mat& out);
	virtual PixelData min_pixel(cv::Mat& out);

	// vorbereite eingangsbild
	virtual void prepare_frame(cv::Mat* frame);
};

