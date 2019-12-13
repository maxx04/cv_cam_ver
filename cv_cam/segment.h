#pragma once
#include "core.h"

using namespace cv;

class segment
{
public:
	segment();
	segment(Point p1, Point p2, PixelColor c1, PixelColor c2);
	~segment();

	Point P1; //HACK optimize to ushort
	Point P2; //HACK protect
	PixelColor C1;
	PixelColor C2;

private:


};