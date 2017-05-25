#pragma once
#include "block.h"
#include "m_sensor.h"



class pyramide
{

	 block<block <m_sensor>> b1; //pyramide 2 mal tief

public:
	pyramide();
	pyramide(Point p);
	~pyramide();

	void set_position(Point p);
};

