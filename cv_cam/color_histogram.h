#pragma once

struct hst { PixelColor color; ushort treffer; };
struct hst_hsv { HSV color;	float frequency; };

class color_histogram
{

#define COLOR_HISTOGRAM_COLORS 16
#define COLOR_HISTOGRAM_GRAYS 16
#define COLOR_HISTOGRAM_BREITE (1*COLOR_HISTOGRAM_COLORS+COLOR_HISTOGRAM_GRAYS)
#define COLOR_HISTOGRAM_MAIN 8

public:

	static hst_hsv base[COLOR_HISTOGRAM_BREITE]; //TODO make protected

	color_histogram();
	~color_histogram();
	ushort compare(color_histogram * h);
	ushort compare(color_histogram h);
	void reset();
	void add(PixelColor clr, ushort distance);
	void draw(Point start);
	Scalar get_max_color();
protected:

	static bool base_defined;

	uint8_t histogram[COLOR_HISTOGRAM_BREITE]; // treffer für base colors
	//hst_hsv main_clr[COLOR_HISTOGRAM_MAIN]; // noch nicht verwendet, als alternative zu base

};


