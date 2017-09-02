#pragma once
#include "color_histogram.h"

struct contour_hist
{
	vector<Point> _cnt;
	color_histogram histogram;
};

class contours
{
public:

	//TODO make protected
	// konturenvektor 
	vector<contour_hist> _contours; 


	contours();
	~contours();
	void draw();
	void new_contour(Point p, color_histogram histogram);
	void add_point(Point p);
	//zufuegt kontur zu konturvector
	void add_contour(contour_hist cnt); 
	void draw_contour(const Mat * frame);


};

