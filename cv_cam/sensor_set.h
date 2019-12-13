#pragma once
#include "m_sensor.h"
#include "contours.h"

class key_points_set;

/*
Klass zu Verwaltung den Sensoren
*/
class sensor_set 
{
	vector<m_sensor> sensors;  // Vektor aus Sensors
	int selected_sensor = 0;   // actives Sensor zum Anzeigen



public:
	int number_sensors = 100;	// Anzahl erstellten Sensoren

	// Erstellt Sensoren nach zufällige Position
	sensor_set(Mat input_frame, int sensors_number);

	// leeres Set
	sensor_set();

	// Löst Klass
	~sensor_set();

	// setzt aktives (untersuchte)  Sensor an
	void select_sensor(int sensor_nr);

	// auswertet alle Sensore mit Bild input_image und pegel für die Schlüsselpunkte
 	void proceed(const Mat* input_image, int pegel);

	// füllt Schlüsselpunkte in key_points aus bild input // TODO Bild ist nicht notwendig
	void add_keypoints(key_points_set* key_points, Mat* input);

	// zeichnet Sensor auf Vergrössertem Bild
	void draw_selected_sensor(const Mat* input_image, const String magnifyed_view_window_name);

	// zeichnet Sensor auf dem Bild	output_frame
	void draw_selected_sensor(Mat* output_frame);

	// zeichnet Schlüsselpunkte auf dem Bild
	void draw_keypoints(Mat* output_frame);

	// zeichnet Liniensegmente auf dem Bild
	void draw_line_segments(Mat* output_frame);

	// zeichnet farbige Abbildung vom Sensor auf dem Bild
	void draw_flats(Mat* output_frame);

	// findet nächste Sensor
	int find_nearest_sensor(int x, int y);

	// gibt Farbe vom Pixel x,y aus dem Bild input 
	PixelColor get_color(int x, int y, const Mat* input);

	//contours find_contours(void);

};

