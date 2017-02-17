#pragma once
class segment
{
public:
	segment();
	segment(Point p1, Point p2, Pixel c1, Pixel c2);
	~segment();

	Point P1; //HACK optimize to ushort
	Point P2; //HACK protect
	Pixel C1;
	Pixel C2;

private:


};