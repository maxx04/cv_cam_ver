#pragma once
#include "block.h"
#include "m_sensor.h"



class pyramide
{

	 block <m_sensor> b1;

public:
	pyramide();
	~pyramide();

	void set_position(Point p);
};

