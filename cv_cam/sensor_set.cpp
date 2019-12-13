#include "key_points_set.h"
#include "key_point_gradient.h"
#include "sensor_set.h"
#include <random>

extern class key_points_set;

sensor_set::sensor_set(Mat frame0, int ns)
{

	const int sensor_size = 24;
	number_sensors = ns;

	default_random_engine generator;
	uniform_int_distribution<int> distribution_x(0, frame0.cols - sensor_size);
	uniform_int_distribution<int> distribution_y(0, frame0.rows - sensor_size);

	// sensoren erstellen und verteilen
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

void sensor_set::proceed(const Mat * block_1, int pegel)
{
	for (int m = 0; m < sensors.size(); m++) 
		sensors[m].proceed(block_1, pegel);
}

void sensor_set::add_keypoints(key_points_set* key_points, Mat* block_1)
{
	
	vector<Point> temp(20);

	key_points->keypoints_vector.clear();
	key_points->activ_frame = block_1; //HACK make privat

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
	PixelColor color = { 0,0,255 };

	for each  (m_sensor m in sensors)
	{  //TODO realisieren in m_sensor
		//Point p1 = m.get_position();

		//for each(Point pnt in m.key_points)
		//{
		//	  //cv::drawMarker(*output_frame, m.get_position() + pnt, Scalar(0, 0, 255), MARKER_CROSS,1);
		//	  //(*output_frame).at<PixelColor>(p1.y + pnt.y, p1.x + pnt.x) = color;
		//	  (*output_frame).at<PixelColor>(p1 + pnt) = color;
		//}
	}
}

void sensor_set::show_line_segments(Mat* output_frame)
{

	Scalar c(255,80,255);

	for each  (m_sensor m in sensors)
	{
		//TODO realisieren in m_sensor

		/*Point p1 = m.get_position();

		for each(segment segm in m.line_segments)
		{
			line((*output_frame), p1 + segm.P1, p1 + segm.P2, c);
		}*/
	}
}

void sensor_set::show_flats(Mat* output_frame)
{
	//PixelColor color = { 0,120,255 };
	Size sz = sensors[0].get_size()/2;

	for each  (m_sensor m in sensors)
	{
		//TODO realisieren in m_sensor

		//if (m.key_points.size() == 0)
		//{
		//	Point p1 = m.get_position()+Point(sz);

		//	//rectangle((*output_frame), Rect(p1.x, p1.y, 12, 12), Scalar(m.color.x, m.color.y, m.color.z));
		//	//circle((*output_frame), p1, 6, Scalar(m.color.x, m.color.y, m.color.z),-1);
		//}

	}
}

ushort sensor_set::find_sensor(int x, int y)
{
	ushort ret = 0;
	int dist_min = 100000;
	Point mou(x, y);
	for (int m = 0; m < number_sensors ; m++)
		if (sensors[m].intersection(mou))
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

PixelColor sensor_set::get_color(int x, int y, const Mat * input)
{
	m_sensor m = sensors[selected_sensor];

	return m.get_color(x, y, input);
}

contours sensor_set::find_contours(void)
{
	contours cnt;




	return cnt;
}
