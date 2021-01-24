#include "b_sensor.h"

b_sensor::b_sensor(Mat* parent_image, cv::Point2i p, int _size = 16) : sensor(parent_image, p, _size)
{

	// berechne Grauwerte (konvertiere Bild)
	cvtColor(img, last_image, cv::COLOR_BGR2GRAY);

	// berechne Grauwerte (konvertiere Bild)
	cvtColor(last_image, last_image, cv::COLOR_BGR2GRAY);
}

void b_sensor::proceed()
{
	// für nächste bearbeitung vorbereiten
 	cvtColor(img, last_image, cv::COLOR_BGR2GRAY);

	Rect roi(position.x, position.y, size, size);

	// speichere aktuelle kopie des Bildes
	(*parent_image)(roi).copyTo(img);

	Mat gray;
	// berechne Grauwerte (konvertiere Bild)
	 cvtColor(img, gray, cv::COLOR_BGR2GRAY);

	set_kontrast(gray);
	set_kontrast(last_image);

}

void b_sensor::draw_magnifyied()
{

	imshow(sensor_magnifyed_window, img);
	imshow(sensor_magnifyed_window_last, last_image);

}
