#include "stdafx.h"
#include "color_histogram.h"

hst color_histogram::base[COLOR_HISTOGRAM_BREITE];
bool color_histogram::base_defined = false;

color_histogram::color_histogram()
{
	if (!base_defined)
	{
		//berechnen HSV verteilung und dann konvertieren in RGB
		hst m;
		int i = 0;
		double h = 0.0;
		double s = 0.5;
		double v = 0.5;
		double schritt = 10.0;

		HSV hsv(0.0,0.0,0.0);
		RGB rgb(0.0, 0.0, 0.0);

		for (h = 0; h < 360; h += schritt)


		{

			hsv.H = h;
			hsv.S = s;
			hsv.V = v;

			rgb = ::HSVToRGB(hsv);

			base[i].color.x = rgb.R;
			base[i].color.y = rgb.G;
			base[i].color.z = rgb.B;
			i++;

		}


		//Grauwerte
		for (ushort b = 0; b < 256; b += 16)
				{

					m.treffer = 1;
					m.color.x = b;
					m.color.y = b;
					m.color.z = b;
					base[i++] = m;

				}



		base_defined = true;
	}
}


color_histogram::~color_histogram()
{

}


ushort color_histogram::compare(color_histogram* h)
{
	return 1;
}


void color_histogram::reset()
{
	for (uint8_t i = 0; i < COLOR_HISTOGRAM_BREITE; i++)
	{
		histogram[i] = 0;
	}
	//histogram.clear();
}

void color_histogram::add(PixelColor clr, ushort distance)
{
	//cv::compareHist();
	// maximale distance ist zwischen 0,0,0 und 256, 256, 256 
	// dann geteilt auf anzahl vorhandenen segmenten
	// oder alles ablegen mit kleinstem abstand dann rausnehmen wichtigste die kommen raus
	// parallelism

	//if (histogram.size() == 0)
	//{
	//	histogram.push_back({ clr,1 });
	//	return;
	//}
	// gehen durch alle positionen vom histogramm
	//und finde minimale abstand

	short d = 1000;
	short dst;

	int treff = 0;
	for (uint8_t i = 0; i < COLOR_HISTOGRAM_BREITE; i++) //TODO leistung schwach
	{
		// ablegen in naechst naehres
		dst = abs(color_distance(clr, base[i].color, RGB_SQUARE));

		if(dst < d)
		{
			treff = i;
			d = dst;
		}

	}

	histogram[treff]++; //erhoehe treffer
}

void color_histogram::draw(Point start)
{
	Mat plotResult; 
	const int width = 600;
	const int high = 120;
	plotResult.create(high, width, CV_8UC3); //TODO Leisungsverlust.
	
	plotResult.setTo(Scalar(127, 127, 127));
	int sz = COLOR_HISTOGRAM_BREITE;

	if (sz == 0)
	{
		//TODO Assert hinzufügen
		cerr << "keine farben in Histogramm" << endl;
		return;
	}

	int max = 0;

	for (int i = 0; i < sz; i++)
	{
		max = MAX(histogram[i], max);
	}

	int step = width / sz;
	float mag = (float)max / (float)high;
	for (int i = 0; i < sz; i++)
	{
		//hst h = histogram[i];
		int hi = (int)((float)(histogram[i])/mag); //TODO mag == 0
		cv::rectangle(plotResult, Rect(i*step, 0, step, hi) , Scalar(base[i].color.x, base[i].color.y, base[i].color.z), -1);
		//cv::rectangle(plotResult, Rect(i*step, 0, step, 100), Scalar((double)(16 * i), 256, 256), -1);
	}

	//cvtColor(plotResult, plotResult, COLOR_HSV2BGR);
	imshow("hist", plotResult);
}

void color_histogram::draw_base()
{
	Mat plotResult;
	const int width = 1500;
	const int high = 120;
	plotResult.create(high, width, CV_8UC3); //TODO Leisungsverlust.

	plotResult.setTo(Scalar(0, 0, 0));
	int sz = COLOR_HISTOGRAM_BREITE;

	int step = width / sz;

	for (int i = 0; i < sz; i++)
	{
		cv::rectangle(plotResult, Rect(i*step, 0, step, high), Scalar(base[i].color.x, base[i].color.y, base[i].color.z), -1);
	}

	//cvtColor(plotResult, plotResult, COLOR_HSV2BGR);
	imshow("hist", plotResult);
}
