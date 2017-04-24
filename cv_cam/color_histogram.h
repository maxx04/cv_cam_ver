#pragma once

struct hst { PixelColor color; ushort treffer; };
class color_histogram
{


#define COLOR_HISTOGRAMM_BREITE 64

public:
	color_histogram();
	~color_histogram();
	ushort compare(color_histogram * h);
	void reset();
	void add(PixelColor clr, ushort distance);
	void draw(Point start);
	void draw_base();

	static vector <hst> base;

protected:

	static bool base_defined;
	//PixelColor histogram[COLOR_HISTOGRAMM_BREITE]; //basis zu vergleich
	//ushort treffer[COLOR_HISTOGRAMM_BREITE];
	//ushort breite;

	vector <hst> histogram;

};

