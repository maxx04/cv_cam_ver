#include "core.h"
#include "opencv2/imgproc.hpp"


core::core()
{
}


core::~core()
{
}


short color_distance(PixelColor Pixel0, PixelColor Pixel1, const color_distance_func function_nr = RGB_3SUM)
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
		m = std::max(abs(Pixel0.x - Pixel1.x), abs(Pixel0.z - Pixel1.z));
		return std::max(abs(Pixel0.x - Pixel1.x), m);

	case HSV_HV:
		HSV a = RGBToHSV2(RGB(Pixel0.z, Pixel0.y, Pixel0.x));
		HSV b = RGBToHSV2(RGB(Pixel1.z, Pixel1.y, Pixel1.x));

		//if (a.S * b.V < 0.032)	return abs(a.V - b.V);
		return 10*(a.H - b.H);
		

	//default:
	//	return abs(Pixel0.x - Pixel1.x) + abs(Pixel0.y - Pixel1.y) + abs(Pixel0.z - Pixel1.z);
	}

	return 0;
}

PixelColor middle_color(PixelColor PA, PixelColor PB)
{
	PixelColor tmp;

	tmp.x = (PA.x + PB.x) / 2;
	tmp.y = (PA.y + PB.y) / 2;
	tmp.z = (PA.z + PB.z) / 2;

	return tmp;
}

cv::Vec3b HSVtoBGR(const cv::Vec3f& hsv)
{
	cv::Mat_<cv::Vec3f> hsvMat(hsv);
	cv::Mat_<cv::Vec3f> bgrMat;

	cv::cvtColor(hsvMat, bgrMat, cv::COLOR_HSV2BGR);

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

static double Min(double a, double b) {
	return a <= b ? a : b;
}

static double Max(double a, double b) {
	return a >= b ? a : b;
}

HSV RGBToHSV(RGB rgb) {
	double delta, min;
	double h = 0, s, v;

	min = Min(Min(rgb.R, rgb.G), rgb.B);
	v = Max(Max(rgb.R, rgb.G), rgb.B);
	delta = v - min;

	if (v == 0.0)
		s = 0;
	else
		s = delta / v;

	if (s == 0)
		h = 0.0;

	else
	{
		if (rgb.R == v)
			h = (rgb.G - rgb.B) / delta;
		else if (rgb.G == v)
			h = 2 + (rgb.B - rgb.R) / delta;
		else if (rgb.B == v)
			h = 4 + (rgb.R - rgb.G) / delta;

		h *= 60;

		if (h < 0.0)
			h = h + 360;
	}

	return HSV(h, s, (v / 255));
}

/* 
Input 0 <= B <= 255, 0 <= G <= 255, 0 <= R <= 255
Output 0 <= H <= 360, 0 <= S <= 1, 0 <= V <= 1  
*/

HSV RGBToHSV2(const RGB rgb)
{
	//OPTI optimieren

	cv::Vec3b bgr;
	HSV hsv_out;

	bgr[0] = rgb.B;
	bgr[1] = rgb.G;
	bgr[2] = rgb.R;

	cv::Mat3f bgrMat(static_cast<cv::Vec3f>(bgr));

	bgrMat *= 1. / 255.;

	cv::Mat3f hsvMat;
	cv::cvtColor(bgrMat, hsvMat, cv::COLOR_BGR2HSV);

	cv::Vec3f hsv = hsvMat(0, 0);

	hsv_out.H = hsv[0];
	hsv_out.S = hsv[1];
	hsv_out.V = hsv[2];

	return hsv_out;
}