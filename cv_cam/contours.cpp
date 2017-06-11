#include "stdafx.h"
#include "contours.h"


contours::contours()
{
}


contours::~contours()
{
}

void contours::draw()
{

}

void contours::new_contour(Point p, color_histogram histogram)
{
	contour_hist c1;
	c1._cnt.push_back(p);
	c1.histogram = histogram;
	_contours.push_back(c1);
}

void contours::add_point(Point p)
{
	int size = _contours.size()-1; //HACK wenn noch keine konturen
	_contours[size]._cnt.push_back(p); //HACK letzte kontour ??
}

void contours::add_contour(contour_hist cnt)
{
	_contours.push_back(cnt);
}

void contours::draw_contour(const Mat* frame)
{
	for each (contour_hist c in _contours)
	{

		Point pa = c._cnt[0];

		Scalar color = c.histogram.get_max_color(); 
		//Scalar(rand() & 255, rand() & 255, rand() & 255);

		for each (Point p in c._cnt)
		{
			assert(p.y != 0);
			cv::line((*frame), pa, p, color);
			pa = p;
		}

	}
}

