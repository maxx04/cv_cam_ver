#pragma once

extern class key_point_gradient;
class key_points_set
{
	vector<key_point_gradient> key_points_global;
	int active_keypoint;

public:
	key_points_set();
	~key_points_set();
	static void redraw_keypoint_window(int, void *);
	void add_point(key_point_gradient p);
};

