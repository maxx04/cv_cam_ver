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
		return 2*(Pixel0.x - Pixel1.x) + (Pixel0.z - Pixel1.z);

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

cv::Vec3b HSVtoBGR(const cv::Vec3f& hsv)
{
	cv::Mat_<cv::Vec3f> hsvMat(hsv);
	cv::Mat_<cv::Vec3f> bgrMat;

	cv::cvtColor(hsvMat, bgrMat, CV_HSV2BGR);

	bgrMat *= 255; // Upscale after conversion

				   // Conversion to Vec3b is handled by OpenCV, no need to static_cast
	return bgrMat(0);
}

 RGB HSVToRGB(HSV hsv) 
{
	double r = 0, g = 0, b = 0;

	if (hsv.S == 0)
	{
		r = hsv.V;
		g = hsv.V;
		b = hsv.V;
	}
	else
	{
		int i;
		double f, p, q, t;

		if (hsv.H == 360)
			hsv.H = 0;
		else
			hsv.H = hsv.H / 60;

		i = (int)trunc(hsv.H);
		f = hsv.H - i;

		p = hsv.V * (1.0 - hsv.S);
		q = hsv.V * (1.0 - (hsv.S * f));
		t = hsv.V * (1.0 - (hsv.S * (1.0 - f)));

		switch (i)
		{
		case 0:
			r = hsv.V;
			g = t;
			b = p;
			break;

		case 1:
			r = q;
			g = hsv.V;
			b = p;
			break;

		case 2:
			r = p;
			g = hsv.V;
			b = t;
			break;

		case 3:
			r = p;
			g = q;
			b = hsv.V;
			break;

		case 4:
			r = t;
			g = p;
			b = hsv.V;
			break;

		default:
			r = hsv.V;
			g = p;
			b = q;
			break;
		}

	}

	return RGB((unsigned char)(r * 255), (unsigned char)(g * 255), (unsigned char)(b * 255));
}