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

	//split black / gray / white
	//set_bw(&img);

	// Storage for blobs
	vector<cv::KeyPoint> keypoints;

	// Detect blobs
	blob_detector -> detect(img, keypoints);

	// Draw detected blobs as red circles.
	// DrawMatchesFlags::DRAW_RICH_KEYPOINTS flag ensures
	// the size of the circle corresponds to the size of blob

	drawKeypoints(img, keypoints, out, Scalar(0, 0, 255), DrawMatchesFlags::DRAW_RICH_KEYPOINTS);

	

}

void b_sensor::draw_magnifyied()
{

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

			if (l > 200)
			{
				gray->at<uint8_t>(i, j) = 255;
			}
			else if (l < 100)
			{
				gray->at<uint8_t>(i, j) = 0;
			}
			else
			{
				gray->at<uint8_t>(i, j) = 127;
			}
		}

}
