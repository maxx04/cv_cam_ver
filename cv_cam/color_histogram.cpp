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
	// gehen durch alle positionen vom histogramm
	// ablegen in naechst naehres

}
