#pragma once
#include "color_histogram.h"
class contours
{
	struct contour_hist
	{
		vector<Point> _cnt;
		color_histogram histogram;
	};

public:
	contours();
	~contours();
	void draw();
	void add(color_histogram h);

protected:
	vector<contour_hist> _contours;
};

