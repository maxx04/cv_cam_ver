#pragma once
#include "m_sensor.h"
#include "contours.h"

class key_points_set;

class sensor_set 
{
	vector<m_sensor> sensors;
	int selected_sensor = 0;

public:
	int number_sensors = 100;

	sensor_set(Mat block_1, int sensors);
	sensor_set();
	~sensor_set();

	void select_sensor(int sensor_nr);
	void draw_selected_sensor(Mat* output_image);
	void magnify_selected_sensor(const Mat* input_image, const String magnifyed_view_window);
	void query_sensors(const Mat * input_image, int pegel);
	void add_keypoints(key_points_set * key_points, Mat * block_1);
	void show_keypoints(Mat * output_frame);
	void show_line_segments(Mat * output_frame);
	void show_flats(Mat * output_frame);
	ushort find_sensor(int x, int y);
	PixelColor get_color(int x, int y, const Mat * input);
	contours find_contours(void);

};

