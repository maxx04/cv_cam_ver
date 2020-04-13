#include "sensor_set.h"
#include "color_histogram.h"
#include <iostream>
#include <opencv2/highgui.hpp>
/*#include "contours.h"
#include "pyramide.h"*/					   


using namespace cv;
using namespace std;

int sensor_nr = 0;
int pegel = 8;
const cv::String main_window = "frame";


Mat frame0, frame1, fg0, fg1;

//pyramide pyr;

sensor_set* s_set; // alle Sensoren
//contours cnt;

static void help()
{
	cout << "\nThis program demonstrates\n"
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
	//cvtColor(tmp, tmp, COLOR_BGR2BHSV);

	s_set -> select_sensor(sensor_nr);

 	//pyr.draw_contours(&tmp);

	//zeichne ausgewaehlte sensor
	s_set->draw(&tmp);

	//zeige bild
	imshow(main_window, tmp);

}

static void pegel_check(int /*arg*/, void*)
{
	s_set -> proceed(&frame1, pegel); // berechne sensoren neu

	redraw_all(0, 0);
}

static void onMouse(int event, int x, int y, int, void*)
{

	if (event == EVENT_LBUTTONDOWN)
	{
		sensor_nr = s_set -> find_nearest_sensor(x, y);

		cv::setTrackbarPos("sensor N", sensor::sensor_magnifyed_window, sensor_nr);
	}

	return;
}

static void onMouse_color(int event, int x, int y, int, void*)
{
	if (event == EVENT_LBUTTONDOWN)
	{
		//Draw color

	 	PixelColor a = s_set -> get_color(x, y, &frame1);

		HSV hsv(0.0, 0.0, 0.0);
		RGB rgb(a.z, a.y, a.x);

		hsv = RGBToHSV2(rgb);


		//s_set->get_selected_sensor().proceed(&frame1);

		//color_histogram h = m.get_histogramm();


		//for (size_t i = 0; i < COLOR_HISTOGRAM_BREITE; i++)
		//{
		//	ushort dst = h.compare_with_base(hsv, i);
		//}

	  	cout << format("%02d-%02d", x, y);
		cout << format(" B:%3d G:%3d R:%3d", a.x, a.y, a.z);
		cout << format("--H:%.0f S:%.2f V:%.2f -> %.3f", hsv.H, hsv.S, hsv.V, hsv.S*hsv.V) << endl;


	}

	redraw_all(0, 0);
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
		cerr << "Unable to open video file: " << inputVideo << endl;
		exit(EXIT_FAILURE);
	}

	cam.read(frame0);

	s_set = new sensor_set(frame0, 2800);	
									    
	//pyr.set_position(Point(500, 400));
	
	namedWindow(main_window, WINDOW_KEEPRATIO | WINDOW_GUI_EXPANDED);
	setMouseCallback(main_window, onMouse, 0);

	namedWindow(sensor::sensor_magnifyed_window, WINDOW_KEEPRATIO | WINDOW_GUI_EXPANDED);
	namedWindow(sensor::sensor_result_window,  WINDOW_KEEPRATIO | WINDOW_GUI_EXPANDED);

	setMouseCallback(sensor::sensor_magnifyed_window, onMouse_color, 0);

	createTrackbar("sensor N", sensor::sensor_magnifyed_window, &sensor_nr, s_set-> number_sensors, redraw_all);
	createTrackbar("pegel", sensor::sensor_magnifyed_window, &pegel, 100, pegel_check);

	//////////////////

	//vector<vector<Point>> contours;
	//vector<Vec4i> hierarchy;
	//Mat tmp2 = Mat::zeros(frame0.rows, frame0.cols, CV_8UC1);
	//Mat tmp1;


	for (int i = 0; cam.read(frame1); i++)
	{
		const double start = (double)getTickCount();

// --------------------------------------------------------------------------------
//	finde keypoints, histogramms
		s_set -> proceed(&frame1, pegel);

		//pyr.query(&frame1, pegel);


//	finde konturen, bereiche mit gleichen histogrammen
		//cnt = s_set -> find_contours();
// --------------------------------------------------------------------------------

		const double timeSec = (getTickCount() - start) / getTickFrequency();

		cout << i << " - ElapsedTime: " << timeSec << " sec " << endl;

		redraw_all(0, 0);

		char c = waitKey();

		if (c == 'q')
		{
			break;
		}
	}


 	return 0;
}