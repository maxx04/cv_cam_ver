#include "color_histogram.h"
#include <iostream>
#include "plot.hpp"

hst_hsv color_histogram::base[COLOR_HISTOGRAM_BREITE];
bool color_histogram::base_defined = false;

color_histogram::color_histogram()
{
	CV_Assert(COLOR_HISTOGRAM_BREITE <= 256);
	CV_Assert(COLOR_HISTOGRAM_COLORS % 8 == 0);	
	CV_Assert(COLOR_HISTOGRAM_GRAYS % 8 == 0);

	if (!base_defined)
	{
		int i = 0;

		double schritt = 360.0 / double(COLOR_HISTOGRAM_COLORS);

		HSV hsv(0.0, 0.7, 0.5);	// TODO S und V ermitteln  für beste Ergebnisse oder Egal?

		for (double h = 0; h < 360; h += schritt)
		{
			hsv.H = h;
			base[i].color = hsv;
			i++;
		}

		//Grauwerte
		unsigned char b = 0;

		for (i = COLOR_HISTOGRAM_COLORS; i < COLOR_HISTOGRAM_BREITE; i++ )
		{
			base[i].color = RGBToHSV2(RGB(b, b, b));
			b += (256 / COLOR_HISTOGRAM_GRAYS);
		}

		base_defined = true;
	}

	reset();
}


color_histogram::~color_histogram()
{

}


ushort color_histogram::compare(color_histogram* h)
{
	ushort sum = 0;
	for (uint8_t i = 0; i < COLOR_HISTOGRAM_BREITE; i++)
	{
		sum += abs(histogram[i] - h->get_frequency(i));
	}
	return sum;
}

ushort color_histogram::compare(color_histogram h)
{
	ushort sum = 0;
	for (uint8_t i = 0; i < COLOR_HISTOGRAM_BREITE; i++)
	{
		sum += abs(histogram[i] - h.get_frequency(i));
	}
	return sum;
}

inline uint8_t color_histogram::get_frequency(int i)
{
	return histogram[i] ? (i > 0 && i < COLOR_HISTOGRAM_BREITE) : 0;
}

void color_histogram::reset()
{
	for (uint8_t i = 0; i < COLOR_HISTOGRAM_BREITE; i++)
	{
		histogram[i] = 0;
	}
}


void color_histogram::add(PixelColor clr)
{
	// gehen durch alle positionen vom histogramm
	//und finde minimale abstand

	short d = 10000;
	short dst;

	HSV hsv_color = ::RGBToHSV2(RGB(clr.z, clr.y, clr.x));

	int treff = 0;
	for (uint8_t i = 0; i < COLOR_HISTOGRAM_BREITE; i++) //OPTI leistung schwach
	{
		// ablegen in naechst naehres
		//dst = abs(color_distance(clr, base[i].color, RGB_SUM_EACH_COLOR));
		dst = hsv_distance(hsv_color, base[i].color);

		if (dst < d)
		{
			treff = i;
			d = dst;
		}

	}

	if (histogram[treff] == 256)  // HACK
		cerr << "Histogram Treffanzahl mehr als 256 " << endl;
	else
		histogram[treff]++; //erhoehe treffer
}

void color_histogram::draw(Point start)
{
	Mat plotResult;
	const int width = 10 * COLOR_HISTOGRAM_BREITE;
	const int high = 100;
	const int base_high = 10; //base Histogramm
	plotResult.create(high + base_high, width, CV_8UC3); //TODO Leisungsverlust.

	plotResult.setTo(Scalar(127, 127, 127));
	int sz = COLOR_HISTOGRAM_BREITE;

	if (sz == 0)
	{
		//TODO Assert hinzufügen
		std::cerr << "keine farben in Histogramm" << std::endl;
		return;
	}

	int max = 0;

	for (int i = 0; i < sz; i++)
	{
		max = MAX(histogram[i], max);
	}

	int step = width / sz;
	float mag = (float)max / (float)high;
	RGB base_RGB(0, 0, 0);

	for (int i = 0; i < sz; i++)
	{
		//hst h = histogram[i];
		base_RGB = HSVToRGB(base[i].color);
		int hi = (int)((float)(histogram[i]) / mag); //TODO mag == 0
		rectangle(plotResult, Rect(i * step, high - hi, step, high), Scalar(base_RGB.B, base_RGB.G, base_RGB.R), -1);
		rectangle(plotResult, Rect(i * step, high, step, base_high + high), Scalar(base_RGB.B, base_RGB.G, base_RGB.R), -1);
		//rectangle(plotResult, Rect(i*step, 0, step, 100), Scalar((double)(16 * i), 256, 256), -1);
	}

	//cvtColor(plotResult, plotResult, COLOR_HSV2BGR);
	imshow("hist", plotResult);
}

Scalar color_histogram::get_mean_color()
{
	int max = 0;
	int position = 0;
	Scalar main_color;
	for (ushort i = 0; i < COLOR_HISTOGRAM_BREITE; i++)
	{
		if (histogram[i] > max)
		{
			position = i;
		}
	}

	RGB c = HSVToRGB(base[position].color);

	return main_color = Scalar(c.B, c.G, c.R);
}

