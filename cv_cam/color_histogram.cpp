#include "stdafx.h"
#include "color_histogram.h"

vector <hst> color_histogram::base;
bool color_histogram::base_defined = false;

color_histogram::color_histogram()
{
	if (!base_defined)
	{
		hst m;
		ushort v = 0;
		ushort s = 0;
		ushort schritt = 256 / 4;
		for (ushort h = 0; h < 256; h += schritt)
			for (s = 0; s < 256; s += schritt)
				 for (v = 0; v < 256; v += schritt)

		{

			m.treffer = 1;
			m.color.x = h;
			m.color.y = s;
			m.color.z = v;
			base.push_back(m);

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
	for (uint8_t i = 0; i < COLOR_HISTOGRAMM_BREITE; i++)
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
	for (uint8_t i = 0; i < COLOR_HISTOGRAMM_BREITE; i++) //TODO leistung schwach
	{
		// ablegen in naechst naehres
		dst = abs(color_distance(clr, base[i].color, RGB_3SUM));

		if(d > dst)
		{
			treff = i;
			d = dst;
			//histogram[i].treffer++;
			//histogram[i].color = middle_color(h.color, clr);
		}

	}

	histogram[treff]++; //erhoehe treffer
}

void color_histogram::draw(Point start)
{
	Mat plotResult; 
	const int width = 200;
	const int high = 120;
	plotResult.create(high, width, CV_8UC3); //TODO Leisungsverlust.
	
	plotResult.setTo(Scalar(127, 127, 127));
	int sz = COLOR_HISTOGRAMM_BREITE;

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

	plotResult.setTo(Scalar(0, 0, 127));
	int sz = base.size();

	if (sz == 0)
	{
		//TODO Assert hinzufügen
		cout << "keine farben in Histogramm" << endl;
		return;
	}

	int step = width / sz;

	for (int i = 0; i < sz; i++)
	{
		hst h = base[i];
		cv::rectangle(plotResult, Rect(i*step, 0, step, high), Scalar(h.color.x, h.color.y, h.color.z), -1);
	}

	cvtColor(plotResult, plotResult, COLOR_HSV2BGR);
	imshow("hist", plotResult);
}
