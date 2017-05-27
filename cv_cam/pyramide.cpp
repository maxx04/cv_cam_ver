#include "stdafx.h"
#include "pyramide.h"


pyramide::pyramide()
{
	
}


pyramide::~pyramide()
{

}

void pyramide::set_position(Point p)
{
	b1.set_position(p);
}

void pyramide::query(const Mat * input, int pegel)
{
	b1.query_all(input, pegel);
}



void pyramide::draw_contours(const Mat* frame)
{
	b1.draw_contours(frame);
}
