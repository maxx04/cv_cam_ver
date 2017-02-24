#pragma once

using namespace cv;
using namespace std;

void redraw_all(int /*arg*/, void*);

#define KEYPOINT_RADIUS 6.0f
#define KEYPOINT_ANGLE_STEP 6.0f
#define KP_POINTS_IN_CIRCLE (int(360.0f/KEYPOINT_ANGLE_STEP)) //HACK variable hinzufügen


class key_point_gradient
{
public:
	void show(const Mat * input, const String fenster);
	key_point_gradient(Point p);
	static void redraw_keypoint_window(int, void *);
	key_point_gradient();
	void set_active_frame(Mat * frame);
	~key_point_gradient();
	void get_direction(Mat frame, Point pos);
	void check(const Mat * input, int pegel);

	static int active_keypoint;
	static Mat* active_frame;


private:
	static uint index[KP_POINTS_IN_CIRCLE];
	static int8_t dx[KP_POINTS_IN_CIRCLE]; // x in coordinaten kreispunkt vom sensor
	static int8_t dy[KP_POINTS_IN_CIRCLE];	// y in coordinaten vom sensor
	static Mat kp_roi;
	static bool index_ready;
	static short size;
	static Mat keypoint_mat;


protected:

	Point position;
	short direction;
	Scalar c1;
	Scalar c2;
public:

};

