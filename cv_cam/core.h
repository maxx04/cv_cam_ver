#pragma once



const enum color_distance_enum {
	RGB_DISTANCE = 0,
	RGB_SQUARE,
	RGB_3SUM,
	RGB_SUM_EACH_COLOR,
	RGB_MAX_EACH_COLOR
};

short color_distance(Pixel Pixel0, Pixel Pixel1, int8_t function_nr);

class core
{
public:
	core();
	~core();
};

