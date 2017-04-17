// cv_cam.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "cv_cam.h"
#include "m_sensor.h"
#include "sensor_set.h"
#include "key_points_set.h"

using namespace cv;
using namespace std;

int sensor_nr = 0;
int pegel = 25;
int number_sensors;

Mat frame0, frame1, fg0, fg1;

sensor_set s_set;
key_points_set k_set; // global

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

	//zeichne sensors ohne keypoints
	s_set.show_flats(&tmp);

	//zeichne keypoints
	s_set.show_keypoints(&tmp);

	//zeichne linien
	s_set.show_line_segments(&tmp);

	//zeige bild
	imshow("points", tmp);

	//zeichne ausgewaehlte sensor
	s_set.magnify_selected_sensor(&frame1, "magnify");


}

static void pegel_check(int /*arg*/, void*)
{
	s_set.check_sensors(&frame1, pegel); // berechne sensoren neu
	s_set.add_keypoints(&k_set, &frame1); // ausleite sensoren neu
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
		PixelColor a = s_set.get_color(x, y, &frame1);

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

	s_set = sensor_set(frame0, 1800);

	number_sensors = s_set.number_sensors;
	
	namedWindow("points", WINDOW_NORMAL);
	setMouseCallback("points", onMouse, 0);

	namedWindow("magnify", WINDOW_NORMAL || WINDOW_KEEPRATIO);
	setMouseCallback("magnify", onMouse_color, 0);

	createTrackbar("sensor N", "magnify", &sensor_nr, number_sensors, redraw_all);
	createTrackbar("pegel", "magnify", &pegel, 100, pegel_check);

	//////////////////

	vector<vector<Point>> contours;
	vector<Vec4i> hierarchy;
	Mat tmp2 = Mat::zeros(frame0.rows, frame0.cols, CV_8UC1);
	Mat tmp1;


	for (int i = 0; cam.read(frame1); i++)
	{

		//cvtColor(frame1, fg1, COLOR_BGR2Lab);
		//GaussianBlur(frame1, fg1, Size(3, 3),3.6f); // smooth
		frame1.copyTo(fg1);
/*
		threshold(fg1, tmp1, 30, 160, THRESH_BINARY_INV); // neues bild mit grenze

		tmp1.convertTo(tmp2, CV_8U); // konvertieren in  8 bit format
		cvtColor(tmp2, tmp1, CV_BGR2GRAY); // konvertieren in grauform

		findContours(tmp1, contours, hierarchy,
			CV_RETR_CCOMP, CV_CHAIN_APPROX_TC89_L1); //fnde konturen

		fg1.copyTo(frame1);


		Moments M;
		double A[7];
		Mat f;

		// iterate through all the top-level contours,
		// draw each connected component with its own random color
 		int idx = 0;
		for (; idx >= 0; idx = hierarchy[idx][0])
		{
			Scalar color(rand() & 255, rand() & 255, rand() & 255);
			cout << idx << " - " << contours[idx].size() << endl;
			M = moments(contours[idx]);
			HuMoments(M, A);

			cout << format("%f" ,A[0]) << endl;

 			drawContours(frame1, contours, idx, color, 3, 8, hierarchy);
		}

*/

		const double start = (double)getTickCount();

// --------------------------------------------------------------------------------
//	finde keypoints
		s_set.check_sensors(&fg1, pegel);
//	verfine keypoints
		//s_set.calculate_float_keypoints();
// --------------------------------------------------------------------------------

		const double timeSec = (getTickCount() - start) / getTickFrequency();

		cout << i << " - ElapsedTime: " << timeSec << " sec " << endl;

		s_set.add_keypoints(&k_set, &frame1);

		//cout << "Keypoints:" << k_set.keypoints_vector.size() << endl;

		redraw_all(0, 0);

		waitKey();

		fg1.copyTo(fg0);

	}

	waitKey();
	return 0;
}