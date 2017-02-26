#include "stdafx.h"
#include "sensor_set.h"

extern class key_points_set;

sensor_set::sensor_set(Mat frame0, int ns)
{

	const int sensor_size = SENSOR_RADIUS*2+2;
	number_sensors = ns;

	default_random_engine generator;
	uniform_int_distribution<int> distribution_x(0, frame0.cols - sensor_size);
	uniform_int_distribution<int> distribution_y(0, frame0.rows - sensor_size);

	// sensoren erstellen
	for (size_t i = 0; i < number_sensors; i++)
	{
		Point sensor_position(distribution_x(generator), distribution_y(generator));
		sensors.push_back(m_sensor(sensor_position, sensor_size));
	}

	// nachbarn ermitteln

	int sensors_number = (int)sensors.size();

	for (int m = 0; m < sensors_number; m++)
	{
		m_sensor* mi = &sensors[m];

		cout << m << "\r";

		for (int mn = 0; mn < sensors_number; mn++)
		{

			if (sensors[mn].cross(mi) && mn != m)
			{
				mi->nighbors.push_back(mn); // HACK Achtung bei vectoraenderung numerrierung wird geaendert

				 //cout << m << " - " << mn.get_position() << " / "; 
			}
		}

	
		//cout << mi.nighbors.size() << endl;
	}

}

sensor_set::sensor_set()
{

}

sensor_set::~sensor_set()
{

}

void sensor_set::select_sensor(int sensor_nr)
{
	if (sensor_nr >= 0 && sensor_nr < number_sensors)
	{
		selected_sensor = sensor_nr;
	}
}

void sensor_set::draw_selected_sensor(Mat * output_image)
{
	m_sensor m = sensors[selected_sensor];

	Rect Sensor_roi(m.get_position(), m.get_size());

	/*
	// Nachbarn zeichnen
	for each (ushort sensor_number in m.nighbors)
	{
		Rect n_roi(sensors[sensor_number].get_position(), sensors[sensor_number].get_size());
		cv::rectangle(*output_image, n_roi, Scalar(0, 250, 120));
	}
	*/

	// sensor selber zeichnen
	rectangle(*output_image, Sensor_roi, Scalar(0, 0, 250));

}

void sensor_set::magnify_selected_sensor(const Mat * input_image, const String magnifyed_view_window)
{
	m_sensor m = sensors[selected_sensor];

	// vergroesserte fenster mit "key points" anzeigen
	m.show(input_image, magnifyed_view_window);
}

void sensor_set::check_sensors(const Mat * frame, int pegel)
{
	for (int m = 0; m < sensors.size(); m++) 
		sensors[m].check(frame, pegel);
}

void sensor_set::add_keypoints(key_points_set* key_points)
{
	vector<Point> temp(20);

	for each  (m_sensor m in sensors)
	{
		temp.clear();
		m.add_points(&temp);

		for each (Point p in temp)
		{
			key_points-> add_point(key_point_gradient(p));
		}
	}
}

void sensor_set::show_keypoints(Mat* output_frame)
{
	Pixel color = { 0,0,255 };

	for each  (m_sensor m in sensors)
	{
		Point p1 = m.get_position();

		for each(Point pnt in m.key_points)
		{
			  //cv::drawMarker(*output_frame, m.get_position() + pnt, Scalar(0, 0, 255), MARKER_CROSS,1);
			  //(*output_frame).at<Pixel>(p1.y + pnt.y, p1.x + pnt.x) = color;
			  (*output_frame).at<Pixel>(p1 + pnt) = color;
		}
	}
}

void sensor_set::show_line_segments(Mat* output_frame)
{

	Scalar c(255,80,255);

	for each  (m_sensor m in sensors)
	{
		Point p1 = m.get_position();

		for each(segment segm in m.line_segments)
		{
			line((*output_frame), p1 + segm.P1, p1 + segm.P2, c);
		}
	}
}

ushort sensor_set::find_sensor(int x, int y)
{
	ushort ret = 0;
	int dist_min = 100000;
	Point mou(x, y);
	for (int m = 0; m < number_sensors ; m++)
		if (sensors[m].cross(mou))
		{
			ushort d = sensors[m].get_distance_to_middle(x, y);
			if (dist_min > d )
			{
				selected_sensor = m;
				dist_min = d;
				ret = m;
			}
		}			

	return ret;
}

Pixel sensor_set::get_color(int x, int y, const Mat * input)
{
	m_sensor m = sensors[selected_sensor];

	return m.get_color(x, y, input);
}
