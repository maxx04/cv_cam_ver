#include "stdafx.h"
#include "color_histogram.h"

vector <hst> color_histogram::base;
bool color_histogram::base_defined = false;

color_histogram::color_histogram()
{
	if (!base_defined)
	{
		hst m;
		ushort v = 100;
		ushort s = 255;

		for (ushort h = 0; h < 179; h += 179 / 16)
			//for (s = 0; s <= 256; s += 256/8)
				 for (v = 0; v <= 256; v += 256 / 10)

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
	histogram.clear();
}

void color_histogram::add(PixelColor clr, ushort distance)
{
	//cv::compareHist();
	// maximale distance ist zwischen 0,0,0 und 256, 256, 256 
	// dann geteilt auf anzahl vorhandenen segmenten
	// oder alles ablegen mit kleinstem abstand dann rausnehmen wichtigste die kommen raus
	// parallelism

	if (histogram.size() == 0)
	{
		histogram.push_back({ clr,1 });
		return;
	}
	// gehen durch alle positionen vom histogramm
	int i = 0;
	for each (hst h in histogram) //TODO leistung schwach
	{
		short d = color_distance(clr, h.color, HSV_HV);
		// ablegen in naechst naehres
		if(d <= distance)
		{
			histogram[i].treffer++;
			histogram[i].color = middle_color(h.color, clr);
			return;
		}
		i++;
	}
	//wenn kein hat gepasst
	histogram.push_back({ clr,1 });
}

void color_histogram::draw(Point start)
{
	Mat plotResult; 
	const int width = 200;
	const int high = 120;
	plotResult.create(high, width, CV_8UC3); //TODO Leisungsverlust.
	
	plotResult.setTo(Scalar(0, 0, 127));
	int sz = histogram.size();

	if (sz == 0)
	{
		//TODO Assert hinzufügen
		cerr << "keine farben in Histogramm" << endl;
		return;
	}

	int max = 0;

	for (int i = 0; i < sz; i++)
	{
		hst h = histogram[i];
		max = MAX(h.treffer, max);
	}

	int step = width / sz;
	float mag = (float)max / (float)high;
	for (int i = 0; i < sz; i++)
	{
		hst h = histogram[i];
		int hi = (int)((float)(h.treffer)/mag); //TODO mag == 0
		cv::rectangle(plotResult, Rect(i*step, 0, step, hi) , Scalar(h.color.x, h.color.y, h.color.z), -1);
		//cv::rectangle(plotResult, Rect(i*step, 0, step, 100), Scalar((double)(16 * i), 256, 256), -1);
	}

	cvtColor(plotResult, plotResult, COLOR_HSV2BGR);
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
