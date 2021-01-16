#include "s_sensor.h"
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/features2d.hpp>
#include <iostream>

s_sensor::s_sensor(Mat* parent_image, cv::Point2i p, int _size = 16) : sensor(parent_image, p, _size)
{

}

void s_sensor::proceed()
{
	Rect roi(position.x, position.y, size, size);

	// speichere aktuelle kopie des Bildes
	(*parent_image)(roi).copyTo(img);

	// erste Pixel Adresse abfragen
	PixelColor* pixelPtr = (PixelColor*)img.data;

	//PixelColor Pixel0 = *(pixelPtr + index[POINTS_IN_CIRCLE - 1]); //letzte pixel
}

void s_sensor::draw_magnifyied()
{
	Mat out;  // ausgabe image

	//CV_Assert((*input).type() == out.type());

	/* Blobsuche */

	// berechne Grauwerte (konvertiere Bild)
	cvtColor(img, out, cv::COLOR_BGR2GRAY);

//	set_kontrast(out);

	// glaette ??

	// finde Blobs

	//cvtColor(out, out, COLOR_HSV2BGR);
	imshow(sensor_magnifyed_window, out);

}
