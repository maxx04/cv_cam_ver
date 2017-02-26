#include "stdafx.h"
#include "key_point_gradient.h"
#include "key_points_set.h"

int key_points_set::active_keypoint;
int key_points_set::keypoints_number;
Mat* key_points_set::activ_frame;
vector<key_point_gradient> key_points_set::keypoints_vector;

key_points_set::key_points_set()
{
	keypoints_number = keypoints_vector.size();
	namedWindow("keypoint", WINDOW_NORMAL || WINDOW_KEEPRATIO);
	//setMouseCallback("keypoint", onMouse_color, 0);

	createTrackbar("Keypoint N", "keypoint",
		&active_keypoint, 100, redraw_keypoint_window); //HACK 100
														
	//createTrackbar("pegel", "keypoint", &pegel, 100, pegel_check);

	//imshow("keypoint", keypoint_mat);
}

void key_points_set::redraw_keypoint_window(int /*arg*/, void*)
{
	keypoints_number = keypoints_vector.size()-1;

cv:setTrackbarMax("Keypoint N", "keypoint", keypoints_number);

	key_point_gradient m = keypoints_vector[active_keypoint];

	m.show(activ_frame, "keypoint");

}

void key_points_set::add_point(key_point_gradient p)
{
	keypoints_vector.push_back(p);
}


key_points_set::~key_points_set()
{
}
