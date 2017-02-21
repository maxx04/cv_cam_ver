// cv_cam.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "cv_cam.h"
#include "m_sensor.h"
#include "sensor_set.h"
#include "key_point_gradient.h"

using namespace cv;
using namespace std;


int sensor_nr = 0;
int pegel = 25;
int number_sensors;

vector<key_point_gradient> key_points_global(100);

Mat frame0, frame1, fg0, fg1;

sensor_set s_set;

static void help()
{
	std::cout << "\nThis program demonstrates\n"
		<< "Usage: \n" << "demhist [videoe_name -- Defaults to ../data/baboon.avi]" << std::endl;
}

const char* keys =
{
	"{help h||}{@image|../data/baboon.avi|input video file}"
};

static void redraw_all(int /*arg*/, void*)
{

	Mat tmp;
	frame1.copyTo(tmp);

	s_set.select_sensor(sensor_nr);

	cout << "Sensor: " << sensor_nr << endl;

	//zeichne sensor und nachbarn
	s_set.draw_selected_sensor(&tmp);

	//zeichne keypoints
	s_set.show_keypoints(&tmp);

	s_set.show_line_segments(&tmp);

	//zeige fenster
	imshow("points", tmp);

	s_set.magnify_selected_sensor(&frame1, "magnify");

}

static void pegel_check(int /*arg*/, void*)
{
	s_set.check_sensors(&frame1, pegel);
	redraw_all(0, 0);
}

static void onMouse(int event, int x, int y, int, void*)
{

	if (event == EVENT_LBUTTONDOWN)
	{
		sensor_nr = s_set.find_sensor(x, y);

		cv::setTrackbarPos("sensor N", "magnify", sensor_nr);

	}

	return;
}

static void onMouse_color(int event, int x, int y, int, void*)
{
	if (event == EVENT_LBUTTONDOWN)
	{
		//Draw color
		Pixel a = s_set.get_color(x, y, &frame1);

		cout << format("%02d - %02d", x, y) << " >> " << format("%3d:%3d:%3d", a.x,a.y,a.z) << endl;


	}
}

int main(int argc, const char * argv[])
{

	CommandLineParser parser(argc, argv, keys);
	if (parser.has("help"))
	{
		help();
		return 0;
	}

	string inputVideo = parser.get<string>(0);

	VideoCapture cam(inputVideo);

	if (!cam.isOpened()) {
		//error in opening the video input
		cerr << "Unable to open video file: " << endl;
		exit(EXIT_FAILURE);
	}


	cam.read(frame0);

	s_set = sensor_set(frame0, 500);

	number_sensors = s_set.number_sensors;

	
	namedWindow("points", WINDOW_NORMAL);
	setMouseCallback("points", onMouse, 0);

	namedWindow("magnify", WINDOW_NORMAL || WINDOW_KEEPRATIO);
	setMouseCallback("magnify", onMouse_color, 0);

	createTrackbar("sensor N", "magnify", &sensor_nr, number_sensors, redraw_all);
	createTrackbar("pegel", "magnify", &pegel, 100, pegel_check);



	for (int i = 0; cam.read(frame1); i++)
	{

		//cvtColor(frame1, fg1, COLOR_BGR2Lab);
		frame1.copyTo(fg1);

		const double start = (double)getTickCount();

		s_set.check_sensors(&fg1, pegel);

		const double timeSec = (getTickCount() - start) / getTickFrequency();

		cout << i << " - ElapsedTime: " << timeSec << " sec " << endl;

		key_points_global.clear();

		s_set.add_keypoints(&key_points_global);

		cout << "Keypoints:" << key_points_global.size() << endl;

		redraw_all(0, 0);

		waitKey();

		fg1.copyTo(fg0);

	}

	waitKey();
	return 0;
}