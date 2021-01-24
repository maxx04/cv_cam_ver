#include "sensor.h"
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

const String sensor::sensor_magnifyed_window = "magnify sensor";
const String sensor::sensor_magnifyed_window_last = "magnify sensor last";
const String sensor::sensor_result_window = "result sensor";
Mat* sensor::parent_image; 
uint sensor::sensors_number = 0;
int sensor::size = 16;
Mat sensor::out;

sensor::sensor(Mat * _parent_image, cv::Point2i p, int _size)
{

	assert(_size >= 12 && _size <= 96); // test auf Groesse
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
	size_t a = img.elemSize();
	return img.at<PixelColor>(y, x);
}


void sensor::draw(Mat* output_image)
{
	rectangle(*output_image, Rect(position.x, position.y, size, size), Scalar(125, 0, 0), 3);

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

	// berechne Grauwerte (konvertiere Bild)
	cvtColor(img, out, cv::COLOR_BGR2GRAY);

	//cvtColor(out, out, COLOR_HSV2BGR);
	imshow(sensor_magnifyed_window, out);

}

/// <summary>
/// Berechnet maximale und minimale Grauwert. Dann aendert (erhoeht) kontrast.
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
	
	float k = ((float)(max_val.value - min_val.value)) / 254.0;

	for (int i = 0; i < out.cols; i++)
		for (int j = 0; j < out.rows; j++)
		{
			uint8_t l = out.at<uint8_t>(i, j);
			int m = (l - min_val.value);
			float x = (k != 0.0) ? (float)m / k : min_val.value;
			out.at<uint8_t>(i, j) = (uint8_t)x;
		}

	out.at<uint8_t>(min_val.x, min_val.y) = (uint8_t)255;
	out.at<uint8_t>(max_val.x, max_val.y) = (uint8_t)0;
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
