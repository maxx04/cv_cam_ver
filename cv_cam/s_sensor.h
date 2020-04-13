#pragma once
#include "sensor.h"

// sensor basierend auf segmentieren
class s_sensor : public sensor
{
public:
	int test;

	s_sensor( Mat* parent_image, cv::Point2i p); // init

	void proceed(const cv::Mat * region_of_interest); // analysieren pixeln und erzeugen segmente und linien
	void draw(); // sensor auf dem gesamtbild markieren
	void draw_magnifyied();	// vergroesserung zeichnen
};

