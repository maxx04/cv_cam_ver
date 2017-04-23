#include "stdafx.h"
#include "core.h"


core::core()
{
}


core::~core()
{
}


short color_distance(PixelColor Pixel0, PixelColor Pixel1, int8_t function_nr)
{
	int m = 9;
	switch (function_nr)
	{
	case RGB_SQUARE:
		return (Pixel0.x - Pixel1.x) ^ 2 + (Pixel0.y - Pixel1.y) ^ 2 + (Pixel0.z - Pixel1.z) ^ 2;

	case RGB_3SUM:
		return (((Pixel0.x + Pixel0.y + Pixel0.z) - (Pixel1.x + Pixel1.y + Pixel1.z)));

	case RGB_SUM_EACH_COLOR:
		return abs(Pixel0.x - Pixel1.x) + abs(Pixel0.y - Pixel1.y) + abs(Pixel0.z - Pixel1.z);

	case RGB_MAX_EACH_COLOR:
		m = max(abs(Pixel0.x - Pixel1.x), abs(Pixel0.z - Pixel1.z));
		return max(abs(Pixel0.x - Pixel1.x), m);

	case HSV_HV:
		return (Pixel0.x - Pixel1.x) + (Pixel0.z - Pixel1.z);

	default:
		return abs(Pixel0.x - Pixel1.x) + abs(Pixel0.y - Pixel1.y) + abs(Pixel0.z - Pixel1.z);
	}

	return 0;
}

PixelColor middle_color(PixelColor PA, PixelColor PB)
{
	PixelColor tmp;

	//tmp = PA / 2 + PB / 2;

	tmp.x = (PA.x + PB.x) / 2;
	tmp.y = (PA.y + PB.y) / 2;
	tmp.z = (PA.z + PB.z) / 2;

	return tmp;
}
