#include "sensor.h"
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

const String sensor::sensor_magnifyed_window = "magnify sensor";
const String sensor::sensor_magnifyed_window_last = "magnify sensor last";
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


void sensor::draw(Mat* output_image)
{
	rectangle(*output_image, Rect(position.x, position.y, size, size), Scalar(125, 0, 0), 1);

	for each (Point pnt in key_points)
	{
		drawMarker(*parent_image, position + pnt, Scalar(0, 0, 255), MarkerTypes::MARKER_CROSS, 1);
		//circle((*output_image), pos + pnt, 0, Scalar( 0, 0, 255));
	}
}

void sensor::draw_magnifyied()
{
	Mat out;  // ausgabe image

	//CV_Assert((*input).type() == out.type());

	/* Blobsuche */


	// berechne Grauwerte (konvertiere Bild)
	cvtColor(img, out, cv::COLOR_BGR2GRAY);

	set_kontrast(out);

	// glaette ??

	// finde Blobs

	//cvtColor(out, out, COLOR_HSV2BGR);
	imshow(sensor_magnifyed_window, out);

}

/// <summary>
/// Berechnet maximale und minimale Grauwert. Dann aendert (erhoeht) kontrast.
/// </summary>
/// <param name="out">Mat GRAY</param>
void sensor::set_kontrast(cv::Mat& out)
{
	// finde max wert
	uint8_t max_val = 0;
	for (int i = 0; i < out.cols; i++)
		for (int j = 0; j < out.rows; j++)
		{
			max_val = max(max_val, out.at<uint8_t>(i, j));
		}

	// finde min wert
	uint8_t min_val = 255;
	for (int i = 0; i < out.cols; i++)
		for (int j = 0; j < out.rows; j++)
		{
			min_val = min(min_val, out.at<uint8_t>(i, j));
		}
	
	float k = ((float)(max_val - min_val)) / 254.0;

	for (int i = 0; i < out.cols; i++)
		for (int j = 0; j < out.rows; j++)
		{
			uint8_t l = out.at<uint8_t>(i, j);
			int m = (l - min_val);
			float x = (k != 0.0) ? (float)m / k : min_val;
			out.at<uint8_t>(i, j) = (uint8_t)x;
		}
}
