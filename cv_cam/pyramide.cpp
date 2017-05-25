#include "stdafx.h"
#include "pyramide.h"


pyramide::pyramide()
{

}

pyramide::pyramide(Point p)
{
	set_position(p);
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
	b1.quiery_all(input, pegel);
}
