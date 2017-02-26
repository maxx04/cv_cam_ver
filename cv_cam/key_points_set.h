#pragma once
class key_point_gradient;
class key_points_set
{

	static int active_keypoint;
	static int keypoints_number;


public:
	static vector<key_point_gradient> keypoints_vector;
	static Mat* activ_frame;

	key_points_set();
	~key_points_set();
	static void redraw_keypoint_window(int, void *);
	void add_point(key_point_gradient p);
};

