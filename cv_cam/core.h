#pragma once

typedef cv::Point3_<uint8_t> PixelColor;

const enum color_distance_enum {
	RGB_DISTANCE = 0,
	RGB_SQUARE,
	RGB_3SUM,
	RGB_SUM_EACH_COLOR,
	RGB_MAX_EACH_COLOR
};

short color_distance(PixelColor Pixel0, PixelColor Pixel1, int8_t function_nr);
PixelColor middle_color(PixelColor PA, PixelColor PB);

class core
{
public:
	core();
	~core();
};

