#pragma once
#include "s_sensor.h"
#include "contours.h"

class key_points_set;

/*
Klass zu Verwaltung den Sensoren
*/
class sensor_set 
{
	vector<sensor*> sensors;  // Vektor aus Sensors
	int selected_sensor = 0;   // actives Sensor zum Anzeigen

public:
	int number_sensors = 0;	// Anzahl erstellten Sensoren

	// Erstellt Sensoren nach zufällige Position
	sensor_set(Mat input_frame, int sensors_number);

	// Loest Klass
	~sensor_set();

	// setzt aktives (untersuchte)  Sensor an
	void select_sensor(int sensor_nr);

	// setzt aktuelles bild
	void set_image(Mat* input_image);

	void draw(Mat* input_image); // zeichnet Sensor auf vergrössertem Bild

	// auswertet alle Sensore mit Bild input_image und pegel für die Schlüsselpunkte
 	void proceed(const Mat* input_image, int pegel);

	// füllt Schlüsselpunkte in key_points aus bild input // TODO Bild ist nicht notwendig
	//void add_keypoints(key_points_set* key_points, Mat* input);

	// findet nächste Sensor
	int find_nearest_sensor(int x, int y);

	// gibt Farbe vom Pixel x,y aus dem Bild input 
	PixelColor get_color(int x, int y, const Mat* input);

	//contours find_contours(void);

};

