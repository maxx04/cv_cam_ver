#include "bforce_sensor.h"

/// <param name="_detect_size">subfenster innerhalb die wird Verschiebung gesucht in pixeln</param> 
int bforce_sensor::detect_size = 64;

bforce_sensor::bforce_sensor(Mat* parent_image, cv::Point2i p, int _size = 16) : sensor(parent_image, p, _size)
{

	if (sensor::sensors_number < 2)
	{
		int max_shift = 16;

		assert(max_shift >= 8 ); // test auf Groesse
		assert(max_shift % 2 == 0); // test auf gerade Zahl

		detect_size = size - 2*max_shift;



		prepare_frame(parent_image);

	}

	Rect roi(position.x, position.y, size, size); // HACK bild war anders kopiert vorbereited durch sensor::

	prepared_frame(roi).copyTo(img);

	shift = { 0, 0 };

}

void bforce_sensor::proceed()
{
	img.copyTo(last_image);

	Rect roi(position.x, position.y, size, size);

	// speichere aktuelle kopie des Bildes
	sensor::prepared_frame(roi).copyTo(img);

	cv::normalize(img, img, 0, 255, cv::NORM_MINMAX, CV_8U);

	find_shift();

	key_points.push_back(shift);

}

void bforce_sensor::draw_magnifyied()
{
	find_shift();

	imshow(sensor_magnifyed_window, img);
	imshow(sensor_result_window, out);
	imshow(sensor_magnifyed_window_last, last_image);

}

void bforce_sensor::set_bw(Mat* gray)
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

void bforce_sensor::prepare_frame(cv::Mat* frame)
{

	// berechne Grauwerte (konvertiere Bild)
	cvtColor(*frame, prepared_frame, cv::COLOR_BGR2GRAY, 1);

	cv::medianBlur(prepared_frame, prepared_frame, 7);

	return;

}


float bforce_sensor::calculate_mean(cv::Mat image)
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

//float bforce_sensor::sub(cv::Mat image1, cv::Mat image2, cv::Mat* out_image)
//{
//
//	//TODO muessen gleich sein
//
//	float diff = 0.0;
//										  
//	for (int i = 0; i < image1.cols; i++)
//		for (int j = 0; j < image1.rows; j++)
//		{
//			uint8_t m = abs(image1.at<uint8_t>(i, j)- image2.at<uint8_t>(i, j));
//			out_image->at<uint8_t>(i, j) = m;
//			diff += (float)m;
//		}
//	return diff;
//}


cv::Point2i bforce_sensor::find_shift()
{
	// TODO: Fügen Sie hier Ihren Implementierungscode ein..
	cv::Rect base((48 - detect_size/2), (48 - detect_size / 2), detect_size, detect_size);

	cv::Point2i sh = { 0, 0 }; // shift is max +- 16

	Mat i1, i2, res;

	i1 = last_image(base);

	float s_min = 10e8;
	Point2i shift_min = { 0,0 };

	int max_shift = (size - detect_size) / 2;

	for (int i = -max_shift; i < max_shift; i++)
		for (int j = -max_shift; j < max_shift; j++)
		{
			sh.x = i;
			sh.y = j;

			cv::Rect shifted_img(max_shift + sh.x, max_shift + sh.y, detect_size, detect_size);

			i2 = img(shifted_img);

			cv::subtract(i1, i2, res);
			float s = calculate_mean(res);

			if (s_min > s)
			{
				shift_min = sh;
				s_min = s;
			}

		}

	cv::Rect shifted_img(max_shift + shift_min.x, max_shift + shift_min.y, detect_size, detect_size);

	cv::subtract(i1, img(shifted_img), out);

	cout << "min " << shift_min << "  " << s_min << endl;

	shift = shift_min;

	return shift_min;
}
