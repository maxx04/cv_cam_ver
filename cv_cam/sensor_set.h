#pragma once

#include <opencv2/imgproc.hpp>
#include <random>
#include <iostream>

#include "s_sensor.h"
#include "m_sensor.h"
#include "b_sensor.h"
#include "contours.h"

class key_points_set;

/*
Klass zu Verwaltung den Sensoren
*/

template <class T>
class sensor_set 
{
	vector<sensor*> sensors;  // Vektor aus Sensors

public:
	int selected_sensor = 0;   // actives Sensor zum Anzeigen
	int number_sensors = 0;	// Anzahl erstellten Sensoren

	// Erstellt Sensoren nach zuf�llige Position
	sensor_set <T> (Mat input_frame, int sensors_number, int _size)
	{
		int sensor_size = T(&frame0, Point(0, 0), _size).size;  // HACK nicht aendern, sonst fehler

		default_random_engine generator;

		uniform_int_distribution<int> distribution_x(0, frame0.cols - sensor_size);
		uniform_int_distribution<int> distribution_y(0, frame0.rows - sensor_size);

		// sensoren erstellen und verteilen
		// auch sensortyp soll definiert sein, �ber Tamplate zu loesen
		for (size_t i = 0; i < sensors_number; i++)
		{
			Point sensor_position(distribution_x(generator), distribution_y(generator));
			sensors.push_back((sensor*) new T(&frame0, sensor_position, sensor_size));
		}

		number_sensors = sensors.size();

		for (int m = 0; m < number_sensors; m++)
		{


			//for (int mn = 0; mn < sensors_number; mn++)
			//{

			//	if (sensors[mn].intersection(mi) && mn != m)
			//	{
			//		mi->nighbors.push_back(mn); // HACK Achtung bei vectoraenderung numerrierung wird geaendert

			//		 //cout << m << " - " << mn.get_position() << " / "; 
			//	}
			//}


			//cout << mi.nighbors.size() << endl;
		}

	};

	// Loest Klass
	~sensor_set <T>() {};

	// setzt aktives (untersuchte)  Sensor an
	void select_sensor (int sensor_nr)
	{
		if (sensor_nr >= 0 && sensor_nr < sensors.size())
		{
			selected_sensor = sensor_nr;
		}
	};

	// setzt aktuelles bild	und kopiert ROI
	void set_image (Mat* input_image)
	{
		sensors[0]->set_image(input_image);

	};

	void draw(Mat* input_image) // zeichnet Sensor auf vergr�ssertem Bild
	{

		// vergroesserte fenster mit "key points" anzeigen
		sensors[selected_sensor]->draw_magnifyied();

		// auf dem gesamtbild
		sensors[selected_sensor]->draw(input_image);

	};

	// auswertet alle Sensore mit Bild input_image und pegel f�r die Schl�sselpunkte
 	void proceed( Mat* input_image, int pegel )
	{
		set_image(input_image);


//////////////////////////////////////////////////////////
	// HACK aktuell ausgeblendet
	//	for (sensor* m : sensors) m->proceed();

		sensors[selected_sensor]->proceed();
	}	

	void proceed(int n) 
	{
		//set_image(region_of_interest);

		sensors[n]->proceed();
	};   // sensor n 

	// f�llt Schl�sselpunkte in key_points aus bild input // TODO Bild ist nicht notwendig
	//void add_keypoints(key_points_set* key_points, Mat* input);

	// findet n�chste Sensor
	int find_nearest_sensor(int x, int y) 
	{
		int ret = 0;
		int d = 0;
		int dist_min = 100000;

		for (int m = 0; m < sensors.size(); m++)
		{
			d = sensors[m]->get_distance_to_middle(x, y);
			if (d < dist_min)
			{
				selected_sensor = m;
				dist_min = d;
				ret = m;
			}
		}

		return ret;
	}

	// gibt Farbe vom Pixel x,y aus dem Bild input 
	PixelColor get_color(int x, int y)
	{
		return sensors[selected_sensor]->get_color(x, y);
	};

	sensor* get_sensor(int n)
	{
		return sensors[n];
	};

};

