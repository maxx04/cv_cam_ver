#include "sensor.h"
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

const String sensor::sensor_magnifyed_window = "magnify sensor";
const String sensor::sensor_result_window = "result sensor";
Mat* sensor::parent_image; 
uint sensor::sensors_number = 0;
int sensor::size = 16;

sensor::sensor(Mat * _parent_image, cv::Point2i p, int _size)
{

	assert(_size > 12 && _size < 96); // test auf Groesse
	assert(_size % 2 == 0); // test auf gerade Zahl

	if (sensors_number < 2) //nur bei erstem sensor alles berechnen
	{
		parent_image = _parent_image;
		size = _size;
	}

	position = p;

	key_points = vector<Point>(0);

	sensors_number++;

	Rect roi(position.x, position.y, size, size);

	(*_parent_image)(roi).copyTo(img);

	//int bytes = sizeof(PixelColor);

}

sensor::~sensor()
{
	sensors_number = (sensors_number > 0) ? sensors_number-1 : 0;
}

void sensor::set_image(Mat* input_image)
{
	parent_image = input_image;
}

void sensor::get_keypoints(vector<Point>* global_points)
{
	for each (Point p in key_points)
	{
		global_points->push_back(Point2i(position + p));
	}
}

int sensor::get_distance_to_middle(int x, int y)
{
	int d1 = position.x + size / 2 - x;
	int d2 = position.y + size / 2 - y;
	return sqrt(d1 * d1 + d2 * d2);
}

PixelColor sensor::get_color(int x, int y, const Mat* input)
{
	PixelColor a = (*input).at<PixelColor>(position.y + y, position.x + x);

	//circle((*input), Point(pos.x + x, pos.y + y), 6, Scalar(a.x, a.y, a.z), -1);

	return a;
}

