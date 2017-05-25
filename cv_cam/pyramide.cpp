#include "stdafx.h"
#include "pyramide.h"


pyramide::pyramide()
{
	//b3 = new block<m_sensor>();
	//b1.set_position(Point(100, 100));
	//b1.size++;
}

pyramide::pyramide(Point p)
{
	set_position(Point(100, 100));
}


pyramide::~pyramide()
{

}

void pyramide::set_position(Point p)
{
	b1.set_position(p);
	b1.size++;
	//b1.k[0] = 1;
}
