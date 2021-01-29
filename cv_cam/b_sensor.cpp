#include "b_sensor.h"

b_sensor::b_sensor(Mat* parent_image, cv::Point2i p, int _size = 16) : sensor(parent_image, p, _size)
{

	// berechne Grauwerte (konvertiere Bild)
	cvtColor(img, last_image, cv::COLOR_BGR2GRAY);

	// Setup SimpleBlobDetector parameters.
	myBlobDetector::Params params;

	// Change thresholds
	params.minThreshold = 100;
	params.maxThreshold = 200;


	// Set up detector with params
	blob_detector = new myBlobDetector(params);

}

void b_sensor::proceed()
{
	Mat gray;

	img.copyTo(last_image);

	Rect roi(position.x, position.y, size, size);

	// speichere aktuelle kopie des Bildes
	(*parent_image)(roi).copyTo(img);
	
	// berechne Grauwerte (konvertiere Bild)
	 cvtColor(img, img, cv::COLOR_BGR2GRAY);

	set_kontrast(img);
	cv::medianBlur(img, img, 5);

	// finde max wert
	max_val = max_pixel(img);

	// finde min wert
	min_val = min_pixel(img);
	
	//split black / gray / white
	//set_bw(&img);

	// Storage for blobs
	vector<cv::KeyPoint> keypoints;

	// Detect blobs
	blob_detector -> detect(img, keypoints);

	img.copyTo(out);

	// Draw detected blobs as red circles.
	// DrawMatchesFlags::DRAW_RICH_KEYPOINTS flag ensures
	// the size of the circle corresponds to the size of blob

	//HACK out hat 3 chennels
	drawKeypoints(img, keypoints, out, Scalar(0, 0, 255), DrawMatchesFlags::DRAW_RICH_KEYPOINTS);

	
}

void b_sensor::draw_magnifyied()
{
 	// draw min max
	out.at<PixelColor>(min_val.x, min_val.y) = PixelColor(255, 255, 255);
	out.at<PixelColor>(max_val.x, max_val.y) = PixelColor(0, 0, 0);

	imshow(sensor_magnifyed_window, img);
	imshow(sensor_result_window, out);
	imshow(sensor_magnifyed_window_last, last_image);

}

void b_sensor::set_bw(Mat* gray)
{
	for (int i = 0; i < gray->cols; i++)
		for (int j = 0; j < gray->rows; j++)
		{
			uint8_t l = gray->at<uint8_t>(i, j);

			if (l > 225)
			{
				gray->at<uint8_t>(i, j) = 255;
			}
			else if (l < 25)
			{
				gray->at<uint8_t>(i, j) = 0;
			}
			else
			{
				gray->at<uint8_t>(i, j) = 127;
			}
		}

}
