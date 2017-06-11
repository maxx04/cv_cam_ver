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
	// konturen
	vector<contour_hist> _contours; //TODO make protected


	contours();
	~contours();
	void draw();
	void new_contour(Point p, color_histogram histogram);
	void add_point(Point p);
	void add_contour(contour_hist cnt);
	void draw_contour(const Mat * frame);


};

