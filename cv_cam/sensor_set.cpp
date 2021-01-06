#include "sensor_set.h"
#include <opencv2/imgproc.hpp>
#include <random>
#include <iostream>

using namespace std;

sensor_set::sensor_set(Mat frame0, int ns)
{
 	int sensor_size = s_sensor(&frame0, Point(0,0), 32).size;  // HACK nicht aendern, sonst fehler

	default_random_engine generator;

	uniform_int_distribution<int> distribution_x(0, frame0.cols - sensor_size);
	uniform_int_distribution<int> distribution_y(0, frame0.rows - sensor_size);

	// sensoren erstellen und verteilen
	for (size_t i = 0; i < ns; i++)
	{
		Point sensor_position(distribution_x(generator), distribution_y(generator));
		sensors.push_back((sensor*) new s_sensor(&frame0, sensor_position, 32));	// HACK notwendige sensor erstellen
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

}


sensor_set::~sensor_set()
{

}

void sensor_set::select_sensor(int sensor_nr)
{
	if (sensor_nr >= 0 && sensor_nr < sensors.size())
	{
		selected_sensor = sensor_nr;
	}
}


void sensor_set::set_image(Mat* input_image)
{
	sensors[0] -> set_image(input_image);
}

void sensor_set::draw(Mat * input_image )
{
	set_image( input_image );

	// vergroesserte fenster mit "key points" anzeigen
	sensors[selected_sensor] -> draw_magnifyied();

	// auf dem gesamtbild
	sensors[selected_sensor] -> draw();

}

void sensor_set::proceed(Mat * region_of_interest, int pegel)
{
	set_image( region_of_interest );

	for (sensor* m : sensors) m -> proceed();
}

void sensor_set::proceed(int n)
{
	//set_image(region_of_interest);

	sensors[n]->proceed();
}


int sensor_set::find_nearest_sensor(int x, int y)
{
	int ret = 0;
	int d = 0;
	int dist_min = 100000;

	for (int m = 0; m < sensors.size(); m++)
	{
		d = sensors[m]-> get_distance_to_middle(x, y);
		if (d < dist_min)
		{
			selected_sensor = m;
			dist_min = d;
			ret = m;
		}
	}

	return ret;
}

PixelColor sensor_set::get_color(int x, int y, const Mat * input)
{
	return sensors[selected_sensor]->get_color(x, y, input);
}

sensor* sensor_set::get_sensor(int n)
{
	return sensors[n];
}

//contours sensor_set::find_contours(void)
//{
//	contours cnt;
//
//
//
//
//	return cnt;
//}
