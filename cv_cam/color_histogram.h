#pragma once
class color_histogram
{
	struct hst { PixelColor color; ushort treffer;};

#define COLOR_HISTOGRAMM_BREITE 8

public:
	color_histogram();
	~color_histogram();
	void reset();
	void add(PixelColor clr, ushort distance);
	void draw(Point start, OutputArray windowName);
protected:
	//PixelColor histogram[COLOR_HISTOGRAMM_BREITE]; // basis zu vergleich
	//ushort treffer[COLOR_HISTOGRAMM_BREITE];
	//ushort breite;

	vector <hst> histogram;

};

