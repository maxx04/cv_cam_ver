#include "stdafx.h"
#include "color_histogram.h"


color_histogram::color_histogram()
{
}


color_histogram::~color_histogram()
{
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
		short d = color_distance(clr, h.color, RGB_DISTANCE);
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
