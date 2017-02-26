#include "stdafx.h"
#include "key_point_gradient.h"
#include "key_points_set.h"


key_points_set::key_points_set()
{
	namedWindow("keypoint", WINDOW_NORMAL || WINDOW_KEEPRATIO);
	//setMouseCallback("keypoint", onMouse_color, 0);

	createTrackbar("sensor N", "keypoint",
		&active_keypoint, 100, redraw_keypoint_window); //HACK 100
														
	//createTrackbar("pegel", "keypoint", &pegel, 100, pegel_check);

	//imshow("keypoint", keypoint_mat);
}

void key_points_set::redraw_keypoint_window(int /*arg*/, void*)
{
	/* key_point_gradient m = (&vector_pointer)[active_keypoint];

	Rect roi(m.position.x - size / 2, m.position.y - size / 2, size, size);

	(*active_frame)(roi).copyTo(keypoint_mat);
	*/
	//imshow("keypoint", keypoint_mat);

}

void key_points_set::add_point(key_point_gradient p)
{
	key_points_global.push_back(p);
}


key_points_set::~key_points_set()
{
}
