#include "sensor.h"
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

const String sensor::sensor_magnifyed_window = "magnify sensor";
const String sensor::sensor_magnifyed_window_last = "magnify sensor last";
const String sensor::sensor_result_window = "result sensor";
Mat* sensor::parent_image; 
Mat sensor::prepared_frame;
uint sensor::sensors_number = 0;
int sensor::size = 16;
Mat sensor::out;

sensor::sensor(Mat * _parent_image, cv::Point2i p, int _size)
{

	if (sensors_number < 1) //nur bei erstem sensor alles berechnen
	{
		assert(_size >= 12 && _size <= 96); // test auf Groesse
		assert(_size % 2 == 0); // test auf gerade Zahl

		parent_image = _parent_image;
		size = _size;
		prepare_frame(parent_image);
	}

	position = p;

	key_points = vector<Point>(0);

	sensors_number++;

	Rect roi(position.x, position.y, size, size);

	prepared_frame(roi).copyTo(img);

	//int bytes = sizeof(PixelColor);

}

sensor::~sensor()
{
	sensors_number = (sensors_number > 0) ? sensors_number-1 : 0;
}

void sensor::set_image(Mat* input_image)
{
	parent_image = input_image;	  //TODO Leistung

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

PixelColor sensor::get_color(int x, int y)
{
	// nur Graubild kann verwendet werden
	CV_Assert(img.elemSize() == 1);

	size_t a = img.elemSize();

	if (a == 1)
	{	
		uint8_t p = img.at<uint8_t>(y, x);
		return 	{ p, p, p };
	}
	return img.at<PixelColor>(y, x);
}


void sensor::draw(Mat* output_image)
{
	rectangle(*output_image, Rect(position.x, position.y, size, size), Scalar(125, 0, 0), 3);

	Point last_p, start_p(position + Point(size / 2, size / 2));

	last_p = start_p;
	
	for each (Point pnt in key_points)
	{
		start_p += pnt;

		line(*output_image, last_p, start_p, Scalar(0, 0, 255), 2);
		//drawMarker(*output_image, start_p, Scalar(0, 0, 255), MarkerTypes::MARKER_CROSS, 10);

		last_p = start_p;
	}
}

void sensor::draw_shifts(Mat* output_image)
{
	if (key_points.size() < 7) return;

	Point last_p, start_p(position + Point(size / 2, size / 2));

	last_p = start_p;

	Point pnt;

	for (int i = key_points.size()-6; i < key_points.size(); i++)
	{

		start_p += key_points[i];

		line(*output_image, last_p, start_p, Scalar(0, 0, 255), 2);
		//drawMarker(*output_image, start_p, Scalar(0, 0, 255), MarkerTypes::MARKER_CROSS, 10);

		last_p = start_p;
	}

	circle(*output_image, last_p, 6, Scalar(0, 0, 255), 2);

}

void sensor::draw_magnifyied()
{
	//CV_Assert((*input).type() == out.type());

	//cvtColor(out, out, COLOR_HSV2BGR);
	imshow(sensor_magnifyed_window, out);

}

/// <summary>
/// Berechnet maximale und minimale Grauwert. Dann aendert (erhoeht) kontrast.
/// analog opencv normalise
/// </summary>
/// <param name="out">Mat GRAY</param>
void sensor::set_kontrast(cv::Mat& out)
{
	// nur Graubild kann verwendet werden
	CV_Assert(out.elemSize() == 1);

	// finde max wert
	PixelData max_val = max_pixel(out);

	// finde min wert
	PixelData min_val = min_pixel(out);
	
	float k = ((float)(max_val.value - min_val.value)) / 255.0;

	for (int i = 0; i < out.cols; i++)
		for (int j = 0; j < out.rows; j++)
		{
			uint8_t l = out.at<uint8_t>(i, j);
			int m = (l - min_val.value);
			float x = (k != 0.0) ? (float)m / k : min_val.value;
			out.at<uint8_t>(i, j) = (uint8_t)x;
		}

}

PixelData sensor::max_pixel(cv::Mat& out)
{
	// nur Graubild kann verwendet werden
	CV_Assert(out.elemSize() == 1);

	// finde max wert
	uint8_t i, j, i_max=0, j_max=0;
	uint8_t max_val = 0, v;

	for (i = 0; i < out.cols; i++)
		for (j = 0; j < out.rows; j++)
		{
			v = out.at<uint8_t>(i, j);
			if (max_val < v)
			{
				max_val = v;
				i_max = i;
				j_max = j;
			}
		}



	return { i_max, j_max, max_val };

}

PixelData sensor::min_pixel(cv::Mat& out)
{
	// nur Graubild kann verwendet werden
	CV_Assert(out.elemSize() == 1);

	// finde max wert
	uint8_t i, j, i_min=255, j_min=255;
	uint8_t min_val = 255, v; // HACK

	for (i = 0; i < out.cols; i++)
		for (j = 0; j < out.rows; j++)
		{
			v = out.at<uint8_t>(i, j);
			if (min_val > v)
			{
				min_val = v;
				i_min = i;
				j_min = j;
			}
			
		}



	return { i_min, j_min, min_val };

}


void sensor::prepare_frame(cv::Mat* frame)
{

	frame->copyTo(prepared_frame);
		
}
