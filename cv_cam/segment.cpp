#include "segment.h"

segment::segment()
{
}

segment::segment(Point p1, Point p2, PixelColor c1, PixelColor c2)
{
	P1 = p1;
	P2 = p2;
	C1 = c1;
	C2 = c2;
}

segment::~segment()
{
}

//void segment::draw(Mat* frame)
//{
//
//}

