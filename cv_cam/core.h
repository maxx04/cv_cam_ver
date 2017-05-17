#pragma once

typedef cv::Point3_<uint8_t> PixelColor;

const enum color_distance_enum {
	RGB_DISTANCE = 0,
	RGB_SQUARE,
	RGB_3SUM,
	RGB_SUM_EACH_COLOR,
	RGB_MAX_EACH_COLOR,
	HSV_HV
};

short color_distance(PixelColor Pixel0, PixelColor Pixel1, int8_t function_nr);
PixelColor middle_color(PixelColor PA, PixelColor PB);

class core
{
public:
	core();
	~core();
};

class RGB
{
public:
	unsigned char R;
	unsigned char G;
	unsigned char B;

	RGB(unsigned char r, unsigned char g, unsigned char b)
	{
		R = r;
		G = g;
		B = b;
	}

	RGB(void)
	{
		R = 0;
		G = 0;
		B = 0;
	}

	bool Equals(RGB rgb)
	{
		return (R == rgb.R) && (G == rgb.G) && (B == rgb.B);
	}
};

class HSV
{
public:
	double H;
	double S;
	double V;

	HSV(double h, double s, double v)
	{
		H = h;
		S = s;
		V = v;
	}

	HSV(void)
	{
		H = 0.0;
		S = 0.0;
		V = 0.0;
	}

	bool Equals(HSV hsv)
	{
		return (H == hsv.H) && (S == hsv.S) && (V == hsv.V);
	}
};

 RGB HSVToRGB(HSV hsv);
 HSV RGBToHSV(RGB rgb);

 short hsv_distance(HSV hsv_color1, HSV color2);
