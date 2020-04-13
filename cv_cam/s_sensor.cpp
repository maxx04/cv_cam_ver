#include "s_sensor.h"
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>

s_sensor::s_sensor(Mat* parent_image, cv::Point2i p) : sensor(parent_image, p)
{
	test = 3;
}

void s_sensor::proceed(const cv::Mat* region_of_interest)
{
	test = 5;
}

void s_sensor::draw()
{
	rectangle(*parent_image, Rect(position.x, position.y, size, size), Scalar(125, 0, 0), 1);

	for each (Point pnt in key_points)
	{
		drawMarker(*parent_image, position + pnt, Scalar(0, 0, 255), MarkerTypes::MARKER_CROSS, 1);
		//circle((*output_image), pos + pnt, 0, Scalar( 0, 0, 255));
	}
}

void s_sensor::draw_magnifyied()
{

	//CV_Assert((*input).type() == out.type());

	Rect roi(position.x, position.y, size, size);

	(*parent_image)(roi).copyTo(data);	// TODO pruefen ob notwendig ist

	//Mat resMat;

	//cvtColor(out, resMat, COLOR_BGR2HSV);

	//plot_graph("plot");

	//clr_hst.draw(pos);

	//clr_hst.draw_base();

	//cout << "keypoints: " << key_points.size(); // << " - nighbors: " << nighbors.size();
	//cout << " sectors: " << (uint)sectors_nmb << endl;

	//line segments

	Scalar c;
	for each (segment sg in line_segments)
	{
		c = Scalar(sg.C1.x, sg.C1.y, sg.C1.z);
		line(data, sg.P1, sg.P2, c);
	}
	/*
	for (int i = 0; i < sectors_nmb; i++)
	{
		cout << format(" > %3d %3d %3d - %2d - %2d"
			, sectors[i].color.x
			, sectors[i].color.y
			, sectors[i].color.z
			, sectors[i].start
			, sectors[i].end) << endl;




// sectors
		uint8_t half_sz = size / 2;
		for (int k = sectors[i].start; k < sectors[i].end; k++)
		{
			out.at<PixelColor>(dy[k] + half_sz, dx[k] + half_sz) = sectors[i].color;
			//OPTI
			if( k == sectors[i].start)
				out.at<PixelColor>(dy[k] + half_sz, dx[k] + half_sz) = PixelColor(255,0,0);
			if (k == sectors[i].end - 1)
				out.at<PixelColor>(dy[k] + half_sz, dx[k] + half_sz) = PixelColor(0,0,255);
		}

	}
	*/

	//keypoint anzeigen
	//for each (Point p in key_points) out.at<PixelColor>(p.y, p.x) = PixelColor(255, 255, 0);

	//Ringfarbe anzeigen
	//circle(out, Point(get_size() / 2), 8, Scalar(P1.x, P1.y, P1.z), -1);

	//Ringfarbe anzeigen
	//circle(out, Point(get_size() / 2), 4, Scalar(P2.x, P2.y, P2.z), -1);

	//cvtColor(out, out, COLOR_HSV2BGR);
	imshow(sensor_magnifyed_window, data);

#pragma region Test

	//Mat hsvchannel[3];
	//// The actual splitting.
	//split(resMat, hsvchannel);

	//imshow(sensor_result_window, hsvchannel[0]);

#pragma endregion


}
