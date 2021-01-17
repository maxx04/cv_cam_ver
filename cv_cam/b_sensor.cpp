#include "b_sensor.h"

b_sensor::b_sensor(Mat* parent_image, cv::Point2i p, int _size = 16) : sensor(parent_image, p, _size)
{

	// für erstes anlauf vorbereiten last_image
	(*parent_image)(cv::Rect(p.x, p.y, _size, _size)).copyTo(last_image);

	// berechne Grauwerte (konvertiere Bild)
	cvtColor(last_image, last_image, cv::COLOR_BGR2GRAY);
}

void b_sensor::proceed()
{
	// für nächste bearbeitung vorbereiten
	img.copyTo(last_image);

	Rect roi(position.x, position.y, size, size);

	// speichere aktuelle kopie des Bildes
	(*parent_image)(roi).copyTo(img);

	// erste Pixel Adresse abfragen
	PixelColor* pixelPtr = (PixelColor*)img.data;

	// berechne Grauwerte (konvertiere Bild)
	cvtColor(img, img, cv::COLOR_BGR2GRAY);

	set_kontrast(img);
	set_kontrast(last_image);

	//PixelColor Pixel0 = *(pixelPtr + index[POINTS_IN_CIRCLE - 1]); //letzte pixel


}

void b_sensor::draw_magnifyied()
{
	//HACK aktuell nur aktive sensoren werden bearbeitet
	proceed();

	imshow(sensor_magnifyed_window, img);
	imshow(sensor_magnifyed_window_last, last_image);

}
