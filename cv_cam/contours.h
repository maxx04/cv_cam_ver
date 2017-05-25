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
	contours();
	~contours();
	void draw();
	void new_contour(Point p, color_histogram histogram);
	void add_point(Point p);

	vector<contour_hist> _contours; //TODO make protected
};

