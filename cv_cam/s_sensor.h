#pragma once
#include "sensor.h"

// sensor basierend auf segmentieren
class s_sensor : public sensor
{
public:

	s_sensor( Mat* parent_image, cv::Point2i p, int _size); // init

	void proceed(); // analysieren pixeln und erzeugen segmente und linien

	// vergroesserte sensor abbilden
	void draw_magnifyied();

};

