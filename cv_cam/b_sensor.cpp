#include "b_sensor.h"

b_sensor::b_sensor(Mat* parent_image, cv::Point2i p, int _size = 16) : sensor(parent_image, p, _size)
{
	

}

void b_sensor::proceed()
{
	Rect roi(position.x, position.y, size, size);

	// speichere aktuelle kopie des Bildes
	(*parent_image)(roi).copyTo(img);



	// erste Pixel Adresse abfragen
	PixelColor* pixelPtr = (PixelColor*)img.data;

	//PixelColor Pixel0 = *(pixelPtr + index[POINTS_IN_CIRCLE - 1]); //letzte pixel
}
