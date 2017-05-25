#pragma once
#include "opencv2/opencv.hpp"
#include "segment.h"
#include "color_histogram.h"

using namespace cv;
using namespace std;


#define SENSOR_RADIUS 11.0f
#define SENSOR_ANGLE_STEP 6.0f
#define POINTS_IN_CIRCLE (int(360.0f/SENSOR_ANGLE_STEP)) //HACK variable hinzufügen
#define MAX_SECTORS 80
#define BIG_SECTOR 6



class m_sensor
{	

	struct Sector
	{
		uint8_t start;
		uint8_t end;
		PixelColor color;
		//ushort distance;
	};



	Point pos;	// position Mitte
	uint size;	// groesse 
	short values[POINTS_IN_CIRCLE];

	//PixelColor P2;

	static uint index[POINTS_IN_CIRCLE];
	static int8_t dx[POINTS_IN_CIRCLE]; // x in coordinaten kreispunkt vom sensor
	static int8_t dy[POINTS_IN_CIRCLE];	// y in coordinaten vom sensor
	static Mat out;
	static uint sensors_number;

	static short smooth_index[9];

	ushort sectors_nmb = 0;
	Sector sectors[MAX_SECTORS]; //HACK optimieren

	color_histogram clr_hst; //TODO spaeter soll entfernt werden, zu viel speicher.

public:
	list<ushort> nighbors; //HACK umbauen in private member
	vector<Point> key_points;
	vector<segment> line_segments;

	m_sensor(Point p, uint sz);
	m_sensor();
	~m_sensor();

	void query(const Mat* input, int pegel);
	void search_keypoints(short * values, int pegel);
	void search_sectors(PixelColor next_pixel, int pegel);
	void search_sectors(Mat * sensor_mat, int pegel, const color_distance_enum dist);
	Point get_position();
	void set_position(Point p);
	Size get_size();
	inline short get_next_ring_value(ushort index, int8_t versatz);

	void smooth_values( uint8_t range);
	void show(const Mat * input, uint magnify);
	void show(const Mat * input, const String fenster);
	void plot_graph(const String fenster);
	void add_points(vector<Point> * global_points);
	bool cross(Point left_upper_pos);
	bool cross(m_sensor * m);
	ushort get_distance_to_middle(int x, int y);
	PixelColor get_color(int x, int y, const Mat * input);
	void add_line_segments();
	bool connect_sectors(Sector S1, Sector S2, list<Sector>* ls);
	void create_sectors_array(const Mat* out, list<Sector>* output_list);
	void create_sectors_array(const Mat * out_ready, Sector * sv);
	void connect_sectors(list<Sector>* input_list, list<Sector>* output_list, int pegel);
	inline PixelColor smooth_color(PixelColor * p);
	void m_sensor::mark_global(const Mat * input); //HACK check fuer bildgroesse 
	void soi(const Mat* src, Mat* dst, uint magnify);

};


