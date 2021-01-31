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
	Mat grey;

	img.copyTo(last_image);

	Rect roi(position.x, position.y, size, size);

	// speichere aktuelle kopie des Bildes
	(*parent_image)(roi).copyTo(img);
	
	// berechne Grauwerte (konvertiere Bild)
	 cvtColor(img, img, cv::COLOR_BGR2GRAY, 1);

	 set_kontrast(img);
	 cv::medianBlur(img, img, 7);

	 cv::threshold(img, grey, 0, 255, cv::THRESH_BINARY_INV + cv::THRESH_OTSU);

	// finde max wert
	max_val = max_pixel(grey);

	// finde min wert
	min_val = min_pixel(grey);
	
	//split black / gray / white
	//set_bw(&img);

	// Storage for blobs
	vector<cv::KeyPoint> keypoints;

	// Detect blobs
	blob_detector -> detect(grey, keypoints);

	grey.copyTo(out);

	// Draw detected blobs as red circles.
	// DrawMatchesFlags::DRAW_RICH_KEYPOINTS flag ensures
	// the size of the circle corresponds to the size of blob

	//HACK out hat 3 chennels
	drawKeypoints(grey, keypoints, out, Scalar(0, 0, 255), DrawMatchesFlags::DRAW_RICH_KEYPOINTS);
	
}

void b_sensor::draw_magnifyied()
{
 	// draw min max
	out.at<PixelColor>(min_val.x, min_val.y) = PixelColor(255, 255, 255);
	out.at<PixelColor>(max_val.x, max_val.y) = PixelColor(0, 0, 0);

	for (auto& a_blob : blob_detector->blobs.get_BlobsList()) {

		unsigned b = a_blob->get_Area();
	

	// draws bounding box
	cv::rectangle(img, a_blob->get_BoundingBox(), cv::Scalar(255, 0, 255), 2);

	// draws centroid
	cv::circle(img, a_blob->get_Centroid(), 5, cv::Scalar(255, 0, 0), 2);

	}
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

			if (l > 205)
			{
				gray->at<uint8_t>(i, j) = 255;
			}
			else if (l < 55)
			{
				gray->at<uint8_t>(i, j) = 0;
			}
			else
			{
				gray->at<uint8_t>(i, j) = 127;
			}
		}

}
