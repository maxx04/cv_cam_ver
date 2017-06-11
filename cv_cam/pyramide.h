#pragma once
#include "block.h"
#include "m_sensor.h"




class pyramide
{
	// set von den blocks
	 block <block <m_sensor>> b1; //pyramide 2 mal tief


public:
	pyramide();
	~pyramide();

	void set_position(Point p);
	void query(const Mat * input, int pegel);
	void draw_contours(const Mat* frame);
};

