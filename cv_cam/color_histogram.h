#pragma once
class color_histogram
{

#define COLOR_HISTOGRAMM_BREITE 8

public:
	color_histogram();
	~color_histogram();
	void add(PixelColor clr, ushort distance);
protected:
	PixelColor histogram[COLOR_HISTOGRAMM_BREITE]; // basis zu vergleich
	ushort treffer[COLOR_HISTOGRAMM_BREITE];
};

