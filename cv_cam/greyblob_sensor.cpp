#include "greyblob_sensor.h"

myBlobDetector* greyblob_sensor::blob_detector = 0;

Ptr<FeatureDetector> greyblob_sensor::orb_detector = 0;

greyblob_sensor::greyblob_sensor(Mat* parent_image, cv::Point2i p, int _size = 16) : sensor(parent_image, p, _size)
{

	// berechne Grauwerte (konvertiere Bild)
	//cvtColor(img, last_image, cv::COLOR_BGR2GRAY);

	if (sensor::sensors_number < 2)
	{

		// Setup SimpleBlobDetector parameters.
		myBlobDetector::Params params;

		// Change thresholds
		params.minThreshold = 100;
		params.maxThreshold = 200;


		// Set up detector with params
		blob_detector = new myBlobDetector(params);

		// create orb detector
		orb_detector = ORB::create(50, 1.2, 4, 20, 0, 2, cv::ORB::HARRIS_SCORE, 15, 20);

		prepare_frame(parent_image);

	}

	Rect roi(position.x, position.y, size, size); // HACK bild war anders kopiert vorbereited durch sensor::

	prepared_frame(roi).copyTo(img);

	min_val = { 0,0,0 };
	max_val = { 0,0,0 };
	shift = { 0, 0 };

}

void greyblob_sensor::proceed()
{
	img.copyTo(last_image);

	Rect roi(position.x, position.y, size, size);

	// speichere aktuelle kopie des Bildes
	sensor::prepared_frame(roi).copyTo(img);

	cv::normalize(img, img, 0, 255, cv::NORM_MINMAX, CV_8U);

	//orb_detector->detect(img, keypoints);

	//cout << keypoints.size() << endl;

	//cv::threshold(img, out, 55, 255, cv::THRESH_BINARY_INV);

	find_shift();

	// Detect blobs
	//blob_detector->detect(out, keypoints);

}

void greyblob_sensor::draw_magnifyied()
{
	Point2i s = find_shift();

	//cv::threshold(img, out, 175, 255, cv::THRESH_BINARY);

	//blob_detector->detect(out, keypoints);

	//for (auto& a_blob : blob_detector->blobs.get_BlobsList()) {

	//	// extract and draws blob contour
	//	auto contours = a_blob->get_Contour().get_ContourPolygon();

	//	if (contours.size() != 1) {
	//		for (auto& iter = contours.begin(); iter != contours.end(); iter++) {
	//			auto next_iter = iter;
	//			next_iter++;
	//			if (next_iter == contours.end())
	//				cv::line(img, *iter, contours[0], cv::Scalar(0, 0, 255), 1, 8);
	//			else
	//				cv::line(img, *iter, *next_iter, cv::Scalar(0, 0, 255), 1, 8);
	//		}
	//	}

	// draws bounding box
	//cv::rectangle(img, a_blob->get_BoundingBox(), cv::Scalar(255, 0, 255), 2);

	// draws centroid
	//cv::circle(img, a_blob->get_Centroid(), 2, cv::Scalar(255, 0, 0), 1);

	//}

	// Draw detected blobs as red circles.
	// DrawMatchesFlags::DRAW_RICH_KEYPOINTS flag ensures
	// the size of the circle corresponds to the size of blob

	// Draw detected blobs as red circles.
	// DrawMatchesFlags::DRAW_RICH_KEYPOINTS flag ensures
	// the size of the circle corresponds to the size of blob
	//HACK out hat 3 chennels
	//drawKeypoints(img, keypoints, out, Scalar(0, 0, 255), DrawMatchesFlags::DEFAULT);

	imshow(sensor_magnifyed_window, img);
	imshow(sensor_result_window, out);
	imshow(sensor_magnifyed_window_last, last_image);

}

void greyblob_sensor::set_bw(Mat* gray)
{
	for (int i = 0; i < gray->cols; i++)
		for (int j = 0; j < gray->rows; j++)
		{
			uint8_t l = gray->at<uint8_t>(i, j);

			if (l > 205)
			{
				gray->at<uint8_t>(i, j) = 255;
			}
			else if (l < 35)
			{
				gray->at<uint8_t>(i, j) = 0;
			}
			else
			{
				gray->at<uint8_t>(i, j) = 128;
			}
		}

}

void greyblob_sensor::prepare_frame(cv::Mat* frame)
{

	// berechne Grauwerte (konvertiere Bild)
	cvtColor(*frame, prepared_frame, cv::COLOR_BGR2GRAY, 1);

	cv::medianBlur(prepared_frame, prepared_frame, 5);

	return;

}


float greyblob_sensor::calculate_mean(cv::Mat image)
{

	float sum = 0.0;

	for (int i = 0; i < image.cols; i++)
		for (int j = 0; j < image.rows; j++)
		{
			uint8_t m = image.at<uint8_t>(i, j);
			sum += (float)m;
		}
	return sum;
}

float greyblob_sensor::sub(cv::Mat image1, cv::Mat image2, cv::Mat* out_image)
{

	//TODO muessen gleich sein

	float diff = 0.0;
										  
	for (int i = 0; i < image1.cols; i++)
		for (int j = 0; j < image1.rows; j++)
		{
			uint8_t m = abs(image1.at<uint8_t>(i, j)- image2.at<uint8_t>(i, j));
			out_image->at<uint8_t>(i, j) = m;
			diff += (float)m;
		}
	return diff;
}


cv::Point2i greyblob_sensor::find_shift()
{
	// TODO: Fügen Sie hier Ihren Implementierungscode ein..
	cv::Rect base((48 - 32), (48 - 32), 64, 64);

	cv::Point2i sh = { 0, 0 }; // shift is max +- 16

	Mat i1, i2, res;

	i1 = last_image(base);

	float s_min = 10e8;
	Point2i shift_min = { 0,0 };

	for (int i = -16; i < 16; i++)
		for (int j = -16; j < 16; j++)
		{
			sh.x = i;
			sh.y = j;

			cv::Rect shifted_img((48 - 32) + sh.x, (48 - 32) + sh.y, 64, 64);

			i2 = img(shifted_img);

			cv::subtract(i1, i2, res);
			float s = calculate_mean(res);

			if (s_min > s)
			{
				shift_min = sh;
				s_min = s;
			}

		}

	cv::Rect shifted_img((48 - 32) + shift_min.x, (48 - 32) + shift_min.y, 64, 64);

	cv::subtract(i1, img(shifted_img), out);

	cout << "min " << shift_min << "  " << s_min << endl;

	shift = shift_min;

	return shift_min;
}
